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

void gf_draw_begin(void) { gf_draw_platform_begin(); }

void gf_draw_end(void) { gf_draw_platform_end(); }

gf_draw_t* gf_draw_create(gf_engine_t* engine, const char* title) {
	gf_draw_t* draw = malloc(sizeof(*draw));
	memset(draw, 0, sizeof(*draw));
	draw->engine  = engine;
	draw->x	      = 0;
	draw->y	      = 0;
	draw->width   = 800;
	draw->height  = 600;
	draw->running = 0;
	draw->draw_3d = 0;
	draw->input   = NULL;
	draw->clip    = NULL;
	draw->font    = NULL;
	draw->fps     = -1;
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
	*dtime = GetTickCount();
#else
	gettimeofday(dtime, NULL);
#endif
#endif
}

double gf_draw_time_number(gf_draw_time_t* dtime) {
	double r = 0;
#ifdef GF_DRAW_USE_CLOCK
	r += (double)(*dtime) / CLOCKS_PER_SEC;
#else
#ifdef _WIN32
	r += (double)(*dtime) / 1000.0;
#else
	r += (double)dtime->tv_sec;
	r += (double)dtime->tv_usec / 1000000.0;
#endif
#endif
	return r;
}

int gf_draw_step(gf_draw_t* draw) {
	int ret;
	if(draw->fps != -1) {
		gf_draw_time_t t;
		gf_draw_time(&t);

		draw->fps = 1.0 / (gf_draw_time_number(&t) - gf_draw_time_number(&draw->last_draw));
	}
	gf_draw_time(&draw->last_draw);
	ret = gf_draw_platform_step(draw);
	if(ret != 0) return ret;
	if(draw->close == 1 && draw->engine->lua != NULL) {
		draw->close = 0;
		gf_lua_close(draw->engine->lua);
	}
	if(draw->fps == -1) {
		draw->fps = 0;
	} else {
#ifdef _WIN32
		double msec;
#endif
		double	       sfps = 60;
		gf_draw_time_t t;

#ifdef _WIN32
		gf_draw_time(&t);
		msec = (1000.0 / sfps) - (gf_draw_time_number(&t) - gf_draw_time_number(&draw->last_draw)) * 1000.0;
		if(msec > 0) Sleep((int)msec);
#else
		do {
			gf_draw_time(&t);
		} while((gf_draw_time_number(&t) - gf_draw_time_number(&draw->last_draw)) <= 1.0 / sfps);
#endif
	}

	return draw->close;
}

void gf_draw_destroy(gf_draw_t* draw) {
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
