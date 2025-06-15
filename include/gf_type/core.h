/**
 * @file gf_type/core.h
 * @~english
 * @brief Type definitions related to core
 */

#ifndef __GF_TYPE_CORE_H__
#define __GF_TYPE_CORE_H__

#include <gf_pre.h>
#include <gf_macro.h>

#define gf_engine_param_t struct gf_engine_param

gf_engine_param_t;

#ifdef GF_EXPOSE_CORE
#define gf_engine_t struct gf_engine

gf_engine_t;

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
 * @struct gf_engine
 * @~english
 * @brief Engine instance
 *
 * @var gf_engine::server
 * @brief Server interface
 *
 * @var gf_engine::client
 * @brief Client interface
 *
 * @var gf_engine::log_list
 * @brief Log output in string
 *
 * @var gf_engine::log
 * @brief Log output
 * @note Default is `stderr`
 * @warning Destroying engine instance **does not** `fclose` this
 *
 * @var gf_engine::base
 * @brief Base resource
 *
 * @var gf_engine::lua
 * @brief Lua interface
 *
 * @var gf_engine::error
 * @brief Set on error
 *
 * @var gf_engine::icon
 * @brief Icon data
 *
 * @var gf_engine::icon_width
 * @brief Icon width
 *
 * @var gf_engine::icon_height
 * @brief Icon height
 *
 * @var gf_engine::config
 * @brief Config
 *
 * @var gf_engine::force_down
 * @brief Force engine shutdown
 *
 * @var gf_engine::name
 * @brief Game name
 */
GF_DECLARE_TYPE(engine, {
	gf_server_t*   server;
	gf_client_t*   client;
	char*	       log_list;
	FILE*	       log;
	gf_resource_t* base;
	gf_lua_t*      lua;
	int	       error;
	unsigned char* icon;
	int	       icon_width;
	int	       icon_height;
	gf_prop_t*     config;
	int	       force_down;
	char*	       name;
});
#else
typedef void gf_engine_t;
#endif

GF_DECLARE_TYPE(engine_param, {
	char* game;
	char* base;
	char* prefix;
});

#endif
