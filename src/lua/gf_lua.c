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
#include <gf_version.h>
#include <gf_prop.h>
#include <gf_file.h>
#include <gf_log.h>
#include <gf_gui.h>
#include <gf_font.h>
#include <gf_gui_component.h>
#include <gf_graphic.h>
#include <gf_draw.h>

/* Standard */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int gf_lua_call_loop(lua_State* s) {
	int	  call = luaL_ref(s, LUA_REGISTRYINDEX);
	gf_lua_t* lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

	lua->loop = call;

	return 0;
}

int gf_lua_call_close(lua_State* s) {
	int	  call = luaL_ref(s, LUA_REGISTRYINDEX);
	gf_lua_t* lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

	lua->close = call;

	return 0;
}

int gf_lua_call_geometry(lua_State* s) {
	gf_lua_t* lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

	lua_newtable(s);

	lua_pushstring(s, "x");
	lua_pushnumber(s, lua->engine->client->draw->x);
	lua_settable(s, -3);

	lua_pushstring(s, "y");
	lua_pushnumber(s, lua->engine->client->draw->y);
	lua_settable(s, -3);

	lua_pushstring(s, "width");
	lua_pushnumber(s, lua->engine->client->draw->width);
	lua_settable(s, -3);

	lua_pushstring(s, "height");
	lua_pushnumber(s, lua->engine->client->draw->height);
	lua_settable(s, -3);

	return 1;
}

int gf_lua_call_shutdown(lua_State* s) {
	gf_lua_t* lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

	lua->engine->client->draw->close = 2;

	return 0;
}

int gf_lua_call_read(lua_State* s) {
	const char* path = luaL_checkstring(s, 1);
	gf_lua_t*   lua;
	gf_file_t*  file;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

	file = gf_file_open(lua->engine, path, "r");
	if(file != NULL) {
		char* buffer	   = malloc(file->size + 1);
		buffer[file->size] = 0;
		gf_file_read(file, buffer, file->size);
		lua_pushlstring(s, buffer, file->size + 1);
		gf_file_close(file);
		free(buffer);
		return 1;
	}

	return 0;
}

int gf_lua_call_fps(lua_State* s) {
	gf_lua_t* lua;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

	lua_pushnumber(s, gf_draw_get_fps(lua->engine->client->draw));

	return 1;
}

int gf_lua_call_require(lua_State* s) {
	const char* path = luaL_checkstring(s, 1);
	gf_lua_t*   lua;
	gf_file_t*  f;

	lua_getglobal(s, "_GF_LUA");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

	lua_getglobal(s, "package");
	lua_getfield(s, -1, "loaded");
	lua_getfield(s, -1, path);
	if(strcmp(luaL_typename(s, -1), "nil") != 0) {
		lua_remove(s, -2);
		lua_remove(s, -2);
		return 1;
	}
	lua_pop(s, 3);

	f = gf_file_open(lua->engine, path, "r");
	if(f != NULL) {
		char* d = malloc(f->size + 1);
		gf_file_read(f, d, f->size);
		d[f->size] = 0;
		gf_file_close(f);

		if(luaL_loadstring(s, d)) {
			free(d);
			lua_pop(s, 1);
		} else if(lua_pcall(s, 0, LUA_MULTRET, 0)) {
			free(d);
			lua_pop(s, 1);
		} else {
			free(d);
		}

		lua_getglobal(s, "package");
		lua_getfield(s, -1, "loaded");

		lua_pushstring(s, path);
		lua_pushvalue(s, -4);
		lua_settable(s, -3);

		return lua_gettop(s) - 1;
	}

	return 0;
}

void gf_lua_create_goldfish(gf_lua_t* lua) {
	gf_version_t ver;
	gf_version_get(&ver);

	lua_newtable(lua->lua);

	gf_lua_create_goldfish_gui(lua);
	gf_lua_create_goldfish_graphic(lua);
	gf_lua_create_goldfish_font(lua);
	gf_lua_create_goldfish_audio(lua);

	lua_pushstring(lua->lua, "loop");
	lua_pushcfunction(lua->lua, gf_lua_call_loop);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "close");
	lua_pushcfunction(lua->lua, gf_lua_call_close);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "geometry");
	lua_pushcfunction(lua->lua, gf_lua_call_geometry);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "shutdown");
	lua_pushcfunction(lua->lua, gf_lua_call_shutdown);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "fps");
	lua_pushcfunction(lua->lua, gf_lua_call_fps);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "read");
	lua_pushcfunction(lua->lua, gf_lua_call_read);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "version");
	lua_pushstring(lua->lua, ver.full);
	lua_settable(lua->lua, -3);

	lua_setglobal(lua->lua, "goldfish");
}

gf_lua_t* gf_lua_create(gf_engine_t* engine) {
	char	  prg[1024];
	gf_lua_t* lua = malloc(sizeof(*lua));
	memset(lua, 0, sizeof(*lua));
	lua->engine = engine;

	lua->loop	= 0;
	lua->close	= 0;
	lua->font_array = NULL;

	lua->lua = luaL_newstate();
	luaL_openlibs(lua->lua);

	lua_pushlightuserdata(lua->lua, lua);
	lua_setglobal(lua->lua, "_GF_LUA");

	gf_lua_meta_init(lua);

	gf_lua_create_goldfish(lua);

	lua_pushcfunction(lua->lua, gf_lua_call_require);
	lua_setglobal(lua->lua, "require");

	return lua;
}

int gf_lua_run(gf_lua_t* lua, const char* path) {
	gf_file_t* f = gf_file_open(lua->engine, path, "r");
	char*	   str;
	if(f == NULL) return -1;
	str = malloc(f->size + 1);
	gf_file_read(f, str, f->size);
	str[f->size] = 0;

	if(luaL_loadstring(lua->lua, str)) {
		gf_log_function(lua->engine, "Lua error: %s", lua_tostring(lua->lua, -1));
		free(str);
		gf_file_close(f);
		return -1;
	}

	if(lua_pcall(lua->lua, 0, LUA_MULTRET, 0)) {
		gf_log_function(lua->engine, "Lua error: %s", lua_tostring(lua->lua, -1));
		free(str);
		gf_file_close(f);
		return -1;
	}

	free(str);
	gf_file_close(f);
	return 0;
}

void gf_lua_step(gf_lua_t* lua) {
	if(lua->loop == 0) return;

	lua_rawgeti(lua->lua, LUA_REGISTRYINDEX, lua->loop);
	lua_pcall(lua->lua, 0, 0, 0);
}

void gf_lua_close(gf_lua_t* lua) {
	if(lua->close == 0) return;

	lua_rawgeti(lua->lua, LUA_REGISTRYINDEX, lua->close);
	lua_pcall(lua->lua, 0, 0, 0);
}

void gf_lua_destroy(gf_lua_t* lua) {
	int i;
	lua_close(lua->lua);

	for(i = 0; i < arrlen(lua->font_array); i++) {
		gf_font_destroy(lua->font_array[i]);
	}
	arrfree(lua->font_array);

	gf_log_function(lua->engine, "Destroyed Lua interface", "");
	free(lua);
}
