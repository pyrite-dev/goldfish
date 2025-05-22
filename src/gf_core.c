#define GF_EXPOSE_CORE
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_CLIENT

#include <gf_pre.h>

/* External library */
#include <stb_image.h>
#include <stb_ds.h>

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

/* Standard */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
LARGE_INTEGER hpc_freq;
#endif
void gf_engine_begin(void) {
	gf_version_t ver;
	if(gf_log_default == NULL) gf_log_default = stderr;

	gf_gui_init_calls();

	gf_version_get(&ver);
	gf_log_function(NULL, "GoldFish Engine %s", ver.full);
	gf_log_function(NULL, "Build date: %s", ver.date);
	gf_log_function(NULL, "Lua %s", ver.lua);
	gf_log_function(NULL, "zlib %s", ver.zlib);
	gf_log_function(NULL, "Thread model: %s", ver.thread);
	gf_log_function(NULL, "Renderer: %s on %s", ver.driver, ver.backend);
#ifdef _WIN32
	if(QueryPerformanceFrequency(&hpc_freq) <= 0) {
		hpc_freq.QuadPart = 0;
	}
#endif
	gf_network_begin();
	gf_client_begin();
	gf_server_begin();
}

void gf_engine_end(void) {
	gf_server_end();
	gf_client_end();
	gf_network_end();
}

gf_engine_t* gf_engine_create(const char* title, int nogui) { return gf_engine_create_ex(title, nogui, "base.pak", NULL, 0); }

gf_engine_t* gf_engine_create_ex(const char* title, int nogui, const char* packpath, char** argv, int argc) {
	int	     st;
	char**	     list = NULL;
	gf_file_t*   f;
	gf_engine_t* engine = malloc(sizeof(*engine));
	memset(engine, 0, sizeof(*engine));
	engine->log   = stderr;
	engine->error = 0;
	engine->lua   = NULL;

	gf_prop_set_integer(&engine->config, "width", 800);
	gf_prop_set_integer(&engine->config, "height", 600);
	gf_prop_set_text(&engine->config, "texture-filter", "linear");

	engine->base = gf_resource_create(engine, packpath != NULL ? packpath : "base.pak");
	if(engine->base == NULL) {
		gf_engine_destroy(engine);
		engine = NULL;
		return engine;
	} else {
		unsigned char* png;
		size_t	       pngsize;
		int	       ch;
		if(gf_resource_get(engine->base, "icon.png", (void**)&png, &pngsize) == 0) {
			engine->icon = stbi_load_from_memory(png, pngsize, &engine->icon_width, &engine->icon_height, &ch, 4);
		}
	}

	if((f = gf_file_open(engine, "base:/autoexec.cfg", "r")) != NULL) {
		char*  buf    = malloc(f->size + 1);
		int    incr   = 0;
		int    i      = 0;
		char** aelist = NULL;
		buf[f->size]  = 0;
		gf_file_read(f, buf, f->size);

		for(i = 0;; i++) {
			if(buf[i] == 0 || buf[i] == '\n') {
				char  oldc = buf[i];
				char* line = buf + incr;
				buf[i]	   = 0;

				if(strlen(line) > 0) {
					arrput(aelist, line);
				}

				incr = i + 1;
				if(oldc == 0) break;
			} else if(buf[i] == '\r') {
				buf[i] = 0;
			}
		}

		if(aelist != NULL) {
			gf_command_run(engine, aelist, arrlen(aelist));
			arrfree(aelist);
		}

		free(buf);
		gf_file_close(f);
	}

	if(argv != NULL) {
		int   i;
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
		int i;
		gf_command_run(engine, list, arrlen(list));
		for(i = 0; i < arrlen(list); i++) {
			free(list[i]);
		}
		arrfree(list);
	}

	if(nogui) {
		gf_log_function(engine, "No GUI mode", "");
		engine->client = NULL;
	} else {
		gf_log_function(engine, "GUI mode", "");
		engine->client = gf_client_create(engine, title);
		if(engine->client == NULL) {
			gf_log_function(engine, "Failed to create client interface", "");
			gf_engine_destroy(engine);
			return NULL;
		}
		gf_log_function(engine, "Switching to graphical console", "");
	}
	engine->server = gf_server_create(engine);

	engine->lua = gf_lua_create(engine);
	if((st = gf_lua_run(engine->lua, "base:/scripts/init.lua")) != 0) {
		gf_assert(engine, st == 0);
		gf_engine_destroy(engine);
		engine = NULL;
		return engine;
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
	while(!engine->error) {
		if(engine->client != NULL) {
			if(gf_client_step(engine->client) != 0) break;
		}
	}
}

void gf_engine_destroy(gf_engine_t* engine) {
	if(engine->lua != NULL) {
		gf_lua_destroy(engine->lua);
	}
	if(engine->server != NULL) gf_server_destroy(engine->server);
	if(engine->client != NULL) gf_client_destroy(engine->client);
	if(engine->base != NULL) gf_resource_destroy(engine->base);
	if(engine->icon != NULL) {
		free(engine->icon);
	}
	free(engine);
	gf_log_function(NULL, "Destroyed engine", "");
}

void gf_engine_shutdown(gf_engine_t* engine) {
	if(engine->server != NULL) {
		gf_server_shutdown(engine->server);
	}
	if(engine->client != NULL) {
		gf_client_shutdown(engine->client);
	}
	gf_log_function(NULL, "Engine shutdown complete", "");
}
