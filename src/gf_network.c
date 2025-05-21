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
