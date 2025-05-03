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

gf_font_t** gf_lua_create_font(gf_lua_t* lua) {
	gf_font_t** font = lua_newuserdata(lua->lua, sizeof(*font));
	luaL_getmetatable(lua->lua, "GoldFishFont");
	lua_setmetatable(lua->lua, -2);
	return font;
}

int gf_lua_call_font_load(lua_State* s) {
	const char* path = luaL_checkstring(s, 1);
	gf_font_t** font;
	gf_lua_t*   lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	font  = gf_lua_create_font(lua);
	*font = gf_font_create_file(lua->engine->client->draw, path);
	if(*font == NULL) {
		lua_pop(s, 1);
		return 0;
	}
	arrput(lua->font_array, *font);

	return 1;
}

int gf_lua_call_font_default(lua_State* s) {
	gf_font_t** font = luaL_checkudata(s, 1, "GoldFishFont");
	gf_lua_t*   lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	lua->engine->client->draw->font = *font;

	return 1;
}

void gf_lua_create_goldfish_font(gf_lua_t* lua) {
	lua_pushstring(lua->lua, "font");
	lua_newtable(lua->lua);

	lua_pushstring(lua->lua, "load");
	lua_pushcfunction(lua->lua, gf_lua_call_font_load);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "default");
	lua_pushcfunction(lua->lua, gf_lua_call_font_default);
	lua_settable(lua->lua, -3);

	lua_settable(lua->lua, -3);
}
