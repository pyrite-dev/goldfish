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
#include <string.h>
#include <stdio.h>

gf_gui_id_t* gf_lua_create_gui(gf_lua_t* lua) {
	gf_gui_id_t* id = lua_newuserdata(lua->lua, sizeof(*id));
	luaL_getmetatable(lua->lua, "GoldFishGUIComponent");
	lua_setmetatable(lua->lua, -2);
	return id;
}

void gf_lua_gui_callback(gf_engine_t* engine, gf_draw_t* draw, gf_gui_id_t id, int type) {
	int*	     call = gf_prop_get_ptr(gf_gui_get_prop(draw->gui, id), "luacall");
	gf_lua_t*    lua;
	gf_gui_id_t* pid;
	if(call == NULL) return;

	lua = *(gf_lua_t**)gf_prop_get_ptr(gf_gui_get_prop(draw->gui, id), "lua");

	lua_rawgeti(lua->lua, LUA_REGISTRYINDEX, *call);

	pid  = gf_lua_create_gui(lua);
	*pid = id;

	lua_pushnumber(lua->lua, type);
	lua_pcall(lua->lua, 2, 0, 0);
}

int gf_lua_call_gui_create(lua_State* s) {
	const char* str = luaL_checkstring(s, 1);
	double	    x	= luaL_checknumber(s, 2);
	double	    y	= luaL_checknumber(s, 3);
	double	    w	= luaL_checknumber(s, 4);
	double	    h	= luaL_checknumber(s, 5);
	gf_lua_t*   lua;
	int	    i;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	for(i = 0; i < sizeof(gf_gui_calls) / sizeof(gf_gui_calls[0]); i++) {
		if(strcmp(gf_gui_calls[i].name, str) == 0) {
			void**	     plua = malloc(sizeof(*plua));
			gf_gui_id_t* id	  = gf_lua_create_gui(lua);

			*plua = lua;

			*id = gf_gui_calls[i].call(lua->engine->client->draw->gui, x, y, w, h);

			gf_gui_set_callback(lua->engine->client->draw->gui, *id, gf_lua_gui_callback);

			gf_prop_set_ptr(gf_gui_get_prop(lua->engine->client->draw->gui, *id), "lua", (void*)plua);

			return 1;
		}
	}
	return 0;
}

int gf_lua_call_gui_sort(lua_State* s) {
	gf_lua_t* lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	gf_gui_sort_component(lua->engine->client->draw->gui);

	return 0;
}

int gf_lua_call_gui_color(lua_State* s) {
	const char*	   type = luaL_checkstring(s, 1);
	gf_graphic_color_t color;
	gf_lua_t*	   lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	if(lua_gettop(s) == 2) {
		if(strcmp(type, "base") == 0) {
			color = lua->engine->client->draw->gui->base;
		} else if(strcmp(type, "font") == 0) {
			color = lua->engine->client->draw->gui->font;
		}

		lua_newtable(s);

		lua_pushnumber(s, color.r);
		lua_rawseti(s, -2, 1);

		lua_pushnumber(s, color.g);
		lua_rawseti(s, -2, 2);

		lua_pushnumber(s, color.b);
		lua_rawseti(s, -2, 3);

		lua_pushnumber(s, color.a);
		lua_rawseti(s, -2, 4);

		return 1;
	} else {
		lua_rawgeti(s, 2, 1);
		color.r = luaL_checknumber(s, -1);
		lua_pop(s, 1);

		lua_rawgeti(s, 2, 2);
		color.g = luaL_checknumber(s, -1);
		lua_pop(s, 1);

		lua_rawgeti(s, 2, 3);
		color.b = luaL_checknumber(s, -1);
		lua_pop(s, 1);

		lua_rawgeti(s, 2, 4);
		color.a = luaL_checknumber(s, -1);
		lua_pop(s, 1);

		if(strcmp(type, "base") == 0) {
			lua->engine->client->draw->gui->base = color;
		} else if(strcmp(type, "font") == 0) {
			lua->engine->client->draw->gui->font = color;
		}
		return 0;
	}
}

void gf_lua_create_goldfish_gui(gf_lua_t* lua) {
	lua_pushstring(lua->lua, "gui");
	lua_newtable(lua->lua);

	lua_pushstring(lua->lua, "create");
	lua_pushcfunction(lua->lua, gf_lua_call_gui_create);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "sort");
	lua_pushcfunction(lua->lua, gf_lua_call_gui_sort);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "color");
	lua_pushcfunction(lua->lua, gf_lua_call_gui_color);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "PRESS_EVENT");
	lua_pushnumber(lua->lua, GF_GUI_PRESS_EVENT);
	lua_settable(lua->lua, -3);

	lua_settable(lua->lua, -3);
}
