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

void bindgen_gui_init(gf_lua_t* lua){
	lua_pushstring(LUA(lua), "gui");
	lua_newtable(LUA(lua));

	lua_settable(LUA(lua), -3);
}

/*** Generated at Wed May 7 2025 ***/
