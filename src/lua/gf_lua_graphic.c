#define GF_EXPOSE_LUA
#define GF_EXPOSE_CORE
#define GF_EXPOSE_CLIENT
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_GUI

#include <gf_pre.h>

/* External library */
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stb_ds.h>

/* Interface */
#include <gf_lua.h>

/* Engine */
#include <gf_prop.h>
#include <gf_file.h>
#include <gf_log.h>
#include <gf_gui.h>
#include <gf_font.h>
#include <gf_gui_component.h>
#include <gf_graphic.h>

/* Standard */
#include <stdlib.h>

int gf_lua_call_graphic_text(lua_State* s) {
	gf_font_t**	   font = luaL_checkudata(s, 1, "GoldFishFont");
	double		   x	= luaL_checknumber(s, 2);
	double		   y	= luaL_checknumber(s, 3);
	double		   sz	= luaL_checknumber(s, 4);
	const char*	   text = luaL_checkstring(s, 5);
	gf_lua_t*	   lua;
	gf_graphic_color_t col;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	if(lua_gettop(s) == 7) {
		lua_rawgeti(s, 6, 1);
		col.r = luaL_checknumber(s, -1);
		lua_pop(s, 1);

		lua_rawgeti(s, 6, 2);
		col.g = luaL_checknumber(s, -1);
		lua_pop(s, 1);

		lua_rawgeti(s, 6, 3);
		col.b = luaL_checknumber(s, -1);
		lua_pop(s, 1);

		lua_rawgeti(s, 6, 4);
		col.a = luaL_checknumber(s, -1);
		lua_pop(s, 1);
	} else {
		col = lua->engine->client->draw->gui->font;
	}

	gf_graphic_text(lua->engine->client->draw, *font, x, y, sz, text, col);

	return 0;
}

int gf_lua_call_graphic_text_width(lua_State* s) {
	gf_font_t** font = luaL_checkudata(s, 1, "GoldFishFont");
	double	    sz	 = luaL_checknumber(s, 2);
	const char* text = luaL_checkstring(s, 3);
	gf_lua_t*   lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	lua_pushnumber(s, gf_graphic_text_width(lua->engine->client->draw, *font, sz, text));

	return 1;
}

int gf_lua_call_graphic_text_height(lua_State* s) {
	gf_font_t** font = luaL_checkudata(s, 1, "GoldFishFont");
	double	    sz	 = luaL_checknumber(s, 2);
	const char* text = luaL_checkstring(s, 3);
	gf_lua_t*   lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	lua_pushnumber(s, gf_graphic_text_height(lua->engine->client->draw, *font, sz, text));

	return 1;
}

int gf_lua_call_graphic_rect(lua_State* s) {
	gf_lua_t*	   lua;
	gf_graphic_color_t col;
	double		   x;
	double		   y;
	double		   w;
	double		   h;

	x = luaL_checknumber(s, 1);
	y = luaL_checknumber(s, 2);
	w = luaL_checknumber(s, 3);
	h = luaL_checknumber(s, 4);

	lua_rawgeti(s, 5, 1);
	col.r = luaL_checknumber(s, -1);
	lua_pop(s, 1);

	lua_rawgeti(s, 5, 2);
	col.g = luaL_checknumber(s, -1);
	lua_pop(s, 1);

	lua_rawgeti(s, 5, 3);
	col.b = luaL_checknumber(s, -1);
	lua_pop(s, 1);

	lua_rawgeti(s, 5, 4);
	col.a = luaL_checknumber(s, -1);
	lua_pop(s, 1);

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	gf_graphic_fill_rect(lua->engine->client->draw, x, y, w, h, col);

	return 0;
}

int gf_lua_call_graphic_points(lua_State* s) {
	int		   i;
	int		   len;
	int		   dim;
	int		   plen;
	double*		   arr;
	gf_lua_t*	   lua;
	gf_graphic_color_t col;

	lua_rawgeti(s, 1, 1);
	col.r = luaL_checknumber(s, -1);
	lua_pop(s, 1);

	lua_rawgeti(s, 1, 2);
	col.g = luaL_checknumber(s, -1);
	lua_pop(s, 1);

	lua_rawgeti(s, 1, 3);
	col.b = luaL_checknumber(s, -1);
	lua_pop(s, 1);

	lua_rawgeti(s, 1, 4);
	col.a = luaL_checknumber(s, -1);
	lua_pop(s, 1);

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	dim  = luaL_checknumber(s, 2);
	plen = (dim == GF_GRAPHIC_2D ? 2 : 3);

	len = lua_rawlen(s, 3);

	arr = malloc(sizeof(*arr) * plen * len);
	for(i = 1; i <= len; i++) {
		double x;
		double y;
		double z;

		lua_rawgeti(s, 3, i);
		lua_rawgeti(s, -1, 1);
		x = luaL_checknumber(s, -1);
		lua_pop(s, 1);

		lua_rawgeti(s, -1, 2);
		y = luaL_checknumber(s, -1);
		lua_pop(s, 1);

		if(dim == GF_GRAPHIC_3D) {
			lua_rawgeti(s, -1, 3);
			z = luaL_checknumber(s, -1);
			lua_pop(s, 1);
			lua_pop(s, 1);

			arr[plen * (i - 1) + 0] = x;
			arr[plen * (i - 1) + 1] = y;
			arr[plen * (i - 1) + 2] = z;
		} else {
			lua_pop(s, 1);

			arr[plen * (i - 1) + 0] = x;
			arr[plen * (i - 1) + 1] = y;
		}
	}

	gf_graphic_points_arr(lua->engine->client->draw, col, dim, len, arr);
	free(arr);

	return 0;
}

void gf_lua_create_goldfish_graphic(gf_lua_t* lua) {
	lua_pushstring(lua->lua, "graphic");
	lua_newtable(lua->lua);

	lua_pushstring(lua->lua, "text");
	lua_pushcfunction(lua->lua, gf_lua_call_graphic_text);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "text_width");
	lua_pushcfunction(lua->lua, gf_lua_call_graphic_text_width);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "text_height");
	lua_pushcfunction(lua->lua, gf_lua_call_graphic_text_height);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "points");
	lua_pushcfunction(lua->lua, gf_lua_call_graphic_points);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "rect");
	lua_pushcfunction(lua->lua, gf_lua_call_graphic_rect);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "DIM_2D");
	lua_pushinteger(lua->lua, GF_GRAPHIC_2D);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "DIM_3D");
	lua_pushinteger(lua->lua, GF_GRAPHIC_3D);
	lua_settable(lua->lua, -3);

	lua_settable(lua->lua, -3);
}
