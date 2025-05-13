#define GF_EXPOSE_DRAW
#define GF_EXPOSE_CORE
#define GF_EXPOSE_CLIENT

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>
#include <stb_image.h>

/* Interface */
#include <gf_draw.h>

/* Engine */
#include <gf_prop.h>
#include <gf_core.h>
#include <gf_log.h>
#include <gf_draw_platform.h>
#include <gf_draw_driver.h>
#include <gf_texture.h>
#include <gf_graphic.h>
#include <gf_font.h>
#include <gf_gui.h>
#include <gf_audio.h>
#include <gf_lua.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

#define NO_SLEEP
#define FPS 60

void gf_draw_begin(void) { gf_draw_platform_begin(); }

void gf_draw_end(void) { gf_draw_platform_end(); }

gf_draw_t* gf_draw_create(gf_engine_t* engine, const char* title) {
	gf_draw_t* draw = malloc(sizeof(*draw));
	memset(draw, 0, sizeof(*draw));
	draw->engine	   = engine;
	draw->x		   = 0;
	draw->y		   = 0;
	draw->width	   = 800;
	draw->height	   = 600;
	draw->running	   = 0;
	draw->draw_3d	   = 0;
	draw->input	   = NULL;
	draw->clip	   = NULL;
	draw->font	   = NULL;
	draw->fps	   = -1;
	draw->loaded_fonts = NULL;
	strcpy(draw->title, title);
	draw->platform = gf_draw_platform_create(engine, draw);
	if(draw->platform != NULL) {
		draw->driver = gf_draw_driver_create(engine, draw);
		gf_draw_reshape(draw);
		draw->running = 1;

		draw->light[0] = 0.0;
		draw->light[1] = 10.0;
		draw->light[2] = 0.0;
		draw->light[3] = 1.0;

		draw->camera[0] = 0;
		draw->camera[1] = 0;
		draw->camera[2] = 1;

		draw->lookat[0] = 0;
		draw->lookat[1] = 0;
		draw->lookat[2] = 0;

		draw->gui = gf_gui_create(engine, draw);
	} else {
		gf_draw_destroy(draw);
		draw = NULL;
	}
	return draw;
}

void gf_draw_reshape(gf_draw_t* draw) { gf_draw_driver_reshape(draw); }

/* Runs every frame */
void gf_draw_frame(gf_draw_t* draw) {
	gf_graphic_clear(draw);
	gf_lua_step(draw->engine->lua);
	gf_gui_render(draw->gui);
}

void gf_draw_time(gf_draw_time_t* dtime) {
#ifdef GF_DRAW_USE_CLOCK
	*dtime = clock();
#else
#ifdef _WIN32
	/* hpc_freq is set at gf_engine_begin */
	extern LARGE_INTEGER hpc_freq;
	if(hpc_freq.QuadPart > 0) {
		LARGE_INTEGER tick;
		QueryPerformanceCounter(&tick);
		*dtime = tick.QuadPart / (hpc_freq.QuadPart / 1000);
		/* divide by freq for seconds, by 1000 more for ms */
	} else {
		/* system has no hpc hw support */
		*dtime = timeGetTime();
	}
#else
	gettimeofday(dtime, NULL);
#endif
#endif
}

double gf_draw_time_number(gf_draw_time_t* dtime) {
	double r = 0;
#ifdef GF_DRAW_USE_CLOCK
	r += (double)(*dtime) / (CLOCKS_PER_SEC / 1000);
#else
#ifdef _WIN32
	r += (double)(*dtime);
#else
	r += (double)dtime->tv_sec * 1000.0;
	r += (double)dtime->tv_usec / 1000.0;
#endif
#endif
	return r;
}

int gf_draw_step(gf_draw_t* draw) {
	int    ret = 0;
	double delta;
#ifdef NO_SLEEP
	gf_draw_time_t tm;
	if(draw->fps == -1) {
		draw->fps = FPS;
		gf_draw_time(&draw->last_draw);
	}
	gf_draw_time(&tm);
	delta = gf_draw_time_number(&tm) - gf_draw_time_number(&draw->last_draw);
	if(delta > 1000.0 / FPS) {
		draw->fps += 1000.0 / delta;
		draw->fps	= draw->fps / 2;
		ret		= gf_draw_platform_step(draw);
		draw->last_draw = tm;
	}
#else
	/* TODO: Implement this */
#endif
	if(ret != 0) return ret;
	if(draw->close == 1 && draw->engine->lua != NULL) {
		draw->close = 0;
		gf_lua_close(draw->engine->lua);
	}
	return draw->close;
}

void gf_draw_destroy(gf_draw_t* draw) {
	while(arrlen(draw->loaded_fonts) > 0) {
		gf_font_destroy(draw->loaded_fonts[0]);
	}
	if(draw->clip != NULL) {
		arrfree(draw->clip);
		draw->clip = NULL;
	}
	if(draw->driver != NULL) gf_draw_driver_destroy(draw->driver);
	if(draw->platform != NULL) gf_draw_platform_destroy(draw->platform);
	gf_log_function(draw->engine, "Destroyed drawing interface", "");
	free(draw);
}

void gf_draw_set_input(gf_draw_t* draw, gf_input_t* input) { draw->input = input; }

double gf_draw_get_fps(gf_draw_t* draw) { return draw->fps; }
