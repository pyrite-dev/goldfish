#define GF_EXPOSE_DRAW_PLATFORM
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_CORE
#define GF_EXPOSE_INPUT

#include <gf_pre.h>

/* Mac OS System */
#include <Quickdraw.h>
#include <Windows.h>
#include <agl.h>
#include <aglMacro.h>

/* Interface */
#include <gf_draw_platform.h>

/* Engine */
#include <gf_draw_driver.h>
#include <gf_log.h>
#include <gf_draw.h>
#include <gf_input.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

void gf_draw_platform_begin(void) {}

void gf_draw_platform_end(void) {}

int gf_draw_platform_has_extension(gf_draw_t* draw, const char* query) { return false; }

int gf_draw_platform_step(gf_draw_t* draw) { return 0; }

gf_draw_platform_t* gf_draw_platform_create(gf_engine_t* engine, gf_draw_t* draw) {
	gf_draw_platform_t* platform = malloc(sizeof(*platform));
	memset(platform, 0, sizeof(*platform));
	platform->engine = engine;

	draw->platform = platform;

	return platform;
}

void gf_draw_platform_destroy(gf_draw_platform_t* platform) { free(platform); }
