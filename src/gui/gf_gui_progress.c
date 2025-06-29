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
 * File Name:    gf_gui_progress.c
 * Author:       Nishi
 *
 * Functionality:
 * GUI progress code.
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
#include <gf_math.h>

/* Standard */
#include <stdlib.h>

extern const double gf_gui_border_width;
extern const int    gf_gui_border_color_diff;

gf_gui_id_t gf_gui_create_progress(gf_gui_t* gui, double x, double y, double w, double h) {
	gf_gui_component_t* c = malloc(sizeof(*c));

	gf_gui_create_component(gui, c, x, y, w, h);

	c->type = GF_GUI_PROGRESS;

	gf_prop_set_floating(&c->prop, "value", 0);
	gf_prop_set_floating(&c->prop, "min-value", 0);
	gf_prop_set_floating(&c->prop, "max-value", 100);
	gf_prop_set_integer(&c->prop, "split", 10);

	c->font.r -= gf_gui_border_color_diff * 2;
	c->font.g += gf_gui_border_color_diff * 2;
	c->font.b -= gf_gui_border_color_diff * 2;

	arrput(gui->area, c);

	return c->key;
}

void gf_gui_progress_render(gf_gui_t* gui, gf_gui_component_t* c) {
	double		   cx;
	double		   cy;
	double		   cw;
	double		   ch;
	double		   x;
	double		   rem;
	double		   bw;
	double		   bh;
	double		   bsw;
	double		   sw;
	double		   min;
	double		   max;
	double		   val;
	int		   split;
	gf_graphic_color_t col;
	if(c->type != GF_GUI_PROGRESS) return;

	gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
	gf_graphic_clip_pop(gui->draw);

	gf_gui_draw_box(gui, GF_GUI_INVERT, cx, cy, cw, ch);

	col = gui->base;
	col.r -= gf_gui_border_color_diff / 2;
	col.g -= gf_gui_border_color_diff / 2;
	col.b -= gf_gui_border_color_diff / 2;
	gf_graphic_fill_rect(gui->draw, cx + gf_gui_border_width, cy + gf_gui_border_width, cw - gf_gui_border_width * 2, ch - gf_gui_border_width * 2, col);

	col = c->font;

	min   = gf_prop_get_floating(&c->prop, "min-value");
	max   = gf_prop_get_floating(&c->prop, "max-value");
	val   = gf_prop_get_floating(&c->prop, "value");
	split = gf_prop_get_integer(&c->prop, "split");

	sw = gf_gui_border_width * 2;
	bh = (ch - gf_gui_border_width * 2) - sw * 2;
	bw = (cw - sw - gf_gui_border_width * 2) / split - sw;

	/* space+box size */
	bsw = sw + bw;

	rem = ((val - min) / (max - min)) * (cw - sw - gf_gui_border_width * 2 - sw * split);
	x   = cx + gf_gui_border_width;
	while(rem > 0) {
		x += sw;

		gf_graphic_fill_rect(gui->draw, x, cy + gf_gui_border_width + sw, (bw > rem ? rem : bw), bh, col);

		x += bw;
		rem -= bw;
	}
}

void gf_gui_progress_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_PROGRESS) return;
}

void gf_gui_progress_click(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_PROGRESS) return;
}
