/**
 * @file gf_type/network.h
 * @~english
 * @brief Type definitions related to network interface
 */

#ifndef __GF_TYPE_NETWORK_H__
#define __GF_TYPE_NETWORK_H__

#include <gf_pre.h>
#include <gf_macro.h>

/**
 * @~english
 * @brief Network states
 */
enum GF_NETWORK_STATES {
	GF_NETWORK_STATE_PRE_CONNECT = 0,
	GF_NETWORK_STATE_CONNECT,
	GF_NETWORK_STATE_CONNECTED,
	GF_NETWORK_STATE_FAILED,
	GF_NETWORK_STATE_FAILED_CONNECT,
	GF_NETWORK_STATE_FAILED_SOCKET
};

#ifdef GF_EXPOSE_NETWORK
typedef struct gf_network_t gf_network_t;
typedef union gf_network_state_t gf_network_state_t;
typedef struct gf_network_state_connect_t gf_network_state_connect_t;

/* External library */

/* Engine */
#include <gf_type/core.h>
#include <gf_type/compat.h>

/* Standard */
#include <time.h>

/**
 * @struct gf_network_state_connect_t
 * @~english
 * @brief Network connect state
 *
 * @var gf_network_state_connect_t::address
 * @brief List of address to try
 *
 * @var gf_network_state_connect_t::index
 * @brief Index for address list
 *
 * @var gf_network_state_connect_t::last
 * @brief Time when it issued connect
 */
GF_DECLARE_TYPE(network_state_connect, {
	gf_uint32_t* address;
	int index;
	time_t last;
});

/**
 * @union gf_network_state_t
 * @~english
 * @brief Network state union
 *
 * @var gf_network_state_t::connect
 * @brief Connect state
 */
GF_DECLARE_UNION(network_state, {
	gf_network_state_connect_t connect;
});

/**
 * @struct gf_network_t
 * @~english
 * @brief Network interface
 *
 * @var gf_network_t::engine
 * @brief Engine instance
 *
 * @var gf_network_t::port
 * @brief Port
 *
 * @var gf_network_t::sock
 * @brief Socket
 *
 * @var gf_network_t::state
 * @brief State
 *
 * @var gf_network_t::u
 * @brief Union
 */
GF_DECLARE_TYPE(network, {
	gf_engine_t* engine;
	int port;
	int sock;
	int state;
	gf_network_state_t u;
});
#else
typedef void gf_network_t;
#endif

#endif
