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

int gf_lua_meta_call_audio_resume(lua_State* s) {
	gf_audio_id_t* id = luaL_checkudata(s, 1, "GoldFishAudio");
	gf_lua_t*      lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	gf_audio_resume(lua->engine->client->audio, *id);

	return 0;
}

int gf_lua_meta_call_audio_pause(lua_State* s) {
	gf_audio_id_t* id = luaL_checkudata(s, 1, "GoldFishAudio");
	gf_lua_t*      lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	gf_audio_pause(lua->engine->client->audio, *id);

	return 0;
}

int gf_lua_meta_call_audio_stop(lua_State* s) {
	gf_audio_id_t* id = luaL_checkudata(s, 1, "GoldFishAudio");
	gf_lua_t*      lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	gf_audio_stop(lua->engine->client->audio, *id);

	return 0;
}

int gf_lua_meta_call_audio_over(lua_State* s) {
	gf_audio_id_t* id = luaL_checkudata(s, 1, "GoldFishAudio");
	gf_lua_t*      lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);

	lua_pushboolean(s, gf_audio_is_over(lua->engine->client->audio, *id));

	return 1;
}

void gf_lua_meta_init_audio(gf_lua_t* lua) {
	luaL_newmetatable(lua->lua, "GoldFishAudio");

	lua_pushstring(lua->lua, "resume");
	lua_pushcfunction(lua->lua, gf_lua_meta_call_audio_resume);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "pause");
	lua_pushcfunction(lua->lua, gf_lua_meta_call_audio_pause);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "stop");
	lua_pushcfunction(lua->lua, gf_lua_meta_call_audio_stop);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "over");
	lua_pushcfunction(lua->lua, gf_lua_meta_call_audio_over);
	lua_settable(lua->lua, -3);

	lua_pushvalue(lua->lua, -1);
	lua_setfield(lua->lua, -2, "__index");
}
