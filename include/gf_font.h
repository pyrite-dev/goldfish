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
 * @file gf_font.h
 * @~english
 * @brief Font
 */

#ifndef __GF_FONT_H__
#define __GF_FONT_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/font.h>

/* Engine */
#include <gf_type/draw.h>

/* Standard */
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Get glyph
 * @param font Font
 * @param code Character code
 * @return Glyph
 */
GF_EXPORT gf_font_glyph_t* gf_font_get(gf_font_t* font, int code);

/**
 * @~english
 * @brief Render text
 * @param font Font
 * @param text Text
 * @param size Size
 * @param width Width
 * @param height Height
 * @return Texture
 */
GF_EXPORT gf_texture_t** gf_font_render(gf_font_t* font, const char* text, double size, double* width, double* height);

/**
 * @~english
 * @brief Load font
 * @param draw Drawing interface
 * @param path Path
 * @param data Data
 * @param size Data size
 * @return Font
 */
GF_EXPORT gf_font_t* gf_font_create(gf_draw_t* draw, const char* path, const void* data, size_t size);

/**
 * @~english
 * @brief Load font
 * @param draw Drawing interface
 * @param path Path
 * @return Font
 */
GF_EXPORT gf_font_t* gf_font_create_file(gf_draw_t* draw, const char* path);

/**
 * @~english
 * @brief Destroy font
 * @param font Font
 */
GF_EXPORT void gf_font_destroy(gf_font_t* font);

#ifdef __cplusplus
}
#endif

#endif
