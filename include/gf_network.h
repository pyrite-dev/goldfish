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
#include <gf_type/compat.h>
#include <gf_type/core.h>

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
 * @brief Initialize network interface
 */
GF_EXPORT void gf_network_begin(void);

/**
 * @~english
 * @brief Cleanup network interface
 */
GF_EXPORT void gf_network_end(void);

/**
 * @~english
 * @brief Create socket
 * @param type Socket type, `udp` or `tcp`
 * @return Socket
 */
GF_EXPORT int gf_network_socket(const char* type);

/**
 * @~english
 * @brief Get last network error
 * @return Network error code
 */
GF_EXPORT int gf_network_get_error(void);

/**
 * @~english
 * @brief Get network error string
 * @return Network error string
 */
GF_EXPORT char* gf_network_error(int code);

/**
 * @~english
 * @brief Do network single step
 * @param net Network interface
 * @return `0` if successful
 */
GF_EXPORT int gf_network_step(gf_network_t* net);

/**
 * @~english
 * @brief Make socket non-blocking
 * @param sock Socket
 */
GF_EXPORT void gf_network_non_block(int sock);

/**
 * @~english
 * @brief Make socket blocking
 * @param sock Socket
 */
GF_EXPORT void gf_network_block(int sock);

/**
 * @~english
 * @brief Connect to host using TCP/IP
 * @param engine Engine instance
 * @param host Host name
 * @param port Port
 * @return Network interface
 */
GF_EXPORT gf_network_t* gf_network_tcp(gf_engine_t* engine, const char* host, int port);

/**
 * @~english
 * @brief Close socket
 * @param sock Socket
 */
GF_EXPORT void gf_network_close(int sock);

#ifdef __cplusplus
}
#endif

#endif
