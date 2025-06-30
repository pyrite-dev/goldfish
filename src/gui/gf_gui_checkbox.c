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
 * File Name:    gf_gui_checkbox.c
 * Author:       Nishi
 *
 * Functionality:
 * GUI checkbox code.
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

gf_gui_id_t gf_gui_create_checkbox(gf_gui_t* gui, double x, double y, double w, double h) {
	gf_gui_component_t* c = malloc(sizeof(*c));

	gf_gui_create_component(gui, c, x, y, w, h);

	c->type = GF_GUI_CHECKBOX;

	c->font.r -= gf_gui_border_color_diff * 2;
	c->font.g += gf_gui_border_color_diff * 2;
	c->font.b -= gf_gui_border_color_diff * 2;

	arrput(gui->area, c);

	return c->key;
}

void gf_gui_checkbox_render(gf_gui_t* gui, gf_gui_component_t* c) {
	double cx;
	double cy;
	double cw;
	double ch;
	int    v;
	if(c->type != GF_GUI_CHECKBOX) return;

	gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
	gf_graphic_clip_pop(gui->draw);

	gf_gui_draw_box(gui, GF_GUI_INVERT, cx, cy, cw, ch);

	if((v = gf_prop_get_integer(&c->prop, "checked")) != GF_PROP_NO_SUCH && v) {
		gf_graphic_fill_polygon(gui->draw, c->font, GF_GRAPHIC_2D, 6, /**/
					cx + cw / 5 * 2, cy + ch / 5 * 3,     /**/
					cx + cw / 5, cy + ch / 5 * 2,	      /**/
					cx + cw / 5, cy + ch / 5 * 3,	      /**/
					cx + cw / 5 * 2, cy + ch / 5 * 4,     /**/
					cx + cw / 5 * 4, cy + ch / 5 * 2,     /**/
					cx + cw / 5 * 4, cy + ch / 5	      /**/
		);
	}
}

void gf_gui_checkbox_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_CHECKBOX) return;
}

void gf_gui_checkbox_click(gf_gui_t* gui, gf_gui_component_t* c) {
	int v;
	if(c->type != GF_GUI_CHECKBOX) return;

	if((v = gf_prop_get_integer(&c->prop, "checked")) != GF_PROP_NO_SUCH && v) {
		gf_prop_set_integer(&c->prop, "checked", 0);
	} else {
		gf_prop_set_integer(&c->prop, "checked", 1);
	}
}
