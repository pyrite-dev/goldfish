#define GF_EXPOSE_GUI
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_INPUT

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>

/* Interface */
#include <gf_gui.h>
#include <gf_graphic.h>

/* Engine */
#include <gf_prop.h>

/* Standard */

extern const int gf_gui_border_color_diff;

void gf_gui_scrollbar_up(gf_engine_t* engine, gf_draw_t* draw, gf_gui_id_t id, int type) {
	double step = gf_prop_get_floating(gf_gui_get_prop(draw->gui, gf_gui_get_parent(draw->gui, id)), "step");
	double lim  = gf_prop_get_floating(gf_gui_get_prop(draw->gui, gf_gui_get_parent(draw->gui, id)), "min-value");
	double val  = gf_prop_get_floating(gf_gui_get_prop(draw->gui, gf_gui_get_parent(draw->gui, id)), "value");
	val -= step;
	if(val < lim) {
		val = lim;
	}
	if(step < lim) {
		val = 0;
	}
	gf_prop_set_floating(gf_gui_get_prop(draw->gui, gf_gui_get_parent(draw->gui, id)), "value", val);
}

void gf_gui_scrollbar_down(gf_engine_t* engine, gf_draw_t* draw, gf_gui_id_t id, int type) {
	double step = gf_prop_get_floating(gf_gui_get_prop(draw->gui, gf_gui_get_parent(draw->gui, id)), "step");
	double lim  = gf_prop_get_floating(gf_gui_get_prop(draw->gui, gf_gui_get_parent(draw->gui, id)), "max-value");
	double val  = gf_prop_get_floating(gf_gui_get_prop(draw->gui, gf_gui_get_parent(draw->gui, id)), "value");
	val += step;
	if(val > (lim - step)) {
		val = lim - step;
	}
	if(step > lim) {
		val = 0;
	}
	gf_prop_set_floating(gf_gui_get_prop(draw->gui, gf_gui_get_parent(draw->gui, id)), "value", val);
}

gf_gui_id_t gf_gui_create_scrollbar(gf_gui_t* gui, double x, double y, double w, double h) {
	gf_gui_component_t c;
	gf_gui_id_t	   up;
	gf_gui_id_t	   dn;
	gf_gui_id_t	   gr;

	gf_gui_create_component(gui, &c, x, y, w, h);

	c.type = GF_GUI_SCROLLBAR;

	hmputs(gui->area, c);

	up = gf_gui_create_button(gui, 0, 0, w, w);
	dn = gf_gui_create_button(gui, 0, 0, w, w);
	gr = gf_gui_create_button(gui, 0, w, w, w * 4);

	gf_gui_set_text(gui, up, "U");
	gf_gui_set_text(gui, dn, "D");

	gf_prop_set_integer(gf_gui_get_prop(gui, dn), "y-base", 1);

	gf_gui_set_parent(gui, up, c.key);
	gf_gui_set_parent(gui, dn, c.key);
	gf_gui_set_parent(gui, gr, c.key);

	gf_prop_set_floating(gf_gui_get_prop(gui, c.key), "min-value", 0);
	gf_prop_set_floating(gf_gui_get_prop(gui, c.key), "step", 10);
	gf_prop_set_floating(gf_gui_get_prop(gui, c.key), "value", 0);
	gf_prop_set_floating(gf_gui_get_prop(gui, c.key), "max-value", 100);

	gf_gui_set_prop_id(gui, c.key, "grab", gr);
	gf_gui_set_prop_id(gui, up, "scrollbar", c.key);
	gf_gui_set_prop_id(gui, dn, "scrollbar", c.key);

	gf_gui_set_callback(gui, up, gf_gui_scrollbar_up);
	gf_gui_set_callback(gui, dn, gf_gui_scrollbar_down);

	return c.key;
}

void gf_gui_scrollbar_render(gf_gui_t* gui, gf_gui_component_t* c) {
	gf_input_t*	   input = gui->draw->input;
	double		   cx;
	double		   cy;
	double		   cw;
	double		   ch;
	int		   prop;
	double		   min;
	double		   max;
	double		   step;
	double		   val;
	double		   steph;
	double		   stepy;
	gf_gui_id_t	   gr;
	gf_graphic_color_t col = gui->base;
	if(c->type != GF_GUI_SCROLLBAR) return;

	gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
	gf_graphic_clip_pop(gui->draw);

	col.r += (double)gf_gui_border_color_diff * 2 / 3;
	col.g += (double)gf_gui_border_color_diff * 2 / 3;
	col.b += (double)gf_gui_border_color_diff * 2 / 3;

	gf_graphic_fill_rect(gui->draw, cx, cy, cw, ch, col);

	min  = gf_prop_get_floating(gf_gui_get_prop(gui, c->key), "min-value");
	max  = gf_prop_get_floating(gf_gui_get_prop(gui, c->key), "max-value");
	step = gf_prop_get_floating(gf_gui_get_prop(gui, c->key), "step");
	val  = gf_prop_get_floating(gf_gui_get_prop(gui, c->key), "value");

	if(step > (max - min)) {
		step = max - min;
	}

	gr = gf_gui_get_prop_id(gui, c->key, "grab");

	steph = (ch - cw * 2) / ((max - min) / step);
	stepy = steph * (val / step);
	if((stepy + steph) > (ch - cw * 2)) {
		stepy = ch - cw * 2 - steph;
	}
	gf_gui_set_wh(gui, gr, cw, steph);
	gf_gui_set_xy(gui, gr, 0, cw + stepy);

	if(gui->pressed == gr) {
		int    v = input->mouse_y - gf_prop_get_integer(gf_gui_get_prop(gui, gr), "diff-y") - (cy + cw);
		double d = min + (max - min) / (ch - cw * 2) * v;

		if(d < min) {
			d = min;
		} else if(d > (max - step)) {
			d = max - step;
		}
		gf_prop_set_floating(gf_gui_get_prop(gui, c->key), "value", d);
	}
}

void gf_gui_scrollbar_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_SCROLLBAR) return;
}

void gf_gui_scrollbar_click(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_SCROLLBAR) return;
}
