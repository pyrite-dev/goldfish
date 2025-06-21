#define GF_EXPOSE_NETWORK

#include <gf_pre.h>

/* External library */
#include <gf_netdrv.h>
#include <minisocket.h>
#include <stb_ds.h>

/* Interface */
#include <gf_network.h>

/* Engine */
#include <gf_type/compat.h>
#include <gf_log.h>
#include <gf_util.h>

/* Standard */
#include <stdlib.h>
#include <string.h>
#include <time.h>

gf_uint32_t gf_network_id(const char* str) {
	gf_uint32_t id = *(gf_uint32_t*)&str[0];
	return htonl(id);
}

static void gf_network_key(gf_network_t* net) {
	gf_uint64_t r;
	int	    count = 0;
	int	    i;
	for(i = 0; i < sizeof(net->random_seed); i++) {
		if(count == 0) {
			count = sizeof(r);
			r     = gf_util_random(net->engine);
		}

		net->random_seed[i] = r & 0xff;

		count--;
		r = r >> 8;
	}
	compact_x25519_keygen(net->private_key, net->public_key, net->random_seed);
}

static void gf_network_key_hex(char* out, gf_uint8_t key[X25519_KEY_SIZE]) {
	int	    i;
	const char* hex = "0123456789ABCDEF";
	for(i = 0; i < X25519_KEY_SIZE; i++) {
		out[2 * i + 0] = hex[(key[i + 0] >> 4) & 0xf];
		out[2 * i + 1] = hex[(key[i + 1] >> 0) & 0xf];
	}
}

static int gf_network_hex(char c) {
	if('0' <= c && c <= '9') {
		return c - '0';
	} else if('A' <= c && c <= 'F') {
		return c - 'A' + 10;
	} else if('a' <= c && c <= 'f') {
		return c - 'a' + 10;
	}
	return 0;
}

static void gf_network_close(int fd) {
#ifdef _WIN32
	closesocket(fd);
#else
	close(fd);
#endif
}

static int gf_network_socket(gf_engine_t* engine, const char* type, const char* host, int port) {
	int		   fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in addr;

	gf_log_function(engine, "Binding to %s:%d", host, port);

#ifdef _WIN32
	if(fd == INVALID_SOCKET) net->fd = -1;
#endif
	if(fd == -1) {
		gf_log_function(engine, "Failed to create socket", "");
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family	     = AF_INET;
	addr.sin_port	     = htons(port);
	addr.sin_addr.s_addr = inet_addr(host);
	if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		gf_log_function(engine, "Failed to bind", "");
		gf_network_close(fd);
		return -1;
	}
	if(listen(fd, 64) == -1) {
		gf_log_function(engine, "Failed to listen", "");
		gf_network_close(fd);
		return -1;
	}
	return fd;
}

static gf_network_t* gf_network_secure(gf_engine_t* engine, ms_interface_t* net, int server) {
	int	      len    = strlen("ClientHello") + 1 + 1 + 1 + 64 + 2;
	int	      first  = 1;
	char**	      list   = NULL;
	char*	      buffer = malloc(1);
	int	      brk    = 0;
	ms_buffer_t*  buf;
	time_t	      began_at;
	int	      good = 0;
	gf_network_t* r	   = malloc(sizeof(*r));
	memset(r, 0, sizeof(*r));
	r->engine = engine;
	r->net	  = net;
	r->fd	  = -1;

	gf_network_key(r);

	buffer[0] = 0;

	gf_log_function(engine, "GFSL handshaking", "");

	began_at = time(NULL);
	if(!server) {
		gf_log_function(engine, "Sent ClientHello", "");
		buf = ms_wbuffer(net, len);
		memcpy(buf->data, "ClientHello 1 ", len - 64 - 2);
		gf_network_key_hex(((char*)buf->data) + (len - 64 - 2), r->public_key);
		memcpy(((char*)buf->data) + (len - 2), "\r\n", 2);
	} else {
		first = 0;
		buf   = ms_rbuffer(net, 1);
	}
	while(1) {
		int st = ms_step(net);
		if((time(NULL) - began_at) >= 3) {
			gf_log_function(engine, "Timeout", "");
			brk = 1;
		}
		if(st != 0 || net->state >= MS_STATE_FAILED) {
			brk = 1;
		} else if(net->state == MS_STATE_AFTER_WRITE && good) {
			break;
		} else if((net->state == MS_STATE_AFTER_WRITE && first) || (net->state == MS_STATE_AFTER_READ && !good)) {
			first = 0;
			buf   = ms_rbuffer(net, 1);
		} else if(net->state == MS_STATE_READ_COMPLETE) {
			unsigned char c = *(unsigned char*)buf->data;
			if(c == '\n') {
				int i;
				int incr = 0;
				int len	 = strlen(buffer);
				for(i = 0; i <= len; i++) {
					if(buffer[i] == ' ' || i == len) {
						char* arg = buffer + incr;
						buffer[i] = 0;
						arrput(list, arg);
						incr = i + 1;
						if(i == len) break;
					}
				}

				if(list != NULL && arrlen(list) > 0) {
					if(strcmp(list[0], server ? "ClientHello" : "ServerHello") == 0 && arrlen(list) == 3 && strcmp(list[1], "1") == 0 && strlen(list[2]) == 64) {
						gf_uint8_t their_public[X25519_KEY_SIZE];
						gf_log_function(engine, "Got %s", list[0]);
						for(i = 0; i < X25519_KEY_SIZE; i++) {
							their_public[i] = (gf_network_hex(list[2][i * 2 + 0]) << 4) | gf_network_hex(list[2][i * 2 + 1]);
						}
						if(server) {
							int	     len2 = strlen("ServerHello") + 1 + 1 + 1 + 64 + 2;
							ms_buffer_t* wbuf = ms_wbuffer(net, len2);
							memcpy(wbuf->data, "ServerHello 1 ", len2 - 64 - 2);
							gf_network_key_hex(((char*)wbuf->data) + (len2 - 64 - 2), r->public_key);
							memcpy(((char*)wbuf->data) + (len2 - 2), "\r\n", 2);
						} else {
							int	     len2 = strlen("ClientAccept") + 2;
							ms_buffer_t* wbuf = ms_wbuffer(net, len2);
							memcpy(wbuf->data, "ClientAccept\r\n", len2);
							good = 1;
							continue;
						}
						compact_x25519_shared(r->shared_secret, r->private_key, their_public);
					} else if(server && strcmp(list[0], "ClientAccept") == 0 && arrlen(list) == 1) {
						gf_log_function(engine, "Got %s", list[0]);
						break;
					} else {
						brk = 1;
					}
					arrfree(list);
					list = NULL;
				} else {
					brk = 1;
				}
				free(buffer);
				buffer	  = malloc(1);
				buffer[0] = 0;
			} else if(c != '\r') {
				char* old = buffer;
				buffer	  = malloc(strlen(old) + 2);
				strcpy(buffer, old);
				buffer[strlen(old)]	= c;
				buffer[strlen(old) + 1] = 0;
				free(old);
			}
		}
		if(brk) break;
	}
	free(buffer);
	if(list != NULL) arrfree(list);
	if(brk) {
		gf_log_function(engine, "Handshake failed", "");
	} else {
		gf_log_function(engine, "Handshake success", "");
	}

	return r;
}

gf_network_t* gf_network_secure_tcp(gf_engine_t* engine, const char* host, int port) {
	ms_interface_t* net;
	gf_network_t*	r;
	gf_log_function(engine, "Connecting to %s:%d", host, port);

	net = ms_tcp(host, port);
	if(net == NULL) {
		gf_log_function(engine, "Connection failure", "");
		return NULL;
	}

	while(1) {
		int st = ms_step(net);
		if(net->state == MS_STATE_CONNECTED) break;
		if(net->state >= MS_STATE_FAILED || st != 0) {
			gf_log_function(engine, "Connection failure", "");
			ms_destroy(net);
			return NULL;
		}
	}

	gf_log_function(engine, "Connected", "");

	r = gf_network_secure(engine, net, 0);
	return r;
}

gf_network_t* gf_network_secure_tcp_server(gf_engine_t* engine, const char* host, int port) {
	gf_network_t* net = malloc(sizeof(*net));
	memset(net, 0, sizeof(*net));
	net->engine = engine;
	net->fd	    = -1;

	net->fd = gf_network_socket(engine, "tcp", host, port);
	if(net->fd == -1) {
		free(net);
		return NULL;
	}

	return net;
}

int gf_network_secure_server_step(gf_network_t* net) {
	fd_set	       fds;
	int	       s;
	struct timeval tv;
	FD_ZERO(&fds);
	FD_SET(net->fd, &fds);

	tv.tv_sec  = 0;
	tv.tv_usec = 0;

	s = select(FD_SETSIZE, &fds, NULL, NULL, &tv);
	if(s == -1) {
		return -1;
	}
	if(s > 0) {
		struct sockaddr_in addr;
		int		   len = sizeof(addr);
		int		   fd  = accept(net->fd, (struct sockaddr*)&addr, &len);
		gf_network_t*	   cn;
		ms_interface_t*	   nint;

		gf_log_function(net->engine, "Connection accepted", "");

		ms_non_block(fd);
		nint = ms_user(fd);
		cn   = gf_network_secure(net->engine, nint, 1);
	}
	return 0;
}

void gf_network_destroy(gf_network_t* net) {
	if(net->net != NULL) ms_destroy(net->net);
	if(net->fd != -1) gf_network_close(net->fd);
	gf_log_function(net->engine, "Destroyed network interface", "");
	free(net);
}
