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

gf_network_t* gf_network_tcp(gf_engine_t* engine, const char* host, int port) {
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

	r->sock = gf_network_socket("tcp");
	if(r->sock == -1) {
		free(r);
		return NULL;
	}
	gf_network_non_block(r->sock);

	r->state	   = GF_NETWORK_STATE_PRE_CONNECT;
	r->port		   = port;
	r->u.connect.index = 0;
	for(i = 0; h->h_addr_list[i] != NULL; i++) {
		gf_uint32_t addr = *(gf_uint32_t*)h->h_addr_list[i];
		arrput(r->u.connect.address, addr);
	}

	return r;
}

int gf_network_step(gf_network_t* net) {
	if(net->state >= GF_NETWORK_STATE_FAILED) return 1;

	if(net->state == GF_NETWORK_STATE_PRE_CONNECT) {
		struct sockaddr_in addr;
		if(arrlen(net->u.connect.address) <= net->u.connect.index) {
			net->state = GF_NETWORK_STATE_FAILED_CONNECT;
			arrfree(net->u.connect.address);
			return 1;
		}
		memset(&addr, 0, sizeof(addr));
		addr.sin_family	     = AF_INET;
		addr.sin_port	     = net->port;
		addr.sin_addr.s_addr = net->u.connect.address[net->u.connect.index++];
		if(connect(net->sock, (struct sockaddr*)&addr, sizeof(addr)) >= 0 || (gf_network_get_error() == EINPROGRESS || gf_network_get_error() == EINTR)) {
			net->state	    = GF_NETWORK_STATE_CONNECT;
			net->u.connect.last = time(NULL);
		}
	} else if(net->state == GF_NETWORK_STATE_CONNECT) {
	}
	return 0;
}

void gf_network_end(void) {
#ifdef _WIN32
	WSACleanup();
#endif
}
