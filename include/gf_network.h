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
void gf_network_begin(void);

/**
 * @~english
 * @brief Cleanup network interface
 */
void gf_network_end(void);

#ifdef __cplusplus
}
#endif

#endif
