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
typedef struct gf_lua_t gf_lua_t;

/* External library */
#include <lua.h>

/* Engine */
#include <gf_type/core.h>
#include <gf_type/gui.h>
#include <gf_type/font.h>

/* Standard */

/**
 * @struct gf_lua_t
 * @~english
 * @brief Lua interface
 *
 * @var gf_lua_t::engine
 * @brief Engine instance
 *
 * @var gf_lua_t::lua
 * @brief Lua
 *
 * @var gf_lua_t::loop
 * @brief Loop call
 *
 * @var gf_lua_t::close
 * @brief Close call
 *
 * @var gf_lua_t::font_array
 * @brief Font array
 */
GF_DECLARE_TYPE(lua, {
	gf_engine_t* engine;
	lua_State*   lua;
	int	     loop;
	int	     close;
	gf_font_t**  font_array;
});
#else
typedef void gf_lua_t;
#endif

#endif
