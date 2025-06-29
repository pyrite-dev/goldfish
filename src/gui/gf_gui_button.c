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
 * File Name:    gf_gui_button.c
 * Author:       Nishi
 *
 * Functionality:
 * GUI button code.
 *
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_GUI
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_INPUT
#define GF_EXPOSE_CORE
#define GF_EXPOSE_CLIENT

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>

/* Interface */
#include <gf_gui.h>

/* Engine */
#include <gf_prop.h>
#include <gf_graphic.h>
#include <gf_draw.h>
#include <gf_log.h>
#include <gf_audio.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

extern const double gf_gui_border_width;
extern const int    gf_gui_border_color_diff;

gf_gui_id_t gf_gui_create_button(gf_gui_t* gui, double x, double y, double w, double h) {
	gf_gui_component_t* c = malloc(sizeof(*c));

	gf_gui_create_component(gui, c, x, y, w, h);

	c->type = GF_GUI_BUTTON;

	arrput(gui->area, c);

	return c->key;
}

void gf_gui_button_render(gf_gui_t* gui, gf_gui_component_t* c) {
	gf_input_t*	   input = gui->draw->input;
	double		   cx;
	double		   cy;
	double		   cw;
	double		   ch;
	double		   x;
	double		   y;
	gf_prop_integer_t  prop;
	gf_prop_floating_t propf;
	gf_font_t*	   font;
	if(c->type != GF_GUI_BUTTON) return;

	font = gf_prop_get_ptr_keep(&c->prop, "font");
	if(font == NULL) font = gui->draw->font;
	if(font == NULL) return;

	gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
	gf_graphic_clip_pop(gui->draw);

	if((prop = gf_prop_get_integer(&c->prop, "no-border")) == GF_PROP_NO_SUCH || !prop) {
		if((prop = gf_prop_get_integer(&c->prop, "grab")) != GF_PROP_NO_SUCH && prop) {
			gf_gui_draw_box(gui, GF_GUI_NORMAL, cx, cy, cw, ch);
		} else {
			gf_gui_draw_box(gui, (gui->pressed == c->key) ? GF_GUI_INVERT : GF_GUI_NORMAL, cx, cy, cw, ch);
		}
	}

	if(c->text != NULL) {
		double		   fsz = GF_GUI_SMALL_FONT_SIZE;
		double		   ogx;
		double		   ogy;
		double		   sp = 0;
		gf_graphic_color_t col;
		if((propf = gf_prop_get_floating(&c->prop, "font-size")) != GF_PROP_NO_SUCH) {
			fsz = propf;
		}
		if((prop = gf_prop_get_integer(&c->prop, "align")) == GF_PROP_NO_SUCH || prop == 0) {
			x = cx + cw / 2 - gf_graphic_text_width(gui->draw, font, fsz, c->text) / 2;
			y = cy + ch / 2 - gf_graphic_text_height(gui->draw, font, fsz, c->text) / 2;
		} else if(prop == -1) {
			x = cx;
			y = cy + ch / 2 - gf_graphic_text_height(gui->draw, font, fsz, c->text) / 2;
		} else if(prop == 1) {
			x = cx - gf_graphic_text_width(gui->draw, font, fsz, c->text);
			y = cy + ch / 2 - gf_graphic_text_height(gui->draw, font, fsz, c->text) / 2;
		}

		ogx = x;
		ogy = y;
		ogx += gf_gui_border_width / 2;
		ogy += gf_gui_border_width / 2;

		if(gui->pressed == c->key) {
			x += gf_gui_border_width / 2;
			y += gf_gui_border_width / 2;
			sp = gf_gui_border_width / 2;
		}

		if(gui->hover == c->key) {
			col = c->hover_font;
		} else {
			col = c->font;
		}

		gf_graphic_clip_push(gui->draw, cx, cy, cw, ch);
		if(strcmp(c->text, "#TriangleUp") == 0) {
			gf_graphic_fill_polygon(gui->draw, col, GF_GRAPHIC_2D, 3, cx + cw / 2 + sp, cy + ch / 4 + sp, cx + cw / 4 + sp, cy + ch / 4 * 3 + sp, cx + cw / 4 * 3 + sp, cy + ch / 4 * 3 + sp);
		} else if(strcmp(c->text, "#TriangleDown") == 0) {
			gf_graphic_fill_polygon(gui->draw, col, GF_GRAPHIC_2D, 3, cx + cw / 2 + sp, cy + ch / 4 * 3 + sp, cx + cw / 4 * 3 + sp, cy + ch / 4 + sp, cx + cw / 4 + sp, cy + ch / 4 + sp);
		} else if(strcmp(c->text, "#Cross") == 0) {
			double w      = cw / 5 * 3;
			double h      = ch / 5 * 3;
			double lwidth = gf_graphic_get_line_width(gui->draw);
			gf_graphic_set_line_width(gui->draw, 2);
			gf_graphic_lines(gui->draw, col, GF_GRAPHIC_2D, 1, cx + sp + cw / 2 - w / 2, cy + sp + ch / 2 - h / 2, cx + w + sp + cw / 2 - w / 2, cy + h + sp + ch / 2 - h / 2);
			gf_graphic_lines(gui->draw, col, GF_GRAPHIC_2D, 1, cx + sp + cw / 2 - w / 2, cy + h + sp + ch / 2 - h / 2, cx + w + sp + cw / 2 - w / 2, cy + sp + ch / 2 - h / 2);
			gf_graphic_set_line_width(gui->draw, lwidth);
		} else {
			if((prop = gf_prop_get_integer(&c->prop, "no-border")) != GF_PROP_NO_SUCH && prop) {
				gf_graphic_color_t dim;
				dim.r = 0;
				dim.g = 0;
				dim.b = 0;
				dim.a = 128;
				gf_graphic_text(gui->draw, font, ogx, ogy, fsz, c->text, dim);
			}
			gf_graphic_text(gui->draw, font, x, y, fsz, c->text, col);
		}
		gf_graphic_clip_pop(gui->draw);
	}
}

void gf_gui_button_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_BUTTON) return;
}

void gf_gui_button_click(gf_gui_t* gui, gf_gui_component_t* c) {
	gf_input_t*	  input = gui->draw->input;
	gf_prop_integer_t prop;
	if(c->type != GF_GUI_BUTTON) return;
	if(c->callback != NULL) {
		c->callback(gui->engine, gui->draw, gui->pressed, GF_GUI_PRESS_EVENT);
	}
	c->pressed = 1;

	if(gui->button_sound != NULL && ((prop = gf_prop_get_integer(&c->prop, "no-sound")) == GF_PROP_NO_SUCH || !prop)) {
		gf_audio_id_t id = gf_audio_load_file(gui->engine->client->audio, gui->button_sound);
		if(id >= 0) {
			gf_audio_auto_destroy(gui->engine->client->audio, id);
			gf_audio_resume(gui->engine->client->audio, id);
		}
	}

	if((prop = gf_prop_get_integer(&c->prop, "close-parent")) != GF_PROP_NO_SUCH && prop) {
		gf_gui_destroy_id(gui, c->parent);
	}
}
