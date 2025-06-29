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
 * File Name:    bindgen_GoldFishAudio.c
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

/*** Dependency ***/
#include <gf_audio.h>
/******************/

/**
 * Symbols:
 *   gf_audio_auto_destroy
 *   gf_audio_pause
 *   gf_audio_resume
 *   gf_audio_stop
 *   gf_audio_is_over
 */

/**
 * C: void gf_audio_auto_destroy(gf_audio_t*, gf_audio_id_t)
 */
int bindgen_GoldFishAudio_gf_audio_auto_destroy(lua_State* s) {
	gf_audio_t*    param0;
	gf_audio_id_t* param1 = luaL_checkudata(s, 1, "GoldFishAudio");
	gf_lua_t*      wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->audio;

	gf_audio_auto_destroy(param0, *param1);

	return 0;
}

/**
 * C: void gf_audio_pause(gf_audio_t*, gf_audio_id_t)
 */
int bindgen_GoldFishAudio_gf_audio_pause(lua_State* s) {
	gf_audio_t*    param0;
	gf_audio_id_t* param1 = luaL_checkudata(s, 1, "GoldFishAudio");
	gf_lua_t*      wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->audio;

	gf_audio_pause(param0, *param1);

	return 0;
}

/**
 * C: void gf_audio_resume(gf_audio_t*, gf_audio_id_t)
 */
int bindgen_GoldFishAudio_gf_audio_resume(lua_State* s) {
	gf_audio_t*    param0;
	gf_audio_id_t* param1 = luaL_checkudata(s, 1, "GoldFishAudio");
	gf_lua_t*      wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->audio;

	gf_audio_resume(param0, *param1);

	return 0;
}

/**
 * C: void gf_audio_stop(gf_audio_t*, gf_audio_id_t)
 */
int bindgen_GoldFishAudio_gf_audio_stop(lua_State* s) {
	gf_audio_t*    param0;
	gf_audio_id_t* param1 = luaL_checkudata(s, 1, "GoldFishAudio");
	gf_lua_t*      wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->audio;

	gf_audio_stop(param0, *param1);

	return 0;
}

/**
 * C: int gf_audio_is_over(gf_audio_t*, gf_audio_id_t)
 */
int bindgen_GoldFishAudio_gf_audio_is_over(lua_State* s) {
	gf_audio_t*    param0;
	gf_audio_id_t* param1 = luaL_checkudata(s, 1, "GoldFishAudio");
	gf_lua_t*      wrap;
	int	       ret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->audio;

	ret = gf_audio_is_over(param0, *param1);
	lua_pushinteger(s, ret);

	return 1;
}

void bindgen_GoldFishAudio_init(gf_lua_t* lua) {
	luaL_newmetatable(LUA(lua), "GoldFishAudio");

	lua_pushstring(LUA(lua), "auto_destroy");
	lua_pushcfunction(LUA(lua), bindgen_GoldFishAudio_gf_audio_auto_destroy);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "pause");
	lua_pushcfunction(LUA(lua), bindgen_GoldFishAudio_gf_audio_pause);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "resume");
	lua_pushcfunction(LUA(lua), bindgen_GoldFishAudio_gf_audio_resume);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "stop");
	lua_pushcfunction(LUA(lua), bindgen_GoldFishAudio_gf_audio_stop);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "is_over");
	lua_pushcfunction(LUA(lua), bindgen_GoldFishAudio_gf_audio_is_over);
	lua_settable(LUA(lua), -3);

	lua_pushvalue(LUA(lua), -1);
	lua_setfield(LUA(lua), -2, "__index");
}

/*** Generated at Mon Jun 9 2025 ***/
