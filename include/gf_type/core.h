/**
 * @file gf_type/core.h
 * @~english
 * @brief Type definitions related to core
 */

#ifndef __GF_TYPE_CORE_H__
#define __GF_TYPE_CORE_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_CORE
typedef struct gf_engine_t gf_engine_t;

/* External library */

/* Engine */
#include <gf_type/server.h>
#include <gf_type/client.h>
#include <gf_type/resource.h>
#include <gf_type/lua.h>
#include <gf_type/prop.h>

/* Standard */
#include <stdio.h>

/**
 * @struct gf_engine_t
 * @~english
 * @brief Engine instance
 *
 * @var gf_engine_t::server
 * @brief Server interface
 *
 * @var gf_engine_t::client
 * @brief Client interface
 *
 * @var gf_engine_t::log
 * @brief Log output
 * @note Default is `stderr`
 * @warning This is `NULL` when logger is using graphical console
 * @warning Destroying engien instance **does not** `fclose` this
 *
 * @var gf_engine_t::base
 * @brief Base resource
 *
 * @var gf_engine_t::lua
 * @brief Lua interface
 *
 * @var gf_engine_t::error
 * @brief Set on error
 *
 * @var gf_engine_t::icon
 * @brief Icon data
 *
 * @var gf_engine_t::icon_width
 * @brief Icon width
 *
 * @var gf_engine_t::icon_height
 * @brief Icon height
 *
 * @var gf_engine_t::config
 * @brief Config
 */
GF_DECLARE_TYPE(engine, {
	gf_server_t*   server;
	gf_client_t*   client;
	FILE*	       log;
	gf_resource_t* base;
	gf_lua_t*      lua;
	int	       error;
	unsigned char* icon;
	int icon_width;
	int icon_height;
	gf_prop_t* config;
});
#else
typedef void gf_engine_t;
#endif

#endif
