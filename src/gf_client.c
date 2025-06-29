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
 * File Name:    gf_client.c
 * Author:       Nishi
 * 
 * Functionality:
 * TODO: Write here
 * 
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_CLIENT
#define GF_EXPOSE_DRAW

#include <gf_pre.h>

/* External library */

/* Interface */
#include <gf_client.h>

/* Engine */
#include <gf_draw.h>
#include <gf_log.h>
#include <gf_input.h>
#include <gf_audio.h>
#include <gf_font.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

void gf_client_begin(void) { gf_draw_begin(); }

void gf_client_end(void) { gf_draw_end(); }

gf_client_t* gf_client_create(gf_engine_t* engine, const char* title) {
	gf_client_t* client = malloc(sizeof(*client));
	memset(client, 0, sizeof(*client));
	client->engine = engine;
	client->draw   = gf_draw_create(engine, title);
	if(client->draw == NULL) {
		gf_log_function(engine, "Failed to create drawing interface", "");
		gf_client_destroy(client);
		return NULL;
	}
	client->audio = gf_audio_create(engine);
	if(client->audio == NULL) {
		gf_log_function(engine, "Failed to create audio interface", "");
		gf_client_destroy(client);
		return NULL;
	}
	client->input = gf_input_create(engine);
	gf_draw_set_input(client->draw, client->input);

	return client;
}

void gf_client_destroy(gf_client_t* client) {
	gf_client_shutdown(client);
	if(client->draw != NULL) gf_draw_destroy(client->draw);
	if(client->audio != NULL) gf_audio_destroy(client->audio);
	if(client->input != NULL) gf_input_destroy(client->input);
	gf_log_function(client->engine, "Destroyed client interface", "");
	free(client);
}

int gf_client_step(gf_client_t* client) {
	int s = gf_draw_step(client->draw);
	if(client->draw->close == 2) {
		client->draw->close = 3;
	}
	return s;
}

void gf_client_shutdown(gf_client_t* client) {
	/**
	 * If draw->close is less than 2: NOT gf_client_destroy
	 * If draw->close is           3:     gf_client_destroy
	 */
	if(client->draw->close < 2) {
		if(client->draw != NULL) {
			/**
			 * Only start shutdown of drawing interface
			 */
			client->draw->close = 2;
		}
	} else if(client->draw->close == 3) {
		gf_log_function(client->engine, "Client shutdown complete", "");
		client->draw->close = 4;
	}
}
