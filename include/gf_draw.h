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
 * @file gf_draw.h
 * @~english
 * @brief Drawing interface
 * @warning You should not use functions written here, since engine does it for you
 */

#ifndef __GF_DRAW_H__
#define __GF_DRAW_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/draw.h>

/* Engine */
#include <gf_type/core.h>
#include <gf_type/input.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Create drawing interface
 * @param engine Engine instance
 * @param title Window title
 * @return Drawing interface
 */
GF_EXPORT gf_draw_t* gf_draw_create(gf_engine_t* engine, const char* title);

/**
 * @~english
 * @brief Destroy drawing interface
 * @param draw Drawing interface
 */
GF_EXPORT void gf_draw_destroy(gf_draw_t* draw);

/**
 * @~english
 * @brief Set input interface
 * @param draw Drawing interface
 * @param input Input interface
 */
GF_EXPORT void gf_draw_set_input(gf_draw_t* draw, gf_input_t* input);

/**
 * @~english
 * @brief Draw frame (common part)
 * @param draw Drawing interface
 */
GF_EXPORT void gf_draw_frame(gf_draw_t* draw);

/**
 * @~english
 * @brief Do drawing single step
 * @param draw Drawing interface
 * @return `0` if successful, otherwise if failed
 */
GF_EXPORT int gf_draw_step(gf_draw_t* draw);

/**
 * @~english
 * @brief Initialize drawing stuffs
 */
GF_EXPORT void gf_draw_begin(void);

/**
 * @~english
 * @brief Cleanup drawing stuffs
 */
GF_EXPORT void gf_draw_end(void);

/**
 * @~english
 * @brief Do stuffs required on resizing window
 * @param draw Drawing interface
 */
GF_EXPORT void gf_draw_reshape(gf_draw_t* draw);

/**
 * @~english
 * @brief Get FPS
 * @param draw Drawing interface
 */
GF_EXPORT double gf_draw_get_fps(gf_draw_t* draw);

/**
 * @~english
 * @brief Set callback
 * @param draw Drawing interface
 * @param callback Callback
 */
GF_EXPORT void gf_draw_set_callback(gf_draw_t* draw, gf_draw_callback_t callback);

#ifdef __cplusplus
}
#endif

#endif
