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
/******************/

/**
 * Symbols:
 */

void bindgen_GoldFishFont_init(gf_lua_t* lua) {
	luaL_newmetatable(LUA(lua), "GoldFishFont");

	lua_pushvalue(LUA(lua), -1);
	lua_setfield(LUA(lua), -2, "__index");
}

/*** Generated at Sat May 10 2025 ***/
