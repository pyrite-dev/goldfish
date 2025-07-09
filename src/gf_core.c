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
 * File Name:    gf_core.c
 * Author:       Nishi
 *
 * Functionality:
 * Core of engine.
 *
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_CORE
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_CLIENT

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>
#ifdef _WIN32
#include <winsock.h>
#include <windows.h>
#endif

/* Interface */
#include <gf_core.h>

/* Engine */
#include <gf_gui_static.h>
#include <gf_assert.h>
#include <gf_lua.h>
#include <gf_client.h>
#include <gf_server.h>
#include <gf_log.h>
#include <gf_version.h>
#include <gf_resource.h>
#include <gf_font.h>
#include <gf_network.h>
#include <gf_command.h>
#include <gf_prop.h>
#include <gf_file.h>
#include <gf_image.h>
#include <gf_util.h>
#include <gf_gui.h>

/* Standard */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
LARGE_INTEGER hpc_freq;
#endif
void gf_engine_begin(void) {
	gf_version_t ver;
#ifdef _WIN32
	WSADATA wsa;
	WSAStartup(MAKEWORD(1, 1), &wsa);
#endif
	if(gf_log_default == NULL) gf_log_default = stderr;

	gf_gui_init_calls();

	gf_version_get(&ver);
	gf_log_function(NULL, "GoldFish Engine %s", ver.full);
	gf_log_function(NULL, "Build date: %s", ver.date);
	gf_log_function(NULL, "Lua %s", ver.lua);
	gf_log_function(NULL, "zlib %s", ver.zlib);
	gf_log_function(NULL, "PCRE %s", ver.pcre);
	gf_log_function(NULL, "Thread model: %s", ver.thread);
	gf_log_function(NULL, "Renderer: %s on %s", ver.driver, ver.backend);
#ifdef _WIN32
	gf_log_function(NULL, "Compile-time WINVER: 0x%04x", WINVER);
	if(QueryPerformanceFrequency(&hpc_freq) <= 0) {
		hpc_freq.QuadPart = 0;
	}
#endif
	gf_client_begin();
#ifndef GF_NO_SERVER
	gf_server_begin();
#endif
}

void gf_engine_end(void) {
#ifndef GF_NO_SERVER
	gf_server_end();
#endif
	gf_client_end();
}

gf_engine_t* gf_engine_create(const char* title, int nogui) {
	gf_engine_param_t param;
	memset(&param, 0, sizeof(param));
	return gf_engine_create_ex(title, nogui, param, NULL, 0);
}

static void gf_engine_init_seed(gf_engine_t* engine) {
	int	    i;
	gf_uint64_t tmp = 0;
#ifdef _WIN32
	engine->seed = GetTickCount();
	tmp ^= gf_util_random(engine);
	engine->seed = GetCurrentProcessId();
	tmp ^= gf_util_random(engine);
	engine->seed = GetCurrentThreadId();
	tmp ^= gf_util_random(engine);

	engine->seed = tmp;
#else
	FILE* f = fopen("/dev/urandom", "rb");
	if(f == NULL) f = fopen("/dev/random", "rb"); /* what unix-like could be missing urandom!?!? */
	if(f != NULL) {
		fread(&engine->seed, 1, sizeof(engine->seed), f);
		fclose(f);
	}
#endif
	/* this is to shuffle */
	for(i = 0; i < 32; i++) tmp = gf_util_random(engine);
}

gf_engine_t* gf_engine_create_ex(const char* title, int nogui, gf_engine_param_t param, char** argv, int argc) {
	int	     st;
	char**	     list = NULL;
	int	     i;
	int	     titlei = 0;
	char*	     search;
	gf_version_t ver;
	gf_engine_t* engine = malloc(sizeof(*engine));
	memset(engine, 0, sizeof(*engine));
	engine->log	    = NULL;
	engine->error	    = 0;
	engine->lua	    = NULL;
	engine->name	    = gf_util_strdup(param.game != NULL ? param.game : title);
	engine->force_down  = 0;
	engine->log_list    = malloc(1);
	engine->log_list[0] = 0;

	sh_new_strdup(engine->resources);

	engine->seed = time(NULL);
	gf_engine_init_seed(engine);

	gf_version_get(&ver);

	gf_log_function(engine, "Welcome to GoldFish Engine %s", ver.full);

	gf_prop_set_integer(&engine->config, "width", 800);
	gf_prop_set_integer(&engine->config, "height", 600);
	gf_prop_set_text(&engine->config, "texture-filter", "linear");
	if(param.prefix != NULL) gf_prop_set_text(&engine->config, "prefix", param.prefix);

	search = gf_util_get_search(engine);
	gf_log_function(engine, "Search path: %s", search);
	free(search);

	engine->base = gf_resource_create(engine, param.base != NULL ? param.base : "base.pak");
	if(engine->base == NULL) {
		gf_engine_destroy(engine);
		engine = NULL;
		return engine;
	} else {
		gf_file_register(engine, "base", engine->base);

		engine->icon = gf_image_load(engine, "base:/icon.png", &engine->icon_width, &engine->icon_height);
	}

	gf_command_file(engine, "base:/config/autoexec.cfg");

	engine->home = gf_util_get_user_path(engine);

	if(argv != NULL) {
		char* buf = NULL;
		for(i = 1; i < argc; i++) {
			char* arg = argv[i];
			if(arg[0] == '-') {
				if(buf != NULL) {
					arrput(list, buf);
				}
				buf = malloc(strlen(arg)); /* strlen(arg) - 1 + 1 */
				strcpy(buf, arg + 1);
			} else if(buf != NULL) {
				char* old = buf;
				buf	  = malloc(strlen(old) + 1 + +strlen(arg) + 2 + 1);
				strcpy(buf, old);
				strcat(buf, " ");
				strcat(buf, "\"");
				strcat(buf, arg);
				strcat(buf, "\"");
				free(old);
			}
		}
		if(buf != NULL) {
			arrput(list, buf);
		}
	}
	if(list != NULL) {
		gf_command_run(engine, list, arrlen(list));
		for(i = 0; i < arrlen(list); i++) {
			free(list[i]);
		}
		arrfree(list);
	}

	if(nogui) {
		gf_log_function(engine, "No GUI mode", "");
		engine->client = NULL;
		gf_command_file(engine, "base:/config/server.cfg");
	} else {
		gf_log_function(engine, "GUI mode", "");
		engine->client = gf_client_create(engine, title + titlei);
		if(engine->client == NULL) {
			gf_log_function(engine, "Failed to create client interface", "");
			gf_engine_destroy(engine);
			return NULL;
		}
		gf_command_file(engine, "base:/config/client.cfg");
	}
#ifndef GF_NO_SERVER
	engine->server = gf_server_create(engine);
#endif

	engine->lua = gf_lua_create(engine);
	if((st = gf_lua_run(engine->lua, "base:/scripts/init.lua")) != 0) {
		gf_assert(engine, st == 0);
		gf_engine_destroy(engine);
		engine = NULL;
		return engine;
	}

	if(!nogui) {
		gf_gui_move_topmost(engine->client->draw->gui, engine->client->draw->console);
	}
	return engine;
}

/**
 * Writing this so I don't forget
 *
 * 1.  Calls gf_client_step
 * 2.  gf_client_step calls gf_draw_step
 * 3.  gf_draw_step calls gf_draw_platform_step (Platform-dependent)
 * 4.  gf_draw_platform_step processes platform-dependent stuffs (e.g. events)
 * 5.  gf_draw_platform_step calls gf_draw_driver_before
 * 6.  gf_draw_platform_step calls gf_draw_frame
 * 7.  gf_draw_frame draws stuffs
 * 8.  gf_draw_platform_step calls gf_draw_driver_after
 * 9.  gf_draw_platform_step swaps buffers
 * 10. Comes back here
 */
void gf_engine_loop(gf_engine_t* engine) {
	while(!engine->error && !engine->force_down) {
		if(engine->client != NULL) {
			if(gf_client_step(engine->client) != 0) break;
		}
	}
}

void gf_engine_destroy(gf_engine_t* engine) {
	int i;
	free(engine->home);
	if(engine->lua != NULL) {
		gf_lua_destroy(engine->lua);
	}
#ifndef GF_NO_SERVER
	if(engine->server != NULL) gf_server_destroy(engine->server);
#endif
	if(engine->client != NULL) gf_client_destroy(engine->client);
	for(i = 0; i < shlen(engine->resources); i++) {
		gf_resource_destroy(engine->resources[i].value);
	}
	shfree(engine->resources);
	if(engine->icon != NULL) {
		free(engine->icon);
	}
	free(engine->log_list);
	free(engine);
	gf_log_function(NULL, "Destroyed engine", "");
}

void gf_engine_shutdown(gf_engine_t* engine) {
#ifndef GF_NO_SERVER
	if(engine->server != NULL) {
		gf_server_shutdown(engine->server);
	}
#endif
	if(engine->client != NULL) {
		gf_client_shutdown(engine->client);
	}
	gf_log_function(NULL, "Engine shutdown complete", "");
	if(engine->client == NULL) {
		engine->force_down = 1;
	}
}

void gf_engine_name(gf_engine_t* engine, const char* name) {
	char* search;
	if(engine->name != NULL) free(engine->name);
	engine->name = gf_util_strdup(name);

	search = gf_util_get_search(engine);
	gf_log_function(NULL, "Search path: %s", search);
	free(search);
}

void gf_engine_prefix(gf_engine_t* engine, const char* prefix) {
	char* search;
	gf_prop_set_text(&engine->config, "prefix", prefix);

	search = gf_util_get_search(engine);
	gf_log_function(NULL, "Search path: %s", search);
	free(search);
}
