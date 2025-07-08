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
 * File Name:    gf_gui_scrollbar.c
 * Author:       Nishi
 *
 * Functionality:
 * GUI scrollbar code.
 *
 **********************************************************************************
 * @endif
 */

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

/* Standard */
#include <stdlib.h>

extern const int gf_gui_border_color_diff;

void GFAPI gf_gui_scrollbar_up(gf_engine_t* engine, gf_draw_t* draw, gf_gui_id_t id, int type) {
	double step = gf_prop_get_floating(gf_gui_get_prop(draw->gui, gf_gui_get_parent(draw->gui, id)), "step");
	double lim  = gf_prop_get_floating(gf_gui_get_prop(draw->gui, gf_gui_get_parent(draw->gui, id)), "min-value");
	double val  = gf_prop_get_floating(gf_gui_get_prop(draw->gui, gf_gui_get_parent(draw->gui, id)), "value");
	double old  = val;
	if(type != GF_GUI_PRESS_EVENT) return;
	val -= step / 2;
	if(val < lim) {
		val = lim;
	}
	if(step < lim) {
		val = lim;
	}
	gf_prop_set_floating(gf_gui_get_prop(draw->gui, gf_gui_get_parent(draw->gui, id)), "value", val);
	if(old != val && draw->gui->area[gf_gui_get_index(draw->gui, gf_gui_get_parent(draw->gui, id))]->callback != NULL) {
		draw->gui->area[gf_gui_get_index(draw->gui, gf_gui_get_parent(draw->gui, id))]->callback(engine, draw, gf_gui_get_parent(draw->gui, id), GF_GUI_CHANGE_EVENT);
	}
}

void GFAPI gf_gui_scrollbar_down(gf_engine_t* engine, gf_draw_t* draw, gf_gui_id_t id, int type) {
	double step = gf_prop_get_floating(gf_gui_get_prop(draw->gui, gf_gui_get_parent(draw->gui, id)), "step");
	double min  = gf_prop_get_floating(gf_gui_get_prop(draw->gui, gf_gui_get_parent(draw->gui, id)), "min-value");
	double lim  = gf_prop_get_floating(gf_gui_get_prop(draw->gui, gf_gui_get_parent(draw->gui, id)), "max-value");
	double val  = gf_prop_get_floating(gf_gui_get_prop(draw->gui, gf_gui_get_parent(draw->gui, id)), "value");
	double old  = val;
	if(type != GF_GUI_PRESS_EVENT) return;
	val += step / 2;
	if(val > (lim - step)) {
		val = lim - step;
	}
	if(step > lim) {
		val = min;
	}
	gf_prop_set_floating(gf_gui_get_prop(draw->gui, gf_gui_get_parent(draw->gui, id)), "value", val);
	if(old != val && draw->gui->area[gf_gui_get_index(draw->gui, gf_gui_get_parent(draw->gui, id))]->callback != NULL) {
		draw->gui->area[gf_gui_get_index(draw->gui, gf_gui_get_parent(draw->gui, id))]->callback(engine, draw, gf_gui_get_parent(draw->gui, id), GF_GUI_CHANGE_EVENT);
	}
}

gf_gui_id_t gf_gui_create_scrollbar(gf_gui_t* gui, double x, double y, double w, double h) {
	gf_gui_component_t* c = malloc(sizeof(*c));
	gf_gui_id_t	    up;
	gf_gui_id_t	    dn;
	gf_gui_id_t	    gr;

	gf_gui_create_component(gui, c, x, y, w, h);

	c->type = GF_GUI_SCROLLBAR;

	arrput(gui->area, c);

	up = gf_gui_create_button(gui, 0, 0, w, w);
	dn = gf_gui_create_button(gui, 0, 0, w, w);
	gr = gf_gui_create_button(gui, 0, w, w, w * 4);

	gf_gui_set_text(gui, up, "#TriangleUp");
	gf_gui_set_text(gui, dn, "#TriangleDown");

	gf_prop_set_integer(gf_gui_get_prop(gui, dn), "y-base", 1);

	gf_gui_set_parent(gui, up, c->key);
	gf_gui_set_parent(gui, dn, c->key);
	gf_gui_set_parent(gui, gr, c->key);

	gf_prop_set_floating(gf_gui_get_prop(gui, c->key), "min-value", 0);
	gf_prop_set_floating(gf_gui_get_prop(gui, c->key), "step", 10);
	gf_prop_set_floating(gf_gui_get_prop(gui, c->key), "value", 0);
	gf_prop_set_floating(gf_gui_get_prop(gui, c->key), "max-value", 100);

	gf_gui_set_prop_id(gui, c->key, "grab", gr);
	gf_gui_set_prop_id(gui, up, "scrollbar", c->key);
	gf_gui_set_prop_id(gui, dn, "scrollbar", c->key);

	gf_prop_set_integer(gf_gui_get_prop(gui, gr), "no-sound", 1);
	gf_prop_set_integer(gf_gui_get_prop(gui, gr), "grab", 1);

	gf_gui_set_callback(gui, up, gf_gui_scrollbar_up);
	gf_gui_set_callback(gui, dn, gf_gui_scrollbar_down);

	return c->key;
}

void gf_gui_scrollbar_render(gf_gui_t* gui, gf_gui_component_t* c) {
	gf_input_t*	   input = gui->draw->input;
	double		   cx;
	double		   cy;
	double		   cw;
	double		   ch;
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
		int    v   = input->mouse_y - gf_prop_get_integer(gf_gui_get_prop(gui, gr), "diff-y") - (cy + cw);
		double d   = min + (max - min) / (ch - cw * 2) * v;
		double old = gf_prop_get_floating(gf_gui_get_prop(gui, c->key), "value");

		if(d < min) {
			d = min;
		} else if(d > (max - step)) {
			d = max - step;
		}
		gf_prop_set_floating(gf_gui_get_prop(gui, c->key), "value", d);
		if(old != d && c->callback != NULL) {
			c->callback(gui->engine, gui->draw, c->key, GF_GUI_CHANGE_EVENT);
		}
	}
}

void gf_gui_scrollbar_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_SCROLLBAR) return;
}

void gf_gui_scrollbar_click(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_SCROLLBAR) return;
}
