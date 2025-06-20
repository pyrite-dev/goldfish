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

static gf_network_t* gf_network_secure(gf_engine_t* engine, ms_interface_t* net, int server) {
	int	      len    = strlen("ClientHello") + 1 + 1 + 1 + 64 + 2;
	int	      first  = 1;
	char**	      list   = NULL;
	char*	      buffer = malloc(1);
	int	      brk    = 0;
	ms_buffer_t*  buf;
	gf_network_t* r = malloc(sizeof(*r));
	memset(r, 0, sizeof(*r));
	r->engine = engine;
	r->net	  = net;

	gf_network_key(r);

	buffer[0] = 0;

	gf_log_function(engine, "GFSL handshaking", "");

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
		if(st != 0 || net->state >= MS_STATE_FAILED) {
			brk = 1;
		} else if((net->state == MS_STATE_AFTER_WRITE && first) || net->state == MS_STATE_AFTER_READ) {
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
							break;
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
		ms_destroy(net);
		free(r);
		r = NULL;
	} else {
		gf_log_function(engine, "Handshake success", "");
	}

	return r;
}

gf_network_t* gf_network_secure_tcp(gf_engine_t* engine, const char* host, int port) {
	ms_interface_t* net;
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

	return gf_network_secure(engine, net, 0);
}
