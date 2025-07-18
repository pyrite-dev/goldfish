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
 * @file gf_draw_platform.h
 * @~english
 * @brief Platform-dependent part of drawing driver
 * @warning You should not use functions written here, since engine does it for you
 */

#ifndef __GF_DRAW_PLATFORM_H__
#define __GF_DRAW_PLATFORM_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/draw_platform.h>

/* Engine */
#include <gf_type/draw.h>
#include <gf_type/core.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Create platform-dependent part of drawing driver
 * @param engine Engine instance
 * @param draw Drawing interface
 * @return Platform-dependent part of drawing driver
 */
GF_EXPORT gf_draw_platform_t* GFAPI gf_draw_platform_create(gf_engine_t* engine, gf_draw_t* draw);

/**
 * @~english
 * @brief Destroy platform-dependent part of drawing driver
 * @param platform Platform-dependent part of drawing driver
 */
GF_EXPORT void GFAPI gf_draw_platform_destroy(gf_draw_platform_t* platform);

/**
 * @~english
 * @brief Do platform-dependent step
 * @param draw Drawing interface
 * @return `0` if successful, otherwise if failed
 */
GF_EXPORT int GFAPI gf_draw_platform_step(gf_draw_t* draw);

/**
 * @~english
 * @brief Check if renderer has extension
 * @param draw Drawing interface
 * @param query Extension to be checked
 * @return `1` if extension is present, otherwise if it isn't
 */
GF_EXPORT int GFAPI gf_draw_platform_has_extension(gf_draw_t* draw, const char* query);

/**
 * @~english
 * @brief Initialize platform-dependent part of drawing driver
 */
GF_EXPORT void GFAPI gf_draw_platform_begin(void);

/**
 * @~english
 * @brief Cleanup platform-dependent part of drawing driver
 */
GF_EXPORT void GFAPI gf_draw_platform_end(void);

#ifdef __cplusplus
}
#endif

#endif
