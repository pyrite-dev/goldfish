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
#include <gf_texture.h>
/******************/

/**
 * Symbols:
 *   gf_texture_destroy
 *   gf_texture_keep_aspect
 */

/**
 * C: void gf_texture_destroy(gf_texture_t*)
 */
int bindgen_GoldFishTexture_gf_texture_destroy(lua_State* s) {
	gf_texture_t** param0 = luaL_checkudata(s, 1, "GoldFishTexture");
	gf_lua_t*      wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	gf_texture_destroy(*param0);

	return 0;
}

/**
 * C: void gf_texture_keep_aspect(gf_texture_t*, int)
 */
int bindgen_GoldFishTexture_gf_texture_keep_aspect(lua_State* s) {
	gf_texture_t** param0 = luaL_checkudata(s, 1, "GoldFishTexture");
	int	       param1 = (int)luaL_checkinteger(s, 2);
	gf_lua_t*      wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	gf_texture_keep_aspect(*param0, param1);

	return 0;
}

void bindgen_GoldFishTexture_init(gf_lua_t* lua) {
	luaL_newmetatable(LUA(lua), "GoldFishTexture");

	lua_pushstring(LUA(lua), "destroy");
	lua_pushcfunction(LUA(lua), bindgen_GoldFishTexture_gf_texture_destroy);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "keep_aspect");
	lua_pushcfunction(LUA(lua), bindgen_GoldFishTexture_gf_texture_keep_aspect);
	lua_settable(LUA(lua), -3);

	lua_pushvalue(LUA(lua), -1);
	lua_setfield(LUA(lua), -2, "__index");
}

/*** Generated at Mon Jun 9 2025 ***/
