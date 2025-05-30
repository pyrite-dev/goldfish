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

#ifndef EINPROGRESS
#define EINPROGRESS WSAEINPROGRESS
#endif
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netdb.h>

#include <sys/ioctl.h>
#include <unistd.h>
#endif

#endif
