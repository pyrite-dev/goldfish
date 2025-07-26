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
 * File Name:    gf_network.c
 * Author:       Nishi
 *
 * Functionality:
 * Handle GoldFish Secure Layer.
 *
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_NETWORK

#include <gf_pre.h>

/* External library */
#include <gf_netdrv.h>
#include <stb_ds.h>

/* Interface */
#include <gf_network.h>

/* Engine */
#include <gf_type/compat.h>
#include <gf_log.h>
#include <gf_util.h>

/* Standard */
#include <stdlib.h>
#include <string.h>
#include <time.h>

gf_network_t* gf_network_secure_tcp(gf_engine_t* engine, const char* host, int port) {
	gf_network_t* net = malloc(sizeof(*net));
	memset(net, 0, sizeof(*net));
	net->engine = engine;
	gf_log_function(engine, "Connecting to %s:%d", host, port);
	return net;
}

gf_network_t* gf_network_secure_tcp_server(gf_engine_t* engine, const char* host, int port) {
	gf_network_t* net = malloc(sizeof(*net));
	memset(net, 0, sizeof(*net));
	net->engine = engine;

	return net;
}

void gf_network_destroy(gf_network_t* net) {
	gf_log_function(net->engine, "Destroyed network interface", "");
	free(net);
}
