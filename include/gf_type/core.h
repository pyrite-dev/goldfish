/**
 * @if SHOW_LICENSE
 **********************************************************************************
 *
 * Copyright (c) 2025 Pyrite Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. All advertising materials mentioning features or use of this software must
 *    display the following acknowledgement:
 *    This product includes software developed by the Pyrite Development Team.
 *
 * 4. Neither the name of Pyrite Development Team nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 **********************************************************************************
 * @endif
 */

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
#define gf_engine_resource_t struct gf_engine_resource
#define gf_engine_t struct gf_engine

gf_engine_t;
gf_engine_resource_t;

/* External library */

/* Engine */
#include <gf_type/server.h>
#include <gf_type/client.h>
#include <gf_type/resource.h>
#include <gf_type/lua.h>
#include <gf_type/prop.h>
#include <gf_type/command.h>
#include <gf_type/compat.h>

/* Standard */
#include <stdio.h>

/**
 * @struct gf_engine_resource
 * @~english
 * @brief Engine resource key value
 *
 * @var gf_engine_resource::key
 * @brief Key
 *
 * @var gf_engine_resource::value
 * @brief Value
 */
GF_DECLARE_TYPE(engine_resource, {
	char*	       key;
	gf_resource_t* value;
});

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
 * @var gf_engine::command_aliases
 * @brief Command alias map
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
 * @var gf_engine::resources
 * @brief Resources
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
 *
 * @var gf_engine::seed
 * @brief Seed
 */
GF_DECLARE_TYPE(engine, {
	gf_server_t*	      server;
	gf_client_t*	      client;
	gf_command_alias_t*   command_aliases;
	char*		      log_list;
	FILE*		      log;
	gf_lua_t*	      lua;
	int		      error;
	gf_resource_t*	      base;
	gf_engine_resource_t* resources;
	unsigned char*	      icon;
	int		      icon_width;
	int		      icon_height;
	gf_prop_t*	      config;
	int		      force_down;
	char*		      name;
	gf_uint64_t	      seed;
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
