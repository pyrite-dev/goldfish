/**
 * @file gf_type/network.h
 * @~english
 * @brief Type definitions related to network interface
 */

#ifndef __GF_TYPE_NETWORK_H__
#define __GF_TYPE_NETWORK_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_NETWORK
typedef struct gf_network_t gf_network_t;

/* External library */

/* Engine */
#include <gf_type/core.h>

/* Standard */

/**
 * @struct gf_network_t
 * @~english
 * @brief Network interface
 *
 * @var gf_network_t::engine
 * @brief Engine instance
 */
GF_DECLARE_TYPE(network, {
	gf_engine_t* engine;
});
#else
typedef void gf_network_t;
#endif

#endif
