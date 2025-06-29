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
 * File Name:    bindgen.c
 * Author:       Nishi
 * 
 * Functionality:
 * TODO: Write here
 * 
 **********************************************************************************
 * @endif
 */

/**
 * Auto-generated using bindgen (https://github.com/pyrite-dev/bindgen)
 * Do not modify this directly! Might get overwritten.
 */

#include <bindgen.h>
#include <lua.h>
#include <lauxlib.h>

#ifndef LUA
#define LUA(wrap) (wrap->lua)
#endif

/*** <Pre> ***/

#ifndef GF_EXPOSE_ALL
#define GF_EXPOSE_ALL
#endif
#include <gf_type/lua.h>

/*** </Pre> ***/

void bindgen_init(gf_lua_t* lua) {
	lua_pushlightuserdata(LUA(lua), lua);
	lua_setglobal(LUA(lua), "_LUA_WRAP");
	bindgen_GoldFishGUIComponent_init(lua);
	bindgen_GoldFishAudio_init(lua);
	bindgen_GoldFishTexture_init(lua);
	bindgen_GoldFishFont_init(lua);

	lua_newtable(LUA(lua));
	bindgen_gui_init(lua);
	bindgen_audio_init(lua);
	bindgen_font_init(lua);
	bindgen_graphic_init(lua);
	lua_setglobal(LUA(lua), "goldfish");
}

/*** Generated at Mon Jun 9 2025 ***/
