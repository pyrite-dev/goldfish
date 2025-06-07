/**
 * @file gf_type/lua.h
 * @~english
 * @brief Type definitions related to lua
 */

#ifndef __GF_TYPE_LUA_H__
#define __GF_TYPE_LUA_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_LUA
#define gf_lua_t struct gf_lua

gf_lua_t;

/* External library */
#include <lua.h>

/* Engine */
#include <gf_type/core.h>
#include <gf_type/gui.h>

/* Standard */

/**
 * @struct gf_lua
 * @~english
 * @brief Lua interface
 *
 * @var gf_lua::engine
 * @brief Engine instance
 *
 * @var gf_lua::lua
 * @brief Lua
 *
 * @var gf_lua::loop
 * @brief Loop call
 *
 * @var gf_lua::close
 * @brief Close call
 */
GF_DECLARE_TYPE(lua, {
	gf_engine_t* engine;
	lua_State*   lua;
	int	     loop;
	int	     close;
});
#else
typedef void gf_lua_t;
#endif

#endif
