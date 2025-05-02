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
#include <gf_audio.h>
#include <gf_prop.h>
#include <gf_file.h>
#include <gf_log.h>
#include <gf_gui.h>
#include <gf_font.h>
#include <gf_gui_component.h>
#include <gf_graphic.h>

/* Standard */

gf_audio_id_t* gf_lua_create_audio(gf_lua_t* lua) {
	gf_audio_id_t* id = lua_newuserdata(lua->lua, sizeof(*id));
	luaL_getmetatable(lua->lua, "GoldFishAudio");
	lua_setmetatable(lua->lua, -2);
	return id;
}

int gf_lua_call_audio_create(lua_State* s) {
	const char*    str = luaL_checkstring(s, 1);
	gf_lua_t*      lua;
	gf_audio_id_t* id;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	id  = gf_lua_create_audio(lua);
	*id = gf_audio_load_file(lua->engine->client->audio, str);
	if(*id == -1) {
		return 0;
	}

	return 1;
}

void gf_lua_create_goldfish_audio(gf_lua_t* lua) {
	lua_pushstring(lua->lua, "audio");
	lua_newtable(lua->lua);

	lua_pushstring(lua->lua, "create");
	lua_pushcfunction(lua->lua, gf_lua_call_audio_create);
	lua_settable(lua->lua, -3);

	lua_settable(lua->lua, -3);
}
