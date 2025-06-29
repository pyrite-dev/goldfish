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
 * @file gf_texture.h
 * @~english
 * @brief Texture
 */

#ifndef __GF_TEXTURE_H__
#define __GF_TEXTURE_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/texture.h>

/* Engine */
#include <gf_type/draw.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Create texture
 * @param draw Drawing interface
 * @param width Width of texture
 * @param height Height of texture
 * @param data Texture data
 * @return Texture
 */
GF_EXPORT gf_texture_t* gf_texture_create(gf_draw_t* draw, int width, int height, unsigned char* data);

/**
 * @~english
 * @brief Destroy texture
 * @param texture Texture
 */
GF_EXPORT void gf_texture_destroy(gf_texture_t* texture);

/**
 * @~english
 * @brief Keep aspect or not
 * @param texture Texture
 * @param keep `0` to not keep, otherwise keep
 */
GF_EXPORT void gf_texture_keep_aspect(gf_texture_t* texture, int keep);

#ifdef __cplusplus
}
#endif

#endif
