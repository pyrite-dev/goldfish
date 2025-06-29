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
 * @file gf_type/texture.h
 * @~english
 * @brief Type definitions related to texture
 */

#ifndef __GF_TYPE_TEXTURE_H__
#define __GF_TYPE_TEXTURE_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_TEXTURE
#define gf_texture_t struct gf_texture

gf_texture_t;

/* External library */

/* Engine */
#include <gf_type/draw_driver.h>

/* Standard */

/**
 * @struct gf_texture
 * @~english
 * @brief Texture
 *
 * @var gf_texture::draw_driver_texture
 * @brief Driver-dependent texture
 *
 * @var gf_texture::width
 * @brief Width of texture
 *
 * @var gf_texture::height
 * @brief Height of texture
 *
 * @var gf_texture::internal_width
 * @brief Internal width of texture
 *
 * @var gf_texture::internal_height
 * @brief Internal height of texture
 *
 * @var gf_texture::keep_aspect
 * @brief Keep aspect or not
 */
GF_DECLARE_TYPE(texture, {
	gf_draw_driver_texture_t* draw_driver_texture;
	int			  width;
	int			  height;
	int			  internal_width;
	int			  internal_height;
	int			  keep_aspect;
});
#else
typedef void gf_texture_t;
#endif

#endif
