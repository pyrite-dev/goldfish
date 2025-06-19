/**
 * @file gf_network.h
 * @~english
 * @brief Network interface
 */

#ifndef __GF_NETWORK_H__
#define __GF_NETWORK_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/network.h>

/* Engine */
#include <gf_type/core.h>
#include <gf_type/compat.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Convert packet ID string to integer
 * @param str Packet ID
 * @return Packet ID in integer
 */
GF_EXPORT gf_uint32_t gf_network_id(const char* str);

/**
 * @~english
 * @brief Connect to host using TCP, and handshake using GoldFish Secure Layer
 * @param engine Engine instance
 * @param host Host
 * @param port Port
 * @return Network interface
 */
GF_EXPORT gf_network_t* gf_network_secure_tcp(gf_engine_t* engine, const char* host, int port);

#ifdef __cplusplus
}
#endif

#endif
