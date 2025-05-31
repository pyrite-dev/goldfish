/**
 * @file gf_netdrv.h
 * @~english
 * @brief Network driver headers
 * @note User should not include this, this header is used internally
 */

#ifndef __GF_NETDRV_H__
#define __GF_NETDRV_H__

#ifdef _WIN32
#include <winsock.h>

#define _EINPROGRESS WSAEINPROGRESS
#define _EINTR WSAEINTR
#define _EWOULDBLOCK WSAEWOULDBLOCK
#define _ENOTCONN WSAENOTCONN
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netdb.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>

#define _EINPROGRESS EINPROGRESS
#define _EINTR EINTR
#define _EWOULDBLOCK EWOULDBLOCK
#define _ENOTCONN ENOTCONN
#endif

#endif
