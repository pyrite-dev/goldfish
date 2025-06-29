/**
 * @if SHOW_LICENSE
 **********************************************************************************
 *
 * Copyright (c) 2025 Pyrite Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. All advertising materials mentioning features or use of this software must
 *    display the following acknowledgement:
 *    This product includes software developed by the Pyrite Development Team.
 *
 * 4. Neither the name of Pyrite Development Team nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 **********************************************************************************
 * @endif
 */

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
