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
#include <gf_gui.h>
#include <gf_gui_component.h>
/******************/

/**
 * Symbols:
 *   gf_gui_set_button_sound
 *   gf_gui_sort_component
 *   gf_gui_create_common
 *   gf_gui_create_button
 *   gf_gui_create_checkbox
 *   gf_gui_create_entry
 *   gf_gui_create_frame
 *   gf_gui_create_progress
 *   gf_gui_create_range
 *   gf_gui_create_scrollbar
 *   gf_gui_create_tab
 *   gf_gui_create_text
 *   gf_gui_create_window
 */

/**
 * C: void gf_gui_set_button_sound(gf_gui_t*, const char*)
 */
int bindgen_gui_gf_gui_set_button_sound(lua_State* s) {
	gf_gui_t*   param0;
	const char* param1 = (const char*)luaL_checkstring(s, 1);
	gf_lua_t*   wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	gf_gui_set_button_sound(param0, param1);

	return 0;
}

/**
 * C: void gf_gui_sort_component(gf_gui_t*)
 */
int bindgen_gui_gf_gui_sort_component(lua_State* s) {
	gf_gui_t* param0;
	gf_lua_t* wrap;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	gf_gui_sort_component(param0);

	return 0;
}

/**
 * C: gf_gui_id_t gf_gui_create_common(gf_gui_t*, const char*, double, double, double, double)
 */
int bindgen_gui_gf_gui_create_common(lua_State* s) {
	gf_gui_t*    param0;
	const char*  param1 = (const char*)luaL_checkstring(s, 1);
	double	     param2 = (double)luaL_checknumber(s, 2);
	double	     param3 = (double)luaL_checknumber(s, 3);
	double	     param4 = (double)luaL_checknumber(s, 4);
	double	     param5 = (double)luaL_checknumber(s, 5);
	gf_lua_t*    wrap;
	gf_gui_id_t  ret;
	gf_gui_id_t* lret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	ret = gf_gui_create_common(param0, param1, param2, param3, param4, param5);
	if(!(ret >= 0)) return 0;
	lret = lua_newuserdata(s, sizeof(*lret));
	luaL_getmetatable(s, "GoldFishGUIComponent");
	lua_setmetatable(s, -2);
	*lret = ret;

	return 1;
}

/**
 * C: gf_gui_id_t gf_gui_create_button(gf_gui_t*, double, double, double, double)
 */
int bindgen_gui_gf_gui_create_button(lua_State* s) {
	gf_gui_t*    param0;
	double	     param1 = (double)luaL_checknumber(s, 1);
	double	     param2 = (double)luaL_checknumber(s, 2);
	double	     param3 = (double)luaL_checknumber(s, 3);
	double	     param4 = (double)luaL_checknumber(s, 4);
	gf_lua_t*    wrap;
	gf_gui_id_t  ret;
	gf_gui_id_t* lret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	ret = gf_gui_create_button(param0, param1, param2, param3, param4);
	if(!(ret >= 0)) return 0;
	lret = lua_newuserdata(s, sizeof(*lret));
	luaL_getmetatable(s, "GoldFishGUIComponent");
	lua_setmetatable(s, -2);
	*lret = ret;

	return 1;
}

/**
 * C: gf_gui_id_t gf_gui_create_checkbox(gf_gui_t*, double, double, double, double)
 */
int bindgen_gui_gf_gui_create_checkbox(lua_State* s) {
	gf_gui_t*    param0;
	double	     param1 = (double)luaL_checknumber(s, 1);
	double	     param2 = (double)luaL_checknumber(s, 2);
	double	     param3 = (double)luaL_checknumber(s, 3);
	double	     param4 = (double)luaL_checknumber(s, 4);
	gf_lua_t*    wrap;
	gf_gui_id_t  ret;
	gf_gui_id_t* lret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	ret = gf_gui_create_checkbox(param0, param1, param2, param3, param4);
	if(!(ret >= 0)) return 0;
	lret = lua_newuserdata(s, sizeof(*lret));
	luaL_getmetatable(s, "GoldFishGUIComponent");
	lua_setmetatable(s, -2);
	*lret = ret;

	return 1;
}

/**
 * C: gf_gui_id_t gf_gui_create_entry(gf_gui_t*, double, double, double, double)
 */
int bindgen_gui_gf_gui_create_entry(lua_State* s) {
	gf_gui_t*    param0;
	double	     param1 = (double)luaL_checknumber(s, 1);
	double	     param2 = (double)luaL_checknumber(s, 2);
	double	     param3 = (double)luaL_checknumber(s, 3);
	double	     param4 = (double)luaL_checknumber(s, 4);
	gf_lua_t*    wrap;
	gf_gui_id_t  ret;
	gf_gui_id_t* lret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	ret = gf_gui_create_entry(param0, param1, param2, param3, param4);
	if(!(ret >= 0)) return 0;
	lret = lua_newuserdata(s, sizeof(*lret));
	luaL_getmetatable(s, "GoldFishGUIComponent");
	lua_setmetatable(s, -2);
	*lret = ret;

	return 1;
}

/**
 * C: gf_gui_id_t gf_gui_create_frame(gf_gui_t*, double, double, double, double)
 */
int bindgen_gui_gf_gui_create_frame(lua_State* s) {
	gf_gui_t*    param0;
	double	     param1 = (double)luaL_checknumber(s, 1);
	double	     param2 = (double)luaL_checknumber(s, 2);
	double	     param3 = (double)luaL_checknumber(s, 3);
	double	     param4 = (double)luaL_checknumber(s, 4);
	gf_lua_t*    wrap;
	gf_gui_id_t  ret;
	gf_gui_id_t* lret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	ret = gf_gui_create_frame(param0, param1, param2, param3, param4);
	if(!(ret >= 0)) return 0;
	lret = lua_newuserdata(s, sizeof(*lret));
	luaL_getmetatable(s, "GoldFishGUIComponent");
	lua_setmetatable(s, -2);
	*lret = ret;

	return 1;
}

/**
 * C: gf_gui_id_t gf_gui_create_progress(gf_gui_t*, double, double, double, double)
 */
int bindgen_gui_gf_gui_create_progress(lua_State* s) {
	gf_gui_t*    param0;
	double	     param1 = (double)luaL_checknumber(s, 1);
	double	     param2 = (double)luaL_checknumber(s, 2);
	double	     param3 = (double)luaL_checknumber(s, 3);
	double	     param4 = (double)luaL_checknumber(s, 4);
	gf_lua_t*    wrap;
	gf_gui_id_t  ret;
	gf_gui_id_t* lret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	ret = gf_gui_create_progress(param0, param1, param2, param3, param4);
	if(!(ret >= 0)) return 0;
	lret = lua_newuserdata(s, sizeof(*lret));
	luaL_getmetatable(s, "GoldFishGUIComponent");
	lua_setmetatable(s, -2);
	*lret = ret;

	return 1;
}

/**
 * C: gf_gui_id_t gf_gui_create_range(gf_gui_t*, double, double, double, double)
 */
int bindgen_gui_gf_gui_create_range(lua_State* s) {
	gf_gui_t*    param0;
	double	     param1 = (double)luaL_checknumber(s, 1);
	double	     param2 = (double)luaL_checknumber(s, 2);
	double	     param3 = (double)luaL_checknumber(s, 3);
	double	     param4 = (double)luaL_checknumber(s, 4);
	gf_lua_t*    wrap;
	gf_gui_id_t  ret;
	gf_gui_id_t* lret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	ret = gf_gui_create_range(param0, param1, param2, param3, param4);
	if(!(ret >= 0)) return 0;
	lret = lua_newuserdata(s, sizeof(*lret));
	luaL_getmetatable(s, "GoldFishGUIComponent");
	lua_setmetatable(s, -2);
	*lret = ret;

	return 1;
}

/**
 * C: gf_gui_id_t gf_gui_create_scrollbar(gf_gui_t*, double, double, double, double)
 */
int bindgen_gui_gf_gui_create_scrollbar(lua_State* s) {
	gf_gui_t*    param0;
	double	     param1 = (double)luaL_checknumber(s, 1);
	double	     param2 = (double)luaL_checknumber(s, 2);
	double	     param3 = (double)luaL_checknumber(s, 3);
	double	     param4 = (double)luaL_checknumber(s, 4);
	gf_lua_t*    wrap;
	gf_gui_id_t  ret;
	gf_gui_id_t* lret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	ret = gf_gui_create_scrollbar(param0, param1, param2, param3, param4);
	if(!(ret >= 0)) return 0;
	lret = lua_newuserdata(s, sizeof(*lret));
	luaL_getmetatable(s, "GoldFishGUIComponent");
	lua_setmetatable(s, -2);
	*lret = ret;

	return 1;
}

/**
 * C: gf_gui_id_t gf_gui_create_tab(gf_gui_t*, double, double, double, double)
 */
int bindgen_gui_gf_gui_create_tab(lua_State* s) {
	gf_gui_t*    param0;
	double	     param1 = (double)luaL_checknumber(s, 1);
	double	     param2 = (double)luaL_checknumber(s, 2);
	double	     param3 = (double)luaL_checknumber(s, 3);
	double	     param4 = (double)luaL_checknumber(s, 4);
	gf_lua_t*    wrap;
	gf_gui_id_t  ret;
	gf_gui_id_t* lret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	ret = gf_gui_create_tab(param0, param1, param2, param3, param4);
	if(!(ret >= 0)) return 0;
	lret = lua_newuserdata(s, sizeof(*lret));
	luaL_getmetatable(s, "GoldFishGUIComponent");
	lua_setmetatable(s, -2);
	*lret = ret;

	return 1;
}

/**
 * C: gf_gui_id_t gf_gui_create_text(gf_gui_t*, double, double, double, double)
 */
int bindgen_gui_gf_gui_create_text(lua_State* s) {
	gf_gui_t*    param0;
	double	     param1 = (double)luaL_checknumber(s, 1);
	double	     param2 = (double)luaL_checknumber(s, 2);
	double	     param3 = (double)luaL_checknumber(s, 3);
	double	     param4 = (double)luaL_checknumber(s, 4);
	gf_lua_t*    wrap;
	gf_gui_id_t  ret;
	gf_gui_id_t* lret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	ret = gf_gui_create_text(param0, param1, param2, param3, param4);
	if(!(ret >= 0)) return 0;
	lret = lua_newuserdata(s, sizeof(*lret));
	luaL_getmetatable(s, "GoldFishGUIComponent");
	lua_setmetatable(s, -2);
	*lret = ret;

	return 1;
}

/**
 * C: gf_gui_id_t gf_gui_create_window(gf_gui_t*, double, double, double, double)
 */
int bindgen_gui_gf_gui_create_window(lua_State* s) {
	gf_gui_t*    param0;
	double	     param1 = (double)luaL_checknumber(s, 1);
	double	     param2 = (double)luaL_checknumber(s, 2);
	double	     param3 = (double)luaL_checknumber(s, 3);
	double	     param4 = (double)luaL_checknumber(s, 4);
	gf_lua_t*    wrap;
	gf_gui_id_t  ret;
	gf_gui_id_t* lret;

	lua_getglobal(s, "_LUA_WRAP");
	wrap = lua_touserdata(s, -1);
	lua_pop(s, 1);

	param0 = wrap->engine->client->draw->gui;

	ret = gf_gui_create_window(param0, param1, param2, param3, param4);
	if(!(ret >= 0)) return 0;
	lret = lua_newuserdata(s, sizeof(*lret));
	luaL_getmetatable(s, "GoldFishGUIComponent");
	lua_setmetatable(s, -2);
	*lret = ret;

	return 1;
}

void bindgen_gui_init(gf_lua_t* lua) {
	lua_pushstring(LUA(lua), "gui");
	lua_newtable(LUA(lua));

	lua_pushstring(LUA(lua), "set_button_sound");
	lua_pushcfunction(LUA(lua), bindgen_gui_gf_gui_set_button_sound);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "sort");
	lua_pushcfunction(LUA(lua), bindgen_gui_gf_gui_sort_component);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "create");
	lua_pushcfunction(LUA(lua), bindgen_gui_gf_gui_create_common);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "create_button");
	lua_pushcfunction(LUA(lua), bindgen_gui_gf_gui_create_button);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "create_checkbox");
	lua_pushcfunction(LUA(lua), bindgen_gui_gf_gui_create_checkbox);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "create_entry");
	lua_pushcfunction(LUA(lua), bindgen_gui_gf_gui_create_entry);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "create_frame");
	lua_pushcfunction(LUA(lua), bindgen_gui_gf_gui_create_frame);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "create_progress");
	lua_pushcfunction(LUA(lua), bindgen_gui_gf_gui_create_progress);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "create_range");
	lua_pushcfunction(LUA(lua), bindgen_gui_gf_gui_create_range);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "create_scrollbar");
	lua_pushcfunction(LUA(lua), bindgen_gui_gf_gui_create_scrollbar);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "create_tab");
	lua_pushcfunction(LUA(lua), bindgen_gui_gf_gui_create_tab);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "create_text");
	lua_pushcfunction(LUA(lua), bindgen_gui_gf_gui_create_text);
	lua_settable(LUA(lua), -3);

	lua_pushstring(LUA(lua), "create_window");
	lua_pushcfunction(LUA(lua), bindgen_gui_gf_gui_create_window);
	lua_settable(LUA(lua), -3);

	lua_settable(LUA(lua), -3);
}

/*** Generated at Wed Jul 2 2025 ***/
