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
 * @file gf_lua.h
 * @~english
 * @brief Lua
 */

#ifndef __GF_LUA_H__
#define __GF_LUA_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/lua.h>

/* Engine */
#include <gf_type/core.h>
#include <gf_type/gui.h>
#include <gf_type/font.h>
#include <gf_type/audio.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Create Lua interface
 * @param engine Engine instance
 * @return Lua interface
 */
GF_EXPORT gf_lua_t* gf_lua_create(gf_engine_t* engine);

/**
 * @~english
 * @brief Run Lua program
 * @param lua Lua interface
 * @param path Path
 * @return `0` if successful, otherwise `-1`
 */
GF_EXPORT int gf_lua_run(gf_lua_t* lua, const char* path);

/**
 * @~english
 * @brief Destroy Lua interface
 * @param lua Lua interface
 */
GF_EXPORT void gf_lua_destroy(gf_lua_t* lua);

/**
 * @~english
 * @brief Run a single step for Lua interface
 * @param lua Lua interface
 */
GF_EXPORT void gf_lua_step(gf_lua_t* lua);

/**
 * @~english
 * @brief Run close hook for Lua interface
 * @param lua Lua interface
 */
GF_EXPORT void gf_lua_close(gf_lua_t* lua);

#ifdef __cplusplus
}
#endif

#endif
