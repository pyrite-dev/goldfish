#define GF_EXPOSE_NETWORK

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>
#include <gf_netdrv.h>

/* Interface */
#include <gf_network.h>

/* Engine */
#include <gf_type/compat.h>
#include <gf_log.h>

/* Standard */
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

/**
 * 3 seconds should be enough
 * i mean - if your ping is over 3000ms
 * it would be unplayable...
 */
#define NET_TIMEOUT 3

gf_uint32_t gf_network_id(const char* str) {
	gf_uint32_t id = *(gf_uint32_t*)&str[0];
	return htonl(id);
}

int gf_network_socket(const char* type) {
	int sock = -1;
	int v;
	if(strcmp(type, "tcp") == 0) {
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	} else if(strcmp(type, "udp") == 0) {
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}

#ifdef _WIN32
	if(sock == INVALID_SOCKET) sock = -1;
#endif

	if(sock == -1) return -1;

	v = 65535;
	setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&v, sizeof(v));
	v = 65535;
	setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&v, sizeof(v));

	if(strcmp(type, "tcp") == 0) {
		v = 1;
		setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&v, sizeof(v));
	}

	return sock;
}

void gf_network_close(int sock) {
#ifdef _WIN32
	closesocket(sock);
#else
	close(sock);
#endif
}

void gf_network_begin(void) {
#ifdef _WIN32
	WSADATA wsa;
	WSAStartup(MAKEWORD(1, 1), &wsa);
#endif
}

int gf_network_get_error(void) {
#ifdef _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}

void gf_network_non_block(int sock) {
	/* TODO: what is difference between this and fcntl O_NONBLOCK/O_NDELAY? */
#ifdef _WIN32
	u_long val = 1;
	ioctlsocket(sock, FIONBIO, &val);
#else
	int val = 1;
	ioctl(sock, FIONBIO, &val);
#endif
}

void gf_network_block(int sock) {
	/* TODO: what is difference between this and fcntl O_NONBLOCK/O_NDELAY? */
#ifdef _WIN32
	u_long val = 0;
	ioctlsocket(sock, FIONBIO, &val);
#else
	int val = 0;
	ioctl(sock, FIONBIO, &val);
#endif
}

char* gf_network_error(int code) {
	char* r;
#ifdef _WIN32
	void* msg;
	int   i;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 0, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&msg, 0, 0);
	r = malloc(strlen((char*)msg) + 1);
	strcpy(r, (char*)msg);
	LocalFree(msg);

	for(i = strlen(r) - 1; i >= 0; i--) {
		char old = r[i];
		r[i]	 = 0;
		if(old == '.') break;
	}
#else
	char* str = (char*)strerror(code);
	r	  = malloc(strlen(str) + 1);
	strcpy(r, str);
#endif
	return r;
}

gf_network_buffer_t* gf_network_wbuffer(gf_network_t* net, size_t size) {
	gf_network_buffer_t* r = malloc(sizeof(*r));
	r->data		       = malloc(size);
	r->size		       = size;
	r->_size	       = size;
	r->seek		       = 0;

	arrput(net->wqueue, r);

	return r;
}

gf_network_buffer_t* gf_network_rbuffer(gf_network_t* net, size_t size) {
	gf_network_buffer_t* r = malloc(sizeof(*r));
	r->data		       = malloc(size);
	r->size		       = size;
	r->_size	       = size;
	r->seek		       = 0;

	arrput(net->rqueue, r);

	return r;
}

gf_network_t* gf_network_init(gf_engine_t* engine, const char* host, int port) {
	struct hostent* h;
	int		i;
	gf_network_t*	r = malloc(sizeof(*r));
	memset(r, 0, sizeof(*r));
	r->engine = engine;

	h = gethostbyname(host);
	if(h == NULL || h->h_addrtype != AF_INET) {
		gf_log_function(engine, "Failed to resolve %s", host);
		free(r);
		return NULL;
	}

	r->sock = -1;

	r->state	   = GF_NETWORK_STATE_PRE_CONNECT;
	r->port		   = port;
	r->u.connect.index = 0;
	for(i = 0; h->h_addr_list[i] != NULL; i++) {
		gf_uint32_t addr = *(gf_uint32_t*)h->h_addr_list[i];
		arrput(r->u.connect.address, addr);
	}

	r->wqueue = NULL;
	r->rqueue = NULL;
}

gf_network_t* gf_network_tcp(gf_engine_t* engine, const char* host, int port) {
	gf_network_t* r = gf_network_init(engine, host, port);
	return r;
}

#define ISFINEERR(x) ((x) == _EWOULDBLOCK || (x) == _EINPROGRESS)

int gf_network_step(gf_network_t* net) {
	if(net->state >= GF_NETWORK_STATE_FAILED) return 1;

	if(net->state == GF_NETWORK_STATE_PRE_CONNECT) {
		struct sockaddr_in addr;
		int		   st;
		int		   r;
		if(net->sock != -1) {
			gf_network_close(net->sock);
			net->sock = -1;
		}

		if(arrlen(net->u.connect.address) <= net->u.connect.index) {
			net->state = GF_NETWORK_STATE_FAILED_CONNECT;
			arrfree(net->u.connect.address);
			return 1;
		}

		net->sock = gf_network_socket("tcp");
		if(net->sock == -1) {
			net->state = GF_NETWORK_STATE_FAILED_SOCKET;
			arrfree(net->u.connect.address);
			return 1;
		}
		gf_network_non_block(net->sock);

		memset(&addr, 0, sizeof(addr));
		addr.sin_family	     = AF_INET;
		addr.sin_port	     = htons(net->port);
		addr.sin_addr.s_addr = net->u.connect.address[net->u.connect.index++];

		st = connect(net->sock, (struct sockaddr*)&addr, sizeof(addr));
		r  = gf_network_get_error();
		if(st >= 0 || (ISFINEERR(r) || r == _EINTR)) {
			net->state	    = GF_NETWORK_STATE_CONNECT;
			net->u.connect.last = time(NULL);
		}
	} else if(net->state == GF_NETWORK_STATE_CONNECT) {
		/* HACK: unreadable */
		time_t	       t = time(NULL);
		fd_set	       fds;
		struct timeval tv;
		int	       st;
		int	       conn = 0;
		int	       r;
		int	       len = sizeof(r);
		tv.tv_sec	   = 0;
		tv.tv_usec	   = 0;
		FD_ZERO(&fds);
		FD_SET(net->sock, &fds);

		st = getsockopt(net->sock, SOL_SOCKET, SO_ERROR, (void*)&r, &len);
		if(r != 0 && !ISFINEERR(r)) {
			char* err = gf_network_error(r);
			gf_log_function(net->engine, "%s", err);
			free(err);
			net->state = GF_NETWORK_STATE_PRE_CONNECT;
		} else {
			st = select(FD_SETSIZE, NULL, &fds, NULL, &tv);

			if(st > 0) {
				conn = r == 0 ? 1 : 0;
			}
			if(!conn) {
				if((t - net->u.connect.last) >= NET_TIMEOUT) {
					gf_log_function(net->engine, "Timeout", "");
					net->state = GF_NETWORK_STATE_PRE_CONNECT;
				}
			}
		}
		if(conn) {
			net->state = GF_NETWORK_STATE_CONNECTED;
			gf_log_function(net->engine, "Connected", "");
		}
	} else if(net->state == GF_NETWORK_STATE_CONNECTED) {
		if(arrlen(net->wqueue) > 0) {
			net->state = GF_NETWORK_STATE_WRITE;
		} else if(arrlen(net->rqueue) > 0) {
			net->state = GF_NETWORK_STATE_READ;
		}
	} else if(net->state == GF_NETWORK_STATE_WRITE) {
		gf_network_buffer_t* buf = net->wqueue[0];
		int		     s	 = send(net->sock, (unsigned char*)buf->data + buf->seek, buf->_size - buf->seek, 0);
		int		     r	 = 0;
		if(s < 0) r = gf_network_get_error();
		if((s < 0 && !ISFINEERR(r)) || s == 0) {
			buf->size  = buf->seek;
			net->state = GF_NETWORK_STATE_WRITE_PART;
			gf_log_function(net->engine, "Could only write %lu bytes, buffer was %lu bytes", (unsigned long)buf->size, (unsigned long)buf->_size);
		} else if(s > 0) {
			buf->seek += s;
			if(buf->seek >= buf->_size) net->state = GF_NETWORK_STATE_WRITE_COMPLETE;
		}
	} else if(net->state == GF_NETWORK_STATE_WRITE_COMPLETE || net->state == GF_NETWORK_STATE_WRITE_PART) {
		net->state = GF_NETWORK_STATE_AFTER_WRITE;
	} else if(net->state == GF_NETWORK_STATE_AFTER_WRITE) {
		int st = net->wqueue[0]->seek != net->wqueue[0]->_size ? GF_NETWORK_STATE_FAILED_WRITE : GF_NETWORK_STATE_CONNECTED;

		free(net->wqueue[0]->data);
		free(net->wqueue[0]);

		arrdel(net->wqueue, 0);
		net->state = st;
	} else if(net->state == GF_NETWORK_STATE_READ) {
		gf_network_buffer_t* buf = net->rqueue[0];
		int		     s	 = recv(net->sock, (unsigned char*)buf->data + buf->seek, buf->_size - buf->seek, 0);
		int		     r	 = 0;
		if(s < 0) r = gf_network_get_error();
		if((s < 0 && !ISFINEERR(r)) || s == 0) {
			buf->size  = buf->seek;
			net->state = GF_NETWORK_STATE_READ_PART;
			gf_log_function(net->engine, "Could only read %lu bytes, buffer was %lu bytes", (unsigned long)buf->size, (unsigned long)buf->_size);
		} else if(s > 0) {
			buf->seek += s;
			if(buf->seek >= buf->_size) net->state = GF_NETWORK_STATE_READ_COMPLETE;
		}
	} else if(net->state == GF_NETWORK_STATE_READ_COMPLETE || net->state == GF_NETWORK_STATE_READ_PART) {
		net->state = GF_NETWORK_STATE_AFTER_READ;
	} else if(net->state == GF_NETWORK_STATE_AFTER_READ) {
		int st = net->rqueue[0]->seek != net->rqueue[0]->_size ? GF_NETWORK_STATE_FAILED_READ : GF_NETWORK_STATE_CONNECTED;

		free(net->rqueue[0]->data);
		free(net->rqueue[0]);

		arrdel(net->rqueue, 0);
		net->state = st;
	}
	return 0;
}

void gf_network_end(void) {
#ifdef _WIN32
	WSACleanup();
#endif
}
