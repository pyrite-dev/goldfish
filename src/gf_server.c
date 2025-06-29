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
 * File Name:    gf_server.c
 * Author:       Nishi
 *
 * Functionality:
 * TODO: Write here
 *
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_SERVER

#include <gf_pre.h>

/* External library */

/* Interface */
#include <gf_server.h>

/* Engine */
#include <gf_physics.h>
#include <gf_log.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

void gf_server_begin(void) { gf_physics_begin(); }

void gf_server_end(void) {
#if !defined(GF_NO_SERVER)
	gf_physics_end();
#endif
}

gf_server_t* gf_server_create(gf_engine_t* engine) {
	gf_server_t* server = malloc(sizeof(*server));
	memset(server, 0, sizeof(*server));
	server->engine	= engine;
	server->physics = gf_physics_create(engine);
	if(server->physics == NULL) {
		gf_log_function(engine, "Failed to create physics interface", "");
		gf_server_destroy(server);
		return NULL;
	}
	return server;
}

void gf_server_destroy(gf_server_t* server) {
	gf_server_shutdown(server);
	if(server->physics != NULL) gf_physics_destroy(server->physics);
	gf_log_function(server->engine, "Destroyed server interface", "");
	free(server);
}

void gf_server_shutdown(gf_server_t* server) { gf_log_function(server->engine, "Server shutdown complete", ""); }
