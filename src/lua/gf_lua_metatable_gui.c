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

int gf_lua_meta_call_gui_component_text(lua_State* s) {
	gf_gui_id_t* id	 = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	const char*  str = luaL_checkstring(s, 2);
	gf_lua_t*    lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	gf_gui_set_text(lua->engine->client->draw->gui, *id, str);
	return 0;
}

int gf_lua_meta_call_gui_component_destroy(lua_State* s) {
	gf_gui_id_t* id = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	gf_lua_t*    lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	gf_gui_destroy_id(lua->engine->client->draw->gui, *id);
	return 0;
}

int gf_lua_meta_call_gui_component_prop(lua_State* s) {
	gf_gui_id_t* id	  = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	const char*  type = luaL_checkstring(s, 2);
	const char*  str  = luaL_checkstring(s, 3);
	gf_lua_t*    lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	if(lua_gettop(s) == 5) {
		if(strcmp(type, "id") == 0) {
			gf_gui_id_t* t = luaL_checkudata(s, 4, "GoldFishGUIComponent");
			gf_gui_set_prop_id(lua->engine->client->draw->gui, *id, str, *t);
		} else if(strcmp(type, "integer") == 0) {
			lua_Integer t = luaL_checkinteger(s, 4);
			gf_prop_set_integer(gf_gui_get_prop(lua->engine->client->draw->gui, *id), str, t);
		} else if(strcmp(type, "floating") == 0) {
			lua_Number t = luaL_checknumber(s, 4);
			gf_prop_set_floating(gf_gui_get_prop(lua->engine->client->draw->gui, *id), str, t);
		}
		return 0;
	} else {
		if(strcmp(type, "id") == 0) {
			gf_gui_id_t* r = gf_lua_create_gui(lua);
			*r	       = gf_gui_get_prop_id(lua->engine->client->draw->gui, *id, str);
		} else if(strcmp(type, "integer") == 0) {
			lua_pushinteger(s, gf_prop_get_integer(gf_gui_get_prop(lua->engine->client->draw->gui, *id), str));
		} else if(strcmp(type, "floating") == 0) {
			lua_pushnumber(s, gf_prop_get_floating(gf_gui_get_prop(lua->engine->client->draw->gui, *id), str));
		}
	}
	return 1;
}

int gf_lua_meta_call_gui_component_callback(lua_State* s) {
	gf_gui_id_t* id = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	gf_lua_t*    lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	if(lua_gettop(s) == 3) {
		int* ptr = malloc(sizeof(*ptr));
		lua_pop(s, 1);
		*ptr = luaL_ref(s, LUA_REGISTRYINDEX);
		gf_prop_set_ptr(gf_gui_get_prop(lua->engine->client->draw->gui, *id), "luacall", (void*)ptr);
	} else {
		gf_prop_delete(gf_gui_get_prop(lua->engine->client->draw->gui, *id), "luacall");
	}
	return 0;
}

int gf_lua_meta_call_gui_component_font(lua_State* s) {
	gf_gui_id_t* id = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	gf_lua_t*    lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	if(lua_gettop(s) == 3) {
		gf_font_t** font = luaL_checkudata(s, 2, "GoldFishFont");
		gf_prop_set_ptr_keep(gf_gui_get_prop(lua->engine->client->draw->gui, *id), "font", *font);
	} else {
		gf_font_t*  pfont = gf_prop_get_ptr_keep(gf_gui_get_prop(lua->engine->client->draw->gui, *id), "font");
		gf_font_t** font  = gf_lua_create_font(lua);

		*font = pfont;
		return 1;
	}
	return 0;
}

int gf_lua_meta_call_gui_component_color(lua_State* s) {
	gf_gui_id_t*	   id	= luaL_checkudata(s, 1, "GoldFishGUIComponent");
	const char*	   name = luaL_checkstring(s, 2);
	gf_lua_t*	   lua;
	gf_graphic_color_t color;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	if(lua_gettop(s) == 4) {
		lua_rawgeti(s, 3, 1);
		color.r = lua_tonumber(s, -1);
		lua_pop(s, 1);

		lua_rawgeti(s, 3, 2);
		color.g = lua_tonumber(s, -1);
		lua_pop(s, 1);

		lua_rawgeti(s, 3, 3);
		color.b = lua_tonumber(s, -1);
		lua_pop(s, 1);

		lua_rawgeti(s, 3, 4);
		color.a = lua_tonumber(s, -1);
		lua_pop(s, 1);

		if(strcmp(name, "font") == 0) {
			gf_gui_set_font_color(lua->engine->client->draw->gui, *id, color);
		} else if(strcmp(name, "hover-font") == 0) {
			gf_gui_set_hover_font_color(lua->engine->client->draw->gui, *id, color);
		}
	} else {
		if(strcmp(name, "font") == 0) {
			color = gf_gui_get_font_color(lua->engine->client->draw->gui, *id);
		} else if(strcmp(name, "hover-font") == 0) {
			color = gf_gui_get_hover_font_color(lua->engine->client->draw->gui, *id);
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
	}
	return 0;
}

int gf_lua_meta_call_gui_component_parent(lua_State* s) {
	gf_gui_id_t* id = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	gf_gui_id_t* parent;
	gf_lua_t*    lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	if(lua_gettop(s) == 3) {
		parent = luaL_checkudata(s, 2, "GoldFishGUIComponent");
		gf_gui_set_parent(lua->engine->client->draw->gui, *id, *parent);
		return 0;
	} else {
		parent	= gf_lua_create_gui(lua);
		*parent = gf_gui_get_parent(lua->engine->client->draw->gui, *id);
		return 1;
	}
}

void gf_lua_meta_init_gui(gf_lua_t* lua) {
	luaL_newmetatable(lua->lua, "GoldFishGUIComponent");

	lua_pushstring(lua->lua, "text");
	lua_pushcfunction(lua->lua, gf_lua_meta_call_gui_component_text);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "parent");
	lua_pushcfunction(lua->lua, gf_lua_meta_call_gui_component_parent);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "destroy");
	lua_pushcfunction(lua->lua, gf_lua_meta_call_gui_component_destroy);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "font");
	lua_pushcfunction(lua->lua, gf_lua_meta_call_gui_component_font);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "color");
	lua_pushcfunction(lua->lua, gf_lua_meta_call_gui_component_color);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "prop");
	lua_pushcfunction(lua->lua, gf_lua_meta_call_gui_component_prop);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "callback");
	lua_pushcfunction(lua->lua, gf_lua_meta_call_gui_component_callback);
	lua_settable(lua->lua, -3);

	lua_pushvalue(lua->lua, -1);
	lua_setfield(lua->lua, -2, "__index");
}
