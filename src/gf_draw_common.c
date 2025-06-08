#define GF_EXPOSE_DRAW
#define GF_EXPOSE_CORE
#define GF_EXPOSE_CLIENT
#define GF_EXPOSE_INPUT
#define GF_EXPOSE_GUI
#define GF_EXPOSE_TEXTURE

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>
#ifdef GF_USE_GLOAD
#define GLOAD_IMPLEMENTATION
#include <gf_gload.h>
#endif

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
#include <gf_prop.h>
#include <gf_image.h>

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
	draw->width	   = gf_prop_get_integer(&engine->config, "width");
	draw->height	   = gf_prop_get_integer(&engine->config, "height");
	draw->running	   = 0;
	draw->draw_3d	   = 0;
	draw->input	   = NULL;
	draw->clip	   = NULL;
	draw->font	   = NULL;
	draw->fps	   = -1;
	draw->loaded_fonts = NULL;
	draw->cursor	   = 1;
	draw->skybox	   = NULL;

	draw->background.r = 0x00;
	draw->background.g = 0x11;
	draw->background.b = 0x11;
	draw->background.a = 0xff;

#ifdef GF_USE_GLOAD
	gload_init();
#endif

	strcpy(draw->title, title);
	draw->platform = gf_draw_platform_create(engine, draw);
	if(draw->platform != NULL) {
#ifndef OLD_CURSOR
		unsigned char* d;
		int	       w;
		int	       h;
#endif
		draw->driver = gf_draw_driver_create(engine, draw);
		gf_draw_reshape(draw);
		draw->running = 1;

		draw->light[0] = 0.0;
		draw->light[1] = 10.0;
		draw->light[2] = 0.0;
		draw->light[3] = 1.0;

		draw->camera[0] = 0;
		draw->camera[1] = 0;
		draw->camera[2] = 0;

		draw->lookat[0] = 0;
		draw->lookat[1] = 0;
		draw->lookat[2] = 1;

		draw->gui = gf_gui_create(engine, draw);

#ifndef OLD_CURSOR
		draw->cursor_texture = NULL;
		d		     = gf_image_load(engine, "base:/cursor.svg", &w, &h);
		if(d == NULL) d = gf_image_load(engine, "base:/cursor.png", &w, &h);
		if(d != NULL) {
			draw->cursor_texture = gf_texture_create(draw, w, h, d);
			free(d);
		}
#endif
	} else {
		gf_draw_destroy(draw);
		draw = NULL;
	}

	return draw;
}

void gf_draw_reshape(gf_draw_t* draw) { gf_draw_driver_reshape(draw); }

#ifdef OLD_CURSOR
/* offsets to use when calculating the mouse cursor shape */
#define MOUSE_OFFSETS_NUM 8
#define CURSOR_MOUSE_OFFSETS_NUM 8
const double mouse_offsets[MOUSE_OFFSETS_NUM][2] = {
    {0, 0},    /**/
    {0, 16},   /**/
    {2, 12},   /**/
    {4, 8.5},  /**/
    {6.5, 6},  /**/
    {9, 3.5},  /**/
    {12, 1.5}, /**/
    {16, 0},   /**/
};
const double outline_mouse_offsets[CURSOR_MOUSE_OFFSETS_NUM][2] = {
    {-2, -2}, /**/
    {-2, 18}, /**/
    {1, 13},  /**/
    {3, 9},   /**/
    {7, 5},   /**/
    {10, 2},  /**/
    {14, 1},  /**/
    {18, -2}, /**/
};
#endif

void gf_draw_cursor(gf_draw_t* draw) {
	if(draw->cursor) {
		gf_graphic_color_t col = draw->gui->font;
#ifdef OLD_CURSOR
		gf_graphic_color_t outline_col;
		double		   coords[2 * MOUSE_OFFSETS_NUM];
		double		   outline_coords[2 * CURSOR_MOUSE_OFFSETS_NUM];
		int		   i;

		outline_col.r = 0;
		outline_col.g = 0;
		outline_col.b = 0;
		outline_col.a = 255;

		for(i = 0; i < MOUSE_OFFSETS_NUM; i++) {
			coords[2 * i + 0] = draw->input->mouse_x + mouse_offsets[i][0];
			coords[2 * i + 1] = draw->input->mouse_y + mouse_offsets[i][1];
		}
		for(i = 0; i < CURSOR_MOUSE_OFFSETS_NUM; i++) {
			outline_coords[2 * i + 0] = draw->input->mouse_x + outline_mouse_offsets[i][0] + 1.5;
			outline_coords[2 * i + 1] = draw->input->mouse_y + outline_mouse_offsets[i][1] + 1.5;
		}

		gf_graphic_fill_polygon_arr(draw, outline_col, GF_GRAPHIC_2D, sizeof(outline_coords) / sizeof(outline_coords[0]) / 2, &outline_coords[0]);
		gf_graphic_fill_polygon_arr(draw, col, GF_GRAPHIC_2D, sizeof(coords) / sizeof(coords[0]) / 2, &coords[0]);

		/* gf_graphic_lines_arr(draw, colInv, GF_GRAPHIC_2D, sizeof(coords) / sizeof(coords[0]) / 2, &coords[0]); */
#else
		if(draw->cursor_texture != NULL) {
			double z = draw->cursor_texture->height;
			z	 = 20 / z;
			gf_graphic_draw_texture_2d(draw, draw->input->mouse_x, draw->input->mouse_y, (double)draw->cursor_texture->width * z, (double)draw->cursor_texture->height * z, draw->cursor_texture, col);
		}
#endif
	}
}

/* Runs every frame */
void gf_draw_frame(gf_draw_t* draw) {
	gf_lua_step(draw->engine->lua);
	gf_gui_render(draw->gui);
	gf_draw_cursor(draw);
	if(arrlen(draw->input->key_queue) > 0) {
		arrdel(draw->input->key_queue, 0);
	}
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
#ifdef GF_DO_SWAP_INTERVAL
	gf_draw_time_t tm;
	gf_draw_time(&tm);
	ret = gf_draw_platform_step(draw);
	gf_draw_time(&draw->last_draw);
	delta = gf_draw_time_number(&draw->last_draw) - gf_draw_time_number(&tm);
	draw->fps += 1000.0 / delta;
	draw->fps = draw->fps / 2;
#else
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
