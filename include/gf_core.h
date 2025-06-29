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
 * @file gf_core.h
 * @~english
 * @brief Core
 */

#ifndef __GF_CORE_H__
#define __GF_CORE_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/core.h>

/* Engine */
#include <gf_type/draw.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief "Powered by GoldFish" image
 */
GF_EXPORT unsigned char gf_powered[];

/**
 * @~english
 * @brief Size of "Powered by GoldFish" image
 */
GF_EXPORT unsigned int gf_powered_len;

/**
 * @~english
 * @brief Initialize engine
 */
GF_EXPORT void gf_engine_begin(void);

/**
 * @~english
 * @brief Cleanup engine
 */
GF_EXPORT void gf_engine_end(void);

/**
 * @~english
 * @brief Create engine instance, with default resource pack path (base.pak)
 * @param title Title to be shown on the window
 * @param nogui `0` to enable client-part, otherwise disable client-part
 * @deprecated Use gf_engine_create_ex instead
 * @return Engine instance
 */
GF_EXPORT gf_engine_t* gf_engine_create(const char* title, int nogui);

/**
 * @~english
 * @brief Create engine instance
 * @param title Title to be shown on the window
 * @param nogui `0` to enable client-part, otherwise disable client-part
 * @param param Parameter
 * @param argv Command line argument list
 * @param argc Command line argument count
 * @return Engine instance
 */
GF_EXPORT gf_engine_t* gf_engine_create_ex(const char* title, int nogui, gf_engine_param_t param, char** argv, int argc);

/**
 * @~english
 * @brief Start engine main loop
 * @param engine Engine instance
 * @note How engine draws a frame internally is explained in gf_core.c
 */
GF_EXPORT void gf_engine_loop(gf_engine_t* engine);

/**
 * @~english
 * @brief Destroy engine instance
 * @param engine Engine instance
 */
GF_EXPORT void gf_engine_destroy(gf_engine_t* engine);

/**
 * @~english
 * @brief Start engine shutdown
 * @param engine Engine instance
 */
GF_EXPORT void gf_engine_shutdown(gf_engine_t* engine);

/**
 * @~english
 * @brief Set game name
 * @param engine Engine instance
 * @param name Name
 */
GF_EXPORT void gf_engine_name(gf_engine_t* engine, const char* name);

/**
 * @~english
 * @brief Set game prefix
 * @param engine Engine instance
 * @param prefix Prefix
 */
GF_EXPORT void gf_engine_prefix(gf_engine_t* engine, const char* prefix);

#ifdef __cplusplus
}
#endif

#endif
