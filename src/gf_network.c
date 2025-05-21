#define GF_EXPOSE_NETWORK

#include <gf_pre.h>

/* External library */
#ifdef _WIN32
#include <winsock.h>
#endif

/* Interface */
#include <gf_network.h>

/* Engine */

/* Standard */
#include <string.h>
#ifndef _WIN32
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <unistd.h>
#endif

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

void gf_network_end(void) {
#ifdef _WIN32
	WSACleanup();
#endif
}
