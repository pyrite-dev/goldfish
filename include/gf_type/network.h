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
#define gf_network_t struct gf_network

gf_network_t;

/* External library */
#include <minisocket.h>
#include <compact25519.h>
#include <aes.h>

/* Engine */
#include <gf_type/core.h>
#include <gf_type/compat.h>

/* Standard */

/**
 * @struct gf_network
 * @~english
 * @brief Network interface
 *
 * @var gf_network::engine
 * @brief Engine instance
 *
 * @var gf_network::net
 * @brief Minisocket interface
 *
 * @var gf_network::fd
 * @brief Socket FD
 *
 * @var gf_network::state
 * @brief Internal state
 *
 * @var gf_network::aes
 * @brief AES context
 *
 * @var gf_network::shared_secret
 * @brief Shared secret
 *
 * @var gf_network::public_key
 * @brief Public key
 *
 * @var gf_network::private_key
 * @brief Private key
 *
 * @var gf_network::random_seed
 * @brief Random seed
 *
 * @var gf_network::clients
 * @brief Connected clients
 *
 * @var gf_network::connected
 * @brief Connected or not
 */
GF_DECLARE_TYPE(network, {
	gf_engine_t*	engine;
	ms_interface_t* net;
	int		fd;
	void*		state;
	struct AES_ctx	aes;
	gf_uint8_t	shared_secret[X25519_SHARED_SIZE];
	gf_uint8_t	private_key[X25519_KEY_SIZE];
	gf_uint8_t	public_key[X25519_KEY_SIZE];
	gf_uint8_t	random_seed[X25519_KEY_SIZE];
	gf_network_t**	clients;
	int		connected;
});
#else
typedef void gf_network_t;
#endif

#endif
