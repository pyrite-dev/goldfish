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
#include <gf_graphic.h>
/******************/

/**
 * Symbols:
 *   gf_graphic_clear
 *   gf_graphic_clip_push
 *   gf_graphic_clip_pop
 *   gf_graphic_text_width
 *   gf_graphic_text_height
 *   gf_graphic_text
 *   gf_graphic_text_wrap
 *   gf_graphic_fill_rect
 */

/**
 * C: void gf_graphic_clear(gf_draw_t*)
 */
int bindgen_graphic_gf_graphic_clear(lua_State* s) {
	gf_draw_t* param0;
	gf_lua_t*  wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw;

	gf_graphic_clear(param0);

	return 0;
}

/**
 * C: void gf_graphic_clip_push(gf_draw_t*, double, double, double, double)
 */
int bindgen_graphic_gf_graphic_clip_push(lua_State* s) {
	gf_draw_t* param0;
	double	   param1 = (double)luaL_checknumber(s, 1);
	double	   param2 = (double)luaL_checknumber(s, 2);
	double	   param3 = (double)luaL_checknumber(s, 3);
	double	   param4 = (double)luaL_checknumber(s, 4);
	gf_lua_t*  wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw;

	gf_graphic_clip_push(param0, param1, param2, param3, param4);

	return 0;
}

/**
 * C: void gf_graphic_clip_pop(gf_draw_t*)
 */
int bindgen_graphic_gf_graphic_clip_pop(lua_State* s) {
	gf_draw_t* param0;
	gf_lua_t*  wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw;

	gf_graphic_clip_pop(param0);

	return 0;
}

/**
 * C: double gf_graphic_text_width(gf_draw_t*, gf_font_t*, double, const char*)
 */
int bindgen_graphic_gf_graphic_text_width(lua_State* s) {
	gf_draw_t*  param0;
	gf_font_t** param1 = luaL_checkudata(s, 1, "GoldFishFont");
	double	    param2 = (double)luaL_checknumber(s, 2);
	const char* param3 = (const char*)luaL_checkstring(s, 3);
	gf_lua_t*   wrap;
	double	    ret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw;

	ret = gf_graphic_text_width(param0, *param1, param2, param3);
	lua_pushnumber(s, ret);

	return 1;
}

/**
 * C: double gf_graphic_text_height(gf_draw_t*, gf_font_t*, double, const char*)
 */
int bindgen_graphic_gf_graphic_text_height(lua_State* s) {
	gf_draw_t*  param0;
	gf_font_t** param1 = luaL_checkudata(s, 1, "GoldFishFont");
	double	    param2 = (double)luaL_checknumber(s, 2);
	const char* param3 = (const char*)luaL_checkstring(s, 3);
	gf_lua_t*   wrap;
	double	    ret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw;

	ret = gf_graphic_text_height(param0, *param1, param2, param3);
	lua_pushnumber(s, ret);

	return 1;
}

/**
 * C: void gf_graphic_text(gf_draw_t*, gf_font_t*, double, double, double, const char*, gf_graphic_color_t)
 */
int bindgen_graphic_gf_graphic_text(lua_State* s) {
	gf_draw_t*	   param0;
	gf_font_t**	   param1 = luaL_checkudata(s, 1, "GoldFishFont");
	double		   param2 = (double)luaL_checknumber(s, 2);
	double		   param3 = (double)luaL_checknumber(s, 3);
	double		   param4 = (double)luaL_checknumber(s, 4);
	const char*	   param5 = (const char*)luaL_checkstring(s, 5);
	gf_graphic_color_t param6;
	gf_lua_t*	   wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw;
	if(lua_gettop(s) >= 6) {
		lua_rawgeti(s, 6, 1);
		param6.r = luaL_checknumber(s, -1);
		lua_pop(s, 1);
		lua_rawgeti(s, 6, 2);
		param6.g = luaL_checknumber(s, -1);
		lua_pop(s, 1);
		lua_rawgeti(s, 6, 3);
		param6.b = luaL_checknumber(s, -1);
		lua_pop(s, 1);
		lua_rawgeti(s, 6, 4);
		param6.a = luaL_checknumber(s, -1);
		lua_pop(s, 1);
	} else {
		param6 = wrap->engine->client->draw->gui->font;
	}

	gf_graphic_text(param0, *param1, param2, param3, param4, param5, param6);

	return 0;
}

/**
 * C: double gf_graphic_text_wrap(gf_draw_t*, gf_font_t*, double, double, double, double, const char*, gf_graphic_color_t)
 */
int bindgen_graphic_gf_graphic_text_wrap(lua_State* s) {
	gf_draw_t*	   param0;
	gf_font_t**	   param1 = luaL_checkudata(s, 1, "GoldFishFont");
	double		   param2 = (double)luaL_checknumber(s, 2);
	double		   param3 = (double)luaL_checknumber(s, 3);
	double		   param4 = (double)luaL_checknumber(s, 4);
	double		   param5 = (double)luaL_checknumber(s, 5);
	const char*	   param6 = (const char*)luaL_checkstring(s, 6);
	gf_graphic_color_t param7;
	gf_lua_t*	   wrap;
	double		   ret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw;
	if(lua_gettop(s) >= 7) {
		lua_rawgeti(s, 7, 1);
		param7.r = luaL_checknumber(s, -1);
		lua_pop(s, 1);
		lua_rawgeti(s, 7, 2);
		param7.g = luaL_checknumber(s, -1);
		lua_pop(s, 1);
		lua_rawgeti(s, 7, 3);
		param7.b = luaL_checknumber(s, -1);
		lua_pop(s, 1);
		lua_rawgeti(s, 7, 4);
		param7.a = luaL_checknumber(s, -1);
		lua_pop(s, 1);
	} else {
		param7 = wrap->engine->client->draw->gui->font;
	}

	ret = gf_graphic_text_wrap(param0, *param1, param2, param3, param4, param5, param6, param7);
	lua_pushnumber(s, ret);

	return 1;
}

/**
 * C: void gf_graphic_fill_rect(gf_draw_t*, double, double, double, double, gf_graphic_color_t)
 */
int bindgen_graphic_gf_graphic_fill_rect(lua_State* s) {
	gf_draw_t*	   param0;
	double		   param1 = (double)luaL_checknumber(s, 1);
	double		   param2 = (double)luaL_checknumber(s, 2);
	double		   param3 = (double)luaL_checknumber(s, 3);
	double		   param4 = (double)luaL_checknumber(s, 4);
	gf_graphic_color_t param5;
	gf_lua_t*	   wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw;
	if(lua_gettop(s) >= 5) {
		lua_rawgeti(s, 5, 1);
		param5.r = luaL_checknumber(s, -1);
		lua_pop(s, 1);
		lua_rawgeti(s, 5, 2);
		param5.g = luaL_checknumber(s, -1);
		lua_pop(s, 1);
		lua_rawgeti(s, 5, 3);
		param5.b = luaL_checknumber(s, -1);
		lua_pop(s, 1);
		lua_rawgeti(s, 5, 4);
		param5.a = luaL_checknumber(s, -1);
		lua_pop(s, 1);
	} else {
		param5 = wrap->engine->client->draw->gui->font;
	}

	gf_graphic_fill_rect(param0, param1, param2, param3, param4, param5);

	return 0;
}

void bindgen_graphic_init(gf_lua_t* lua) {
	lua_pushstring(LUA(lua), "graphic");
	lua_newtable(LUA(lua));

	lua_pushstring(LUA(lua), "clear");
	lua_pushcfunction(LUA(lua), bindgen_graphic_gf_graphic_clear);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "clip_push");
	lua_pushcfunction(LUA(lua), bindgen_graphic_gf_graphic_clip_push);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "clip_pop");
	lua_pushcfunction(LUA(lua), bindgen_graphic_gf_graphic_clip_pop);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "text_width");
	lua_pushcfunction(LUA(lua), bindgen_graphic_gf_graphic_text_width);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "text_height");
	lua_pushcfunction(LUA(lua), bindgen_graphic_gf_graphic_text_height);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "text");
	lua_pushcfunction(LUA(lua), bindgen_graphic_gf_graphic_text);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "text_wrap");
	lua_pushcfunction(LUA(lua), bindgen_graphic_gf_graphic_text_wrap);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "fill_rect");
	lua_pushcfunction(LUA(lua), bindgen_graphic_gf_graphic_fill_rect);
	lua_settable(LUA(lua), -3);

	lua_settable(LUA(lua), -3);
}

/*** Generated at Thu May 8 2025 ***/
