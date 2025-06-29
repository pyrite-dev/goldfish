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
 * File Name:    gf_texture.c
 * Author:       Nishi
 *
 * Functionality:
 * Wrap driver-dependent texture code.
 *
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_TEXTURE
#define GF_EXPOSE_DRAW_DRIVER

#include <gf_pre.h>

/* External library */

/* Interface */
#include <gf_texture.h>

/* Engine */
#include <gf_draw_driver.h>

/* Standard */
#include <stdlib.h>

gf_texture_t* gf_texture_create(gf_draw_t* draw, int width, int height, unsigned char* data) {
	gf_texture_t*		  texture = malloc(sizeof(*texture));
	gf_draw_driver_texture_t* ddtexture;
	texture->internal_width	 = width;
	texture->internal_height = height;
	ddtexture		 = gf_draw_driver_register_texture(draw, width, height, &texture->internal_width, &texture->internal_height, data);
	if(ddtexture == NULL) {
		free(texture);
		return NULL;
	}
	texture->width		     = width;
	texture->height		     = height;
	texture->draw_driver_texture = ddtexture;
	texture->keep_aspect	     = 0;
	return texture;
}

void gf_texture_destroy(gf_texture_t* texture) {
	gf_draw_driver_destroy_texture(texture->draw_driver_texture);
	free(texture);
}

void gf_texture_keep_aspect(gf_texture_t* texture, int keep) { texture->keep_aspect = keep; }
