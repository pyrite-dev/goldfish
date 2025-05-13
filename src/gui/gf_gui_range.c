#define GF_EXPOSE_GUI
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_INPUT

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>

/* Interface */
#include <gf_gui.h>

/* Engine */
#include <gf_graphic.h>
#include <gf_prop.h>
#include <gf_math.h>

/* Standard */
#include <stdlib.h>

#define RANGE_GRAB_HEIGHT 20
#define LINE_HEIGHT 8

extern const double gf_gui_border_width;
extern const int    gf_gui_border_color_diff;

gf_gui_id_t gf_gui_create_range(gf_gui_t* gui, double x, double y, double w, double h) {
	gf_gui_component_t* c = malloc(sizeof(*c));
	gf_gui_id_t	    gr;

	if(h < RANGE_GRAB_HEIGHT + LINE_HEIGHT / 2) {
		h = RANGE_GRAB_HEIGHT + LINE_HEIGHT / 2;
	}

	gf_gui_create_component(gui, c, x, y, w, h);

	c->type = GF_GUI_RANGE;

	gf_prop_set_floating(&c->prop, "value", 0);
	gf_prop_set_floating(&c->prop, "min-value", 0);
	gf_prop_set_floating(&c->prop, "max-value", 100);

	arrput(gui->area, c);

	gr = gf_gui_create_button(gui, 0, 0, RANGE_GRAB_HEIGHT / 2, RANGE_GRAB_HEIGHT);

	gf_gui_set_parent(gui, gr, c->key);

	gf_gui_set_prop_id(gui, c->key, "grab", gr);

	gf_prop_set_integer(gf_gui_get_prop(gui, gr), "no-sound", 1);
	gf_prop_set_integer(gf_gui_get_prop(gui, gr), "grab", 1);

	gf_gui_sort_component(gui);

	return c->key;
}

void gf_gui_range_render(gf_gui_t* gui, gf_gui_component_t* c) {
	gf_input_t*	   input = gui->draw->input;
	double		   cx;
	double		   cy;
	double		   cw;
	double		   ch;
	gf_graphic_color_t col;
	double		   bx;
	double		   by;
	double		   bw;
	double		   bh;
	double		   gw;
	double		   gh;
	double		   lx = 0;
	double		   step;
	double		   min;
	double		   max;
	double		   val;
	gf_gui_id_t	   gr;
	if(c->type != GF_GUI_RANGE) return;

	gr = gf_gui_get_prop_id(gui, c->key, "grab");

	gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
	gf_graphic_clip_pop(gui->draw);

	gf_gui_get_wh(gui, gr, &gw, &gh);

	col = gui->base;
	col.r -= gf_gui_border_color_diff * 2;
	col.g -= gf_gui_border_color_diff * 2;
	col.b -= gf_gui_border_color_diff * 2;

	bw = cw;
	bh = 4;
	bx = cx;
	by = cy + gh / 2 - bh / 2;

	min = gf_prop_get_floating(&c->prop, "min-value");
	max = gf_prop_get_floating(&c->prop, "max-value");
	val = gf_prop_get_floating(&c->prop, "value");

	gf_gui_draw_box(gui, GF_GUI_INVERT, bx, by, bw, bh);
	gf_graphic_fill_rect(gui->draw, bx + gf_gui_border_width, by + gf_gui_border_width, bw - gf_gui_border_width * 2, bh - gf_gui_border_width * 2, col);
	if((step = gf_prop_get_floating(&c->prop, "step")) != GF_PROP_NO_SUCH && step > 0) {
		double lw = cw - gw;
		double sw = (step / gf_math_abs(min - max)) * lw;
		if(sw >= 1) {
			col = gui->base;
			col.r += gf_gui_border_color_diff;
			col.g += gf_gui_border_color_diff;
			col.b += gf_gui_border_color_diff;
			while(lx <= lw) {
				if(lx > lw) lx = lw;

				gf_graphic_fill_rect(gui->draw, cx + lx + gw / 2, cy + (gh - LINE_HEIGHT / 2), 1, LINE_HEIGHT, col);
				lx += sw;
			}
		}
	}
	gf_gui_set_xy(gui, gr, (cw - gw) * gf_math_abs((val - min) / (max - min)), 0);

	if(gui->pressed == gr) {
		double v   = input->mouse_x - gf_prop_get_integer(gf_gui_get_prop(gui, gr), "diff-x") - cx;
		double d   = 0;
		double old = gf_prop_get_floating(gf_gui_get_prop(gui, c->key), "value");

		d = ((v / (cw - gw)) * gf_math_abs(max - min)) + min;
		if(step != GF_PROP_NO_SUCH) {
			d = gf_math_round(d / step) * step;
		}

		if(d < min) d = min;
		if(d > max) d = max;

		gf_prop_set_floating(&c->prop, "value", d);
		if(old != d && c->callback != NULL) {
			c->callback(gui->engine, gui->draw, c->key, GF_GUI_CHANGE_EVENT);
		}
	}
}

void gf_gui_range_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_RANGE) return;
}

void gf_gui_range_click(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_RANGE) return;
}
