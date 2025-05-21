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

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

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
 * @brief Close socket
 * @param sock Socket
 */
GF_EXPORT void gf_network_close(int sock);

#ifdef __cplusplus
}
#endif

#endif
