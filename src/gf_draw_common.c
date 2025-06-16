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
#include <gf_type/compat.h>
#include <gf_assert.h>
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
#include <gf_input.h>
#include <gf_action.h>
#include <gf_command.h>

/* Standard */
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NO_SLEEP
#define FPS 60

void gf_draw_begin(void) { gf_draw_platform_begin(); }

void gf_draw_end(void) { gf_draw_platform_end(); }

void console_submit(gf_engine_t* engine, gf_draw_t* draw, gf_gui_id_t id, int type) {
	if(type == GF_GUI_PRESS_EVENT || type == GF_GUI_ACTIVATE_EVENT) {
		gf_gui_id_t entry = gf_gui_get_prop_id(draw->gui, draw->console, "entry");
		char*	    list[1];

		list[0] = (char*)gf_gui_get_text(draw->gui, entry);
		gf_command_run(draw->engine, &list[0], 1);

		gf_gui_set_text(draw->gui, entry, "");
	}
}

void console_close(gf_engine_t* engine, gf_draw_t* draw, gf_gui_id_t id, int type) {
	if(type == GF_GUI_PRESS_EVENT) {
		gf_gui_id_t p = gf_gui_get_parent(draw->gui, id);
		gf_prop_set_integer(gf_gui_get_prop(draw->gui, p), "hide", 1);
	}
}

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

	draw->intro.finished = 0;
	draw->intro.frame    = 0;

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

	if(draw != NULL) {
		gf_gui_id_t fr;
		gf_gui_id_t console;
		gf_gui_id_t button;
		gf_gui_id_t entry;
		gf_gui_id_t cb;
		double	    w;
		double	    h;

		draw->console = gf_gui_create_window(draw->gui, 0, 0, 600, 450);
		cb	      = gf_gui_get_prop_id(draw->gui, draw->console, "close-button");
		gf_prop_delete(gf_gui_get_prop(draw->gui, cb), "close-parent");
		gf_gui_set_callback(draw->gui, cb, console_close);

		fr = gf_gui_get_prop_id(draw->gui, draw->console, "frame");
		gf_gui_set_text(draw->gui, draw->console, "Console");

		gf_gui_get_wh(draw->gui, fr, &w, &h);

		console = gf_gui_create_text(draw->gui, 0, 0, w, h - 25);
		gf_gui_set_parent(draw->gui, console, fr);

		entry = gf_gui_create_entry(draw->gui, 0, 0, w - 60 - 5, 20);
		gf_prop_set_integer(gf_gui_get_prop(draw->gui, entry), "y-base", 1);
		gf_gui_set_parent(draw->gui, entry, fr);
		gf_gui_set_callback(draw->gui, entry, console_submit);

		button = gf_gui_create_button(draw->gui, 0, 0, 60, 20);
		gf_gui_set_text(draw->gui, button, "Submit");
		gf_prop_set_integer(gf_gui_get_prop(draw->gui, button), "x-base", 1);
		gf_prop_set_integer(gf_gui_get_prop(draw->gui, button), "y-base", 1);
		gf_gui_set_parent(draw->gui, button, fr);
		gf_gui_set_callback(draw->gui, button, console_submit);

		gf_gui_set_prop_id(draw->gui, draw->console, "console", console);
		gf_gui_set_prop_id(draw->gui, draw->console, "entry", entry);
		gf_prop_set_floating(gf_gui_get_prop(draw->gui, entry), "font-size", 14);
		gf_prop_set_floating(gf_gui_get_prop(draw->gui, console), "font-size", 12);

		gf_prop_set_integer(gf_gui_get_prop(draw->gui, draw->console), "hide", 1);
		gf_gui_sort_component(draw->gui);
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
	gf_gui_id_t console = gf_gui_get_prop_id(draw->gui, draw->console, "console");
	gf_gui_id_t scrollbar = gf_gui_get_prop_id(draw->gui, console, "scrollbar");
	gf_gui_id_t entry   = gf_gui_get_prop_id(draw->gui, draw->console, "entry");
	gf_gui_id_t fr	    = gf_gui_get_prop_id(draw->gui, draw->console, "frame");
	double	    w;
	double	    h;

	gf_gui_get_wh(draw->gui, fr, &w, &h);
	gf_gui_set_wh(draw->gui, console, w, h - 25);
	gf_gui_set_wh(draw->gui, entry, w - 60 - 5, 20);

	// Keep the console scrolled down
	float scroll_value = gf_prop_get_floating(gf_gui_get_prop(draw->gui, scrollbar), "value");
	float max = gf_prop_get_floating(gf_gui_get_prop(draw->gui, scrollbar), "max-value") - gf_prop_get_floating(gf_gui_get_prop(draw->gui, scrollbar), "step");
	gf_bool_t keep_scrolled = scroll_value >= max - 20.f;

	gf_gui_set_text(draw->gui, console, draw->engine->log_list);
	if(draw->monospace_font != NULL) {
		gf_prop_set_ptr_keep(gf_gui_get_prop(draw->gui, console), "font", draw->monospace_font);
		gf_prop_set_ptr_keep(gf_gui_get_prop(draw->gui, entry), "font", draw->monospace_font);
	}

	if(draw->intro.finished == 0) {
		if(draw->intro.powered != NULL) {
			const char*	   lines[] = {"(C) 2025 Pyrite Development Team. All rights reserved.", "This product includes software developed by the Pyrite Development Team."};
			int		   i;
			double		   mw = 0;
			double		   mh = 0;
			double		   y;
			double		   sec = draw->intro.frame;
			gf_graphic_color_t col;
			double		   fsz = 21;

			sec = sec / gf_draw_get_fps(draw);

			col.r = 0;
			col.g = 0;
			col.b = 0;
			col.a = 255;
			gf_graphic_fill_rect(draw, 0, 0, draw->width, draw->height, col);

			col.r = 255;
			col.g = 255;
			col.b = 255;
			col.a = 0;
			if(sec <= 0.2) {
				col.a = sqrt(sec * (1.0 / 0.2)) * 255;
			} else if(sec > 0.2 && sec <= 0.2 + 1) {
				col.a = 255;
			} else if(sec > 0.2 + 1 && sec <= 0.2 + 1 + 0.2) {
				double v = 0.2 - (sec - 0.2 - 1);
				col.a	 = sqrt(v * (1.0 / 0.2)) * 255;
			} else if(sec > 0.2 + 1 + 0.2 && sec <= 0.2 + 1 + 0.3) {
			} else {
				draw->intro.finished = 1;
			}
			gf_graphic_draw_texture_2d(draw, draw->width / 2 - draw->intro.powered->width / 2, draw->height / 2 - draw->intro.powered->height / 2, draw->intro.powered->width, draw->intro.powered->height, draw->intro.powered, col);

			col.r = 192;
			col.g = 192;
			col.b = 192;

			for(i = 0; i < sizeof(lines) / sizeof(lines[0]); i++) {
				double tw = gf_graphic_text_width(draw, draw->font, fsz, lines[i]);
				mh += gf_graphic_text_height(draw, draw->font, fsz, lines[i]);
				if(tw > mw) mw = tw;
			}

			y = draw->height / 2 + draw->intro.powered->height / 2 + (draw->height / 2 - draw->intro.powered->height / 2) / 2 - mh / 2;
			for(i = 0; i < sizeof(lines) / sizeof(lines[0]); i++) {
				gf_graphic_text(draw, draw->font, draw->width / 2 - mw / 2, y, fsz, lines[i], col);
				y += gf_graphic_text_height(draw, draw->font, fsz, lines[i]);
			}

			draw->intro.frame++;
		} else {
			int	       w;
			int	       h;
			unsigned char* d = gf_image_load_memory(draw->engine, gf_powered, gf_powered_len, &w, &h);
			gf_assert(draw->engine, d != NULL);
			draw->intro.powered = gf_texture_create(draw, w, h, d);
		}
		if(gf_input_key_pressed(draw->input, GF_INPUT_KEY_ESCAPE)) {
			draw->intro.finished = 1;
		}
	} else {
		gf_lua_step(draw->engine->lua);
		gf_gui_render(draw->gui);
		gf_draw_cursor(draw);
	}
	gf_action_process(draw->engine, draw->input);
	if(arrlen(draw->input->key_queue) > 0) {
		arrdel(draw->input->key_queue, 0);
	}
	if(arrlen(draw->input->key_rqueue) > 0) {
		arrdel(draw->input->key_rqueue, 0);
	}

	// Scroll the console down
	if (keep_scrolled) {
		gf_prop_set_floating(gf_gui_get_prop(draw->gui, scrollbar), "value", gf_prop_get_floating(gf_gui_get_prop(draw->gui, scrollbar), "max-value") - gf_prop_get_floating(gf_gui_get_prop(draw->gui, scrollbar), "step"));
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
	if(draw->intro.powered != NULL) {
		gf_texture_destroy(draw->intro.powered);
	}
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
