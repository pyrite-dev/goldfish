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
#include <gf_font.h>
/******************/

/**
 * Symbols:
 *   gf_font_create_file
 */

/**
 * C: gf_font_t* gf_font_create_file(gf_draw_t*, const char*)
 */
int bindgen_font_gf_font_create_file(lua_State* s) {
	gf_draw_t*  param0;
	const char* param1 = (const char*)luaL_checkstring(s, 1);
	gf_lua_t*   wrap;
	gf_font_t*  ret;
	gf_font_t** lret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw;

	ret = gf_font_create_file(param0, param1);
	if(!(ret != NULL)) return 0;
	lret = lua_newuserdata(s, sizeof(*lret));
	luaL_getmetatable(s, "GoldFishFont");
	lua_setmetatable(s, -2);
	*lret = ret;

	return 1;
}

void bindgen_font_init(gf_lua_t* lua) {
	lua_pushstring(LUA(lua), "font");
	lua_newtable(LUA(lua));

	lua_pushstring(LUA(lua), "create_file");
	lua_pushcfunction(LUA(lua), bindgen_font_gf_font_create_file);
	lua_settable(LUA(lua), -3);

	lua_settable(LUA(lua), -3);
}

/*** Generated at Wed Jul 2 2025 ***/
