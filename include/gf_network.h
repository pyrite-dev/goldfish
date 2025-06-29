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

/**
 * @~english
 * @brief Start secure TCP server
 * @param engine Engine instance
 * @param host Host
 * @param port Port
 * @return Network interface
 */
GF_EXPORT gf_network_t* gf_network_secure_tcp_server(gf_engine_t* engine, const char* host, int port);

/**
 * @~english
 * @brief Do network interface single step
 * @param net Network interface
 * @return `0` if successful
 */
GF_EXPORT int gf_network_secure_server_step(gf_network_t* net);

/**
 * @~english
 * @brief Destroy network interface
 * @param net Network interface
 */
GF_EXPORT void gf_network_destroy(gf_network_t* net);

#ifdef __cplusplus
}
#endif

#endif
