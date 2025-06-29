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
 * File Name:    gf_gui_text.c
 * Author:       Nishi
 *
 * Functionality:
 * GUi text code.
 *
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_GUI
#define GF_EXPOSE_DRAW

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>

/* Interface */
#include <gf_gui.h>

/* Engine */
#include <gf_graphic.h>
#include <gf_font.h>
#include <gf_prop.h>

/* Standard */
#include <stdlib.h>

extern const double gf_gui_border_width;
extern const int    gf_gui_border_color_diff;

gf_gui_id_t gf_gui_create_text(gf_gui_t* gui, double x, double y, double w, double h) {
	gf_gui_component_t* c = malloc(sizeof(*c));
	gf_gui_id_t	    scroll;

	gf_gui_create_component(gui, c, x, y, w, h);

	c->type = GF_GUI_TEXT;

	arrput(gui->area, c);

	scroll = gf_gui_create_scrollbar(gui, gf_gui_border_width, gf_gui_border_width, 20 - gf_gui_border_width, h - gf_gui_border_width * 2);
	gf_prop_set_integer(gf_gui_get_prop(gui, scroll), "x-base", 1);
	gf_gui_set_parent(gui, scroll, c->key);
	gf_gui_set_prop_id(gui, c->key, "scrollbar", scroll);

	gf_gui_sort_component(gui);

	return c->key;
}

void gf_gui_text_render(gf_gui_t* gui, gf_gui_component_t* c) {
	double		   cx;
	double		   cy;
	double		   cw;
	double		   ch;
	gf_font_t*	   font;
	gf_gui_id_t	   scroll;
	gf_graphic_color_t col;
	if(c->type != GF_GUI_TEXT) return;

	font = gf_prop_get_ptr_keep(&c->prop, "font");
	if(font == NULL) font = gui->draw->font;
	if(font == NULL) return;

	gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
	gf_graphic_clip_pop(gui->draw);

	gf_gui_draw_box(gui, GF_GUI_INVERT, cx, cy, cw, ch);

	col = gui->base;
	col.r -= gf_gui_border_color_diff / 2;
	col.g -= gf_gui_border_color_diff / 2;
	col.b -= gf_gui_border_color_diff / 2;
	gf_graphic_fill_rect(gui->draw, cx + gf_gui_border_width, cy + gf_gui_border_width, cw - gf_gui_border_width * 2, ch - gf_gui_border_width * 2, col);

	scroll = gf_gui_get_prop_id(gui, c->key, "scrollbar");
	gf_gui_set_wh(gui, scroll, 20 - gf_gui_border_width, c->height - gf_gui_border_width * 2);
	gf_prop_set_floating(gf_gui_get_prop(gui, scroll), "step", c->height - gf_gui_border_width * 2);
	if(c->text != NULL) {
		double ax = cx + gf_gui_border_width;
		double ay = cy + gf_gui_border_width;
		double aw = cw - 20 - gf_gui_border_width * 3;
		double ah = ch - gf_gui_border_width * 2;
		double sy = gf_prop_get_floating(gf_gui_get_prop(gui, scroll), "value");

		double propf;
		double fsz = GF_GUI_SMALL_FONT_SIZE;
		if((propf = gf_prop_get_floating(&c->prop, "font-size")) != GF_PROP_NO_SUCH) {
			fsz = propf;
		}

		gf_graphic_clip_push(gui->draw, ax, ay, aw, ah);
		gf_prop_set_floating(gf_gui_get_prop(gui, scroll), "max-value", gf_graphic_text_wrap(gui->draw, font, ax, ay - sy, aw, fsz, c->text, c->font));
		gf_graphic_clip_pop(gui->draw);
	}
}

void gf_gui_text_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_TEXT) return;
}

void gf_gui_text_click(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_TEXT) return;
}
