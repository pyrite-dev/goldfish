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
 * File Name:    bindgen_GoldFishGUIComponent.c
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
#include <gf_gui.h>
/******************/

/**
 * Symbols:
 *   gf_gui_destroy_id
 *   gf_gui_set_parent
 *   gf_gui_get_parent
 *   gf_gui_set_text
 *   gf_gui_get_text
 *   gf_gui_set_font_color
 *   gf_gui_get_font_color
 *   gf_gui_set_hover_font_color
 *   gf_gui_get_hover_font_color
 *   gf_gui_move_topmost
 */

/**
 * C: void gf_gui_destroy_id(gf_gui_t*, gf_gui_id_t)
 */
int bindgen_GoldFishGUIComponent_gf_gui_destroy_id(lua_State* s) {
	gf_gui_t*    param0;
	gf_gui_id_t* param1 = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	gf_lua_t*    wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	gf_gui_destroy_id(param0, *param1);

	return 0;
}

/**
 * C: void gf_gui_set_parent(gf_gui_t*, gf_gui_id_t, gf_gui_id_t)
 */
int bindgen_GoldFishGUIComponent_gf_gui_set_parent(lua_State* s) {
	gf_gui_t*    param0;
	gf_gui_id_t* param1 = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	gf_gui_id_t* param2 = luaL_checkudata(s, 2, "GoldFishGUIComponent");
	gf_lua_t*    wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	gf_gui_set_parent(param0, *param1, *param2);

	return 0;
}

/**
 * C: gf_gui_id_t gf_gui_get_parent(gf_gui_t*, gf_gui_id_t)
 */
int bindgen_GoldFishGUIComponent_gf_gui_get_parent(lua_State* s) {
	gf_gui_t*    param0;
	gf_gui_id_t* param1 = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	gf_lua_t*    wrap;
	gf_gui_id_t  ret;
	gf_gui_id_t* lret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	ret = gf_gui_get_parent(param0, *param1);
	if(!(ret >= 0)) return 0;
	lret = lua_newuserdata(s, sizeof(*lret));
	luaL_getmetatable(s, "GoldFishGUIComponent");
	lua_setmetatable(s, -2);
	*lret = ret;

	return 1;
}

/**
 * C: void gf_gui_set_text(gf_gui_t*, gf_gui_id_t, const char*)
 */
int bindgen_GoldFishGUIComponent_gf_gui_set_text(lua_State* s) {
	gf_gui_t*    param0;
	gf_gui_id_t* param1 = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	const char*  param2 = (const char*)luaL_checkstring(s, 2);
	gf_lua_t*    wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	gf_gui_set_text(param0, *param1, param2);

	return 0;
}

/**
 * C: char* gf_gui_get_text(gf_gui_t*, gf_gui_id_t)
 */
int bindgen_GoldFishGUIComponent_gf_gui_get_text(lua_State* s) {
	gf_gui_t*    param0;
	gf_gui_id_t* param1 = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	gf_lua_t*    wrap;
	char*	     ret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	ret = (char*)gf_gui_get_text(param0, *param1);
	lua_pushstring(s, ret);

	return 1;
}

/**
 * C: void gf_gui_set_font_color(gf_gui_t*, gf_gui_id_t, gf_graphic_color_t)
 */
int bindgen_GoldFishGUIComponent_gf_gui_set_font_color(lua_State* s) {
	gf_gui_t*	   param0;
	gf_gui_id_t*	   param1 = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	gf_graphic_color_t param2;
	gf_lua_t*	   wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;
	if(lua_gettop(s) >= 2) {
		lua_rawgeti(s, 2, 1);
		param2.r = luaL_checknumber(s, -1);
		lua_pop(s, 1);
		lua_rawgeti(s, 2, 2);
		param2.g = luaL_checknumber(s, -1);
		lua_pop(s, 1);
		lua_rawgeti(s, 2, 3);
		param2.b = luaL_checknumber(s, -1);
		lua_pop(s, 1);
		lua_rawgeti(s, 2, 4);
		param2.a = luaL_checknumber(s, -1);
		lua_pop(s, 1);
	} else {
		param2 = wrap->engine->client->draw->gui->font;
	}

	gf_gui_set_font_color(param0, *param1, param2);

	return 0;
}

/**
 * C: gf_graphic_color_t gf_gui_get_font_color(gf_gui_t*, gf_gui_id_t)
 */
int bindgen_GoldFishGUIComponent_gf_gui_get_font_color(lua_State* s) {
	gf_gui_t*	   param0;
	gf_gui_id_t*	   param1 = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	gf_lua_t*	   wrap;
	gf_graphic_color_t ret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	ret = gf_gui_get_font_color(param0, *param1);
	lua_newtable(s);
	lua_pushnumber(s, ret.r);
	lua_rawseti(s, -2, 1);
	lua_pushnumber(s, ret.g);
	lua_rawseti(s, -2, 2);
	lua_pushnumber(s, ret.b);
	lua_rawseti(s, -2, 3);
	lua_pushnumber(s, ret.a);
	lua_rawseti(s, -2, 4);

	return 1;
}

/**
 * C: void gf_gui_set_hover_font_color(gf_gui_t*, gf_gui_id_t, gf_graphic_color_t)
 */
int bindgen_GoldFishGUIComponent_gf_gui_set_hover_font_color(lua_State* s) {
	gf_gui_t*	   param0;
	gf_gui_id_t*	   param1 = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	gf_graphic_color_t param2;
	gf_lua_t*	   wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;
	if(lua_gettop(s) >= 2) {
		lua_rawgeti(s, 2, 1);
		param2.r = luaL_checknumber(s, -1);
		lua_pop(s, 1);
		lua_rawgeti(s, 2, 2);
		param2.g = luaL_checknumber(s, -1);
		lua_pop(s, 1);
		lua_rawgeti(s, 2, 3);
		param2.b = luaL_checknumber(s, -1);
		lua_pop(s, 1);
		lua_rawgeti(s, 2, 4);
		param2.a = luaL_checknumber(s, -1);
		lua_pop(s, 1);
	} else {
		param2 = wrap->engine->client->draw->gui->font;
	}

	gf_gui_set_hover_font_color(param0, *param1, param2);

	return 0;
}

/**
 * C: gf_graphic_color_t gf_gui_get_hover_font_color(gf_gui_t*, gf_gui_id_t)
 */
int bindgen_GoldFishGUIComponent_gf_gui_get_hover_font_color(lua_State* s) {
	gf_gui_t*	   param0;
	gf_gui_id_t*	   param1 = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	gf_lua_t*	   wrap;
	gf_graphic_color_t ret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	ret = gf_gui_get_hover_font_color(param0, *param1);
	lua_newtable(s);
	lua_pushnumber(s, ret.r);
	lua_rawseti(s, -2, 1);
	lua_pushnumber(s, ret.g);
	lua_rawseti(s, -2, 2);
	lua_pushnumber(s, ret.b);
	lua_rawseti(s, -2, 3);
	lua_pushnumber(s, ret.a);
	lua_rawseti(s, -2, 4);

	return 1;
}

/**
 * C: void gf_gui_move_topmost(gf_gui_t*, gf_gui_id_t)
 */
int bindgen_GoldFishGUIComponent_gf_gui_move_topmost(lua_State* s) {
	gf_gui_t*    param0;
	gf_gui_id_t* param1 = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	gf_lua_t*    wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	gf_gui_move_topmost(param0, *param1);

	return 0;
}

void bindgen_GoldFishGUIComponent_init(gf_lua_t* lua) {
	luaL_newmetatable(LUA(lua), "GoldFishGUIComponent");

	lua_pushstring(LUA(lua), "destroy");
	lua_pushcfunction(LUA(lua), bindgen_GoldFishGUIComponent_gf_gui_destroy_id);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "set_parent");
	lua_pushcfunction(LUA(lua), bindgen_GoldFishGUIComponent_gf_gui_set_parent);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "get_parent");
	lua_pushcfunction(LUA(lua), bindgen_GoldFishGUIComponent_gf_gui_get_parent);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "set_text");
	lua_pushcfunction(LUA(lua), bindgen_GoldFishGUIComponent_gf_gui_set_text);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "get_text");
	lua_pushcfunction(LUA(lua), bindgen_GoldFishGUIComponent_gf_gui_get_text);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "set_font_color");
	lua_pushcfunction(LUA(lua), bindgen_GoldFishGUIComponent_gf_gui_set_font_color);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "get_font_color");
	lua_pushcfunction(LUA(lua), bindgen_GoldFishGUIComponent_gf_gui_get_font_color);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "set_hover_font_color");
	lua_pushcfunction(LUA(lua), bindgen_GoldFishGUIComponent_gf_gui_set_hover_font_color);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "get_hover_font_color");
	lua_pushcfunction(LUA(lua), bindgen_GoldFishGUIComponent_gf_gui_get_hover_font_color);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "move_topmost");
	lua_pushcfunction(LUA(lua), bindgen_GoldFishGUIComponent_gf_gui_move_topmost);
	lua_settable(LUA(lua), -3);

	lua_pushvalue(LUA(lua), -1);
	lua_setfield(LUA(lua), -2, "__index");
}

/*** Generated at Mon Jun 9 2025 ***/
