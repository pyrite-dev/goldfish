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
#include <gf_audio.h>

#include <bindgen.h>

/* Standard */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int gf_lua_call_loop(lua_State* s) {
	int	  call = luaL_ref(s, LUA_REGISTRYINDEX);
	gf_lua_t* lua;

	lua_getglobal(s, "_LUA_WRAP");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

	lua->loop = call;

	return 0;
}

int gf_lua_call_close(lua_State* s) {
	int	  call = luaL_ref(s, LUA_REGISTRYINDEX);
	gf_lua_t* lua;

	lua_getglobal(s, "_LUA_WRAP");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

	lua->close = call;

	return 0;
}

int gf_lua_call_geometry(lua_State* s) {
	gf_lua_t* lua;

	lua_getglobal(s, "_LUA_WRAP");
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

	lua_getglobal(s, "_LUA_WRAP");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

	lua->engine->client->draw->close = 2;

	return 0;
}

int gf_lua_call_read(lua_State* s) {
	const char* path = luaL_checkstring(s, 1);
	gf_lua_t*   lua;
	gf_file_t*  file;

	lua_getglobal(s, "_LUA_WRAP");
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

	lua_getglobal(s, "_LUA_WRAP");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

	lua_pushnumber(s, gf_draw_get_fps(lua->engine->client->draw));

	return 1;
}

int gf_lua_call_font_default(lua_State* s) {
	gf_font_t** font = luaL_checkudata(s, 1, "GoldFishFont");
	gf_lua_t*   lua;

	lua_getglobal(s, "_LUA_WRAP");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

	lua->engine->client->draw->font = *font;

	return 1;
}

int gf_lua_call_require(lua_State* s) {
	const char* path = luaL_checkstring(s, 1);
	gf_lua_t*   lua;
	gf_file_t*  f;

	lua_getglobal(s, "_LUA_WRAP");
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

int gf_lua_meta_call_gui_component_prop(lua_State* s) {
	gf_gui_id_t* id	  = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	const char*  type = luaL_checkstring(s, 2);
	const char*  str  = luaL_checkstring(s, 3);
	gf_lua_t*    lua;

	lua_getglobal(s, "_LUA_WRAP");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

	if(lua_gettop(s) == 4) {
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
			gf_gui_id_t* r = lua_newuserdata(s, sizeof(*r));
			luaL_getmetatable(s, "GoldFishGUIComponent");
			lua_setmetatable(s, -2);
			*r = gf_gui_get_prop_id(lua->engine->client->draw->gui, *id, str);
		} else if(strcmp(type, "integer") == 0) {
			lua_pushinteger(s, gf_prop_get_integer(gf_gui_get_prop(lua->engine->client->draw->gui, *id), str));
		} else if(strcmp(type, "floating") == 0) {
			lua_pushnumber(s, gf_prop_get_floating(gf_gui_get_prop(lua->engine->client->draw->gui, *id), str));
		}
	}
	return 1;
}

void gf_lua_gui_callback(gf_engine_t* engine, gf_draw_t* draw, gf_gui_id_t id, int type) {
	int*	     call = gf_prop_get_ptr(gf_gui_get_prop(draw->gui, id), "luacall");
	gf_lua_t*    lua;
	gf_gui_id_t* pid;
	if(call == NULL) return;

	lua = (gf_lua_t*)gf_prop_get_ptr_keep(gf_gui_get_prop(draw->gui, id), "lua");

	lua_rawgeti(lua->lua, LUA_REGISTRYINDEX, *call);

	pid = lua_newuserdata(lua->lua, sizeof(*pid));
	luaL_getmetatable(lua->lua, "GoldFishGUIComponent");
	lua_setmetatable(lua->lua, -2);
	*pid = id;

	lua_pushnumber(lua->lua, type);
	if(lua_pcall(lua->lua, 2, 0, 0)) {
		gf_log_function(lua->engine, "Lua error: %s", lua_tostring(lua->lua, -1));
	}
}

int gf_lua_meta_call_gui_component_callback(lua_State* s) {
	gf_gui_id_t* id = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	gf_lua_t*    lua;

	lua_getglobal(s, "_LUA_WRAP");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

	if(lua_gettop(s) == 2) {
		int* ptr = malloc(sizeof(*ptr));
		*ptr	 = luaL_ref(s, LUA_REGISTRYINDEX);
		gf_prop_set_ptr(gf_gui_get_prop(lua->engine->client->draw->gui, *id), "luacall", (void*)ptr);
		gf_gui_set_callback(lua->engine->client->draw->gui, *id, gf_lua_gui_callback);
		gf_prop_set_ptr_keep(gf_gui_get_prop(lua->engine->client->draw->gui, *id), "lua", (void*)lua);
	} else {
		gf_prop_delete(gf_gui_get_prop(lua->engine->client->draw->gui, *id), "luacall");
	}
	return 0;
}

int gf_lua_meta_call_gui_component_font(lua_State* s) {
	gf_gui_id_t* id = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	gf_lua_t*    lua;

	lua_getglobal(s, "_LUA_WRAP");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

	if(lua_gettop(s) == 2) {
		gf_font_t** font = luaL_checkudata(s, 2, "GoldFishFont");
		gf_prop_set_ptr_keep(gf_gui_get_prop(lua->engine->client->draw->gui, *id), "font", *font);
	} else {
		gf_font_t*  pfont = gf_prop_get_ptr_keep(gf_gui_get_prop(lua->engine->client->draw->gui, *id), "font");
		gf_font_t** font  = lua_newuserdata(s, sizeof(*font));
		luaL_getmetatable(s, "GoldFishFont");
		lua_setmetatable(s, -2);
		*font = pfont;

		*font = pfont;
		return 1;
	}
	return 0;
}

int gf_lua_meta_call_gui_component_size(lua_State* s) {
	gf_gui_id_t* id = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	gf_lua_t*    lua;

	lua_getglobal(s, "_LUA_WRAP");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

	if(lua_gettop(s) == 2) {
		double w;
		double h;

		lua_rawgeti(s, 2, 1);
		w = lua_tonumber(s, -1);
		lua_pop(s, 1);

		lua_rawgeti(s, 2, 2);
		h = lua_tonumber(s, -1);
		lua_pop(s, 1);

		gf_gui_set_wh(lua->engine->client->draw->gui, *id, w, h);
	} else {
		double w;
		double h;

		gf_gui_get_wh(lua->engine->client->draw->gui, *id, &w, &h);

		lua_newtable(s);

		lua_pushnumber(s, w);
		lua_rawseti(s, -2, 1);

		lua_pushnumber(s, h);
		lua_rawseti(s, -2, 2);

		return 1;
	}
	return 0;
}

int gf_lua_meta_call_gui_component_move(lua_State* s) {
	gf_gui_id_t* id = luaL_checkudata(s, 1, "GoldFishGUIComponent");
	gf_lua_t*    lua;

	lua_getglobal(s, "_LUA_WRAP");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

	if(lua_gettop(s) == 2) {
		double x;
		double y;

		lua_rawgeti(s, 2, 1);
		x = lua_tonumber(s, -1);
		lua_pop(s, 1);

		lua_rawgeti(s, 2, 2);
		y = lua_tonumber(s, -1);
		lua_pop(s, 1);

		gf_gui_set_xy(lua->engine->client->draw->gui, *id, x, y);
	} else {
		double x;
		double y;

		gf_gui_get_xy(lua->engine->client->draw->gui, *id, &x, &y);

		lua_newtable(s);

		lua_pushnumber(s, x);
		lua_rawseti(s, -2, 1);

		lua_pushnumber(s, y);
		lua_rawseti(s, -2, 2);

		return 1;
	}
	return 0;
}

int gf_lua_meta_call_audio_over(lua_State* s) {
	gf_audio_id_t* id = luaL_checkudata(s, 1, "GoldFishAudio");
	gf_lua_t*      lua;

	lua_getglobal(s, "_LUA_WRAP");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

	lua_pushboolean(s, gf_audio_is_over(lua->engine->client->audio, *id));

	return 1;
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

	lua_getglobal(s, "_LUA_WRAP");
	lua = lua_touserdata(s, -1);
	lua_pop(s, 1);

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

void gf_lua_create_goldfish(gf_lua_t* lua) {
	gf_version_t ver;
	gf_version_get(&ver);

	lua_getglobal(lua->lua, "goldfish");

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

	lua_pushstring(lua->lua, "server_only");
	lua_pushboolean(lua->lua, lua->engine->client == NULL ? 1 : 0);
	lua_settable(lua->lua, -3);

	lua_setglobal(lua->lua, "goldfish");
}

gf_lua_t* gf_lua_create(gf_engine_t* engine) {
	gf_lua_t* lua = malloc(sizeof(*lua));
	memset(lua, 0, sizeof(*lua));
	lua->engine = engine;

	lua->loop	= 0;
	lua->close	= 0;
	lua->font_array = NULL;

	lua->lua = luaL_newstate();
	luaL_openlibs(lua->lua);

	bindgen_init(lua);

	gf_lua_create_goldfish(lua);

	lua_pushcfunction(lua->lua, gf_lua_call_require);
	lua_setglobal(lua->lua, "require");

	/* GoldFishGUIComponent */
	luaL_getmetatable(lua->lua, "GoldFishGUIComponent");

	lua_pushstring(lua->lua, "prop");
	lua_pushcfunction(lua->lua, gf_lua_meta_call_gui_component_prop);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "size");
	lua_pushcfunction(lua->lua, gf_lua_meta_call_gui_component_size);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "move");
	lua_pushcfunction(lua->lua, gf_lua_meta_call_gui_component_move);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "font");
	lua_pushcfunction(lua->lua, gf_lua_meta_call_gui_component_font);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "callback");
	lua_pushcfunction(lua->lua, gf_lua_meta_call_gui_component_callback);
	lua_settable(lua->lua, -3);

	lua_pop(lua->lua, 1);

	/* GoldFishAudio */
	luaL_getmetatable(lua->lua, "GoldFishAudio");

	lua_pushstring(lua->lua, "over");
	lua_pushcfunction(lua->lua, gf_lua_meta_call_audio_over);
	lua_settable(lua->lua, -3);

	lua_settable(lua->lua, -3);

	lua_pop(lua->lua, 1);

	/* goldfish.font */
	lua_getglobal(lua->lua, "goldfish");
	lua_getfield(lua->lua, -1, "font");

	lua_pushstring(lua->lua, "default");
	lua_pushcfunction(lua->lua, gf_lua_call_font_default);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "load");
	lua_getfield(lua->lua, -2, "create_file");
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "create_file");
	lua_pushnil(lua->lua);
	lua_settable(lua->lua, -3);

	lua_pop(lua->lua, 2);

	/* goldfish.gui */
	lua_getglobal(lua->lua, "goldfish");
	lua_getfield(lua->lua, -1, "gui");

	lua_pushstring(lua->lua, "PRESS_EVENT");
	lua_pushinteger(lua->lua, GF_GUI_PRESS_EVENT);
	lua_settable(lua->lua, -3);

	lua_pop(lua->lua, 2);

	/* goldfish.audio */
	lua_getglobal(lua->lua, "goldfish");
	lua_getfield(lua->lua, -1, "audio");

	lua_pushstring(lua->lua, "load");
	lua_getfield(lua->lua, -2, "load_file");
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "load_file");
	lua_pushnil(lua->lua);
	lua_settable(lua->lua, -3);

	lua_pop(lua->lua, 2);

	/* goldfish.graphic */
	lua_getglobal(lua->lua, "goldfish");
	lua_getfield(lua->lua, -1, "graphic");

	lua_pushstring(lua->lua, "DIM_2D");
	lua_pushinteger(lua->lua, GF_GRAPHIC_2D);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "DIM_3D");
	lua_pushinteger(lua->lua, GF_GRAPHIC_3D);
	lua_settable(lua->lua, -3);

	lua_pushstring(lua->lua, "points");
	lua_pushcfunction(lua->lua, gf_lua_call_graphic_points);
	lua_settable(lua->lua, -3);

	lua_pop(lua->lua, 2);

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
