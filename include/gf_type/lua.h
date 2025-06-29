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
 * @file gf_type/lua.h
 * @~english
 * @brief Type definitions related to lua
 */

#ifndef __GF_TYPE_LUA_H__
#define __GF_TYPE_LUA_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_LUA
#define gf_lua_t struct gf_lua

gf_lua_t;

/* External library */
#include <lua.h>

/* Engine */
#include <gf_type/core.h>
#include <gf_type/gui.h>

/* Standard */

/**
 * @struct gf_lua
 * @~english
 * @brief Lua interface
 *
 * @var gf_lua::engine
 * @brief Engine instance
 *
 * @var gf_lua::lua
 * @brief Lua
 *
 * @var gf_lua::loop
 * @brief Loop call
 *
 * @var gf_lua::close
 * @brief Close call
 */
GF_DECLARE_TYPE(lua, {
	gf_engine_t* engine;
	lua_State*   lua;
	int	     loop;
	int	     close;
});
#else
typedef void gf_lua_t;
#endif

#endif
