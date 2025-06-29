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
 * File Name:    gf_gui_window.c
 * Author:       Nishi
 *
 * Functionality:
 * GUI window code.
 *
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_GUI
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_INPUT
#define GF_EXPOSE_TEXTURE

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
#include <gf_file.h>
#include <gf_texture.h>
#include <gf_image.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

extern const double gf_gui_border_width;
extern const int    gf_gui_border_color_diff;

gf_gui_id_t gf_gui_create_window(gf_gui_t* gui, double x, double y, double w, double h) {
	gf_gui_component_t* c = malloc(sizeof(*c));
	gf_gui_id_t	    close_button;
	gf_gui_id_t	    frame;

	gf_gui_create_component(gui, c, x, y, w, h);

	c->type = GF_GUI_WINDOW;

	arrput(gui->area, c);

	gf_prop_set_text(gf_gui_get_prop(gui, c->key), "icon", "base:/winicon.png");

	close_button = gf_gui_create_button(gui, 5, 5, 20, 20);
	gf_gui_set_parent(gui, close_button, c->key);
	gf_prop_set_integer(gf_gui_get_prop(gui, close_button), "x-base", 1);
	gf_prop_set_integer(gf_gui_get_prop(gui, close_button), "close-parent", 1);
	gf_gui_set_text(gui, close_button, "#Cross");

	frame = gf_gui_create_frame(gui, 5, 10 + 20, w - 10, h - 20 - 10 - 5);

	gf_gui_set_parent(gui, frame, c->key);

	gf_gui_set_prop_id(gui, c->key, "frame", frame);
	gf_gui_set_prop_id(gui, c->key, "close-button", close_button);

	return c->key;
}

void gf_gui_window_render(gf_gui_t* gui, gf_gui_component_t* c) {
	gf_input_t*	   input = gui->draw->input;
	double		   cx;
	double		   cy;
	double		   cw;
	double		   ch;
	gf_prop_integer_t  prop;
	gf_graphic_color_t col = gui->font;
	gf_gui_id_t	   fid;
	int		   frame;
	gf_font_t*	   font;
	double		   shift = 0;
	const char*	   icon;
	if(c->type != GF_GUI_WINDOW) return;

	font = gf_prop_get_ptr_keep(&c->prop, "font");
	if(font == NULL) font = gui->draw->font;
	if(font == NULL) return;

	gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
	gf_graphic_clip_pop(gui->draw);

	if((icon = gf_prop_get_text(&c->prop, "icon")) != NULL && c->texture == NULL) {
		int	       w;
		int	       h;
		unsigned char* buf = gf_image_load(gui->engine, icon, &w, &h);
		if(buf != NULL) {
			c->texture = gf_texture_create(gui->draw, w, h, buf);
			free(buf);
		} else {
			gf_prop_delete(&c->prop, "icon");
		}
	}

	if((prop = gf_prop_get_integer(&c->prop, "active")) == GF_PROP_NO_SUCH || !prop) {
		col.r -= (double)gf_gui_border_color_diff * 3 / 2;
		col.g -= (double)gf_gui_border_color_diff * 3 / 2;
		col.b -= (double)gf_gui_border_color_diff * 3 / 2;
	}

	gf_gui_draw_box(gui, GF_GUI_NORMAL, cx, cy, cw, ch);

	if(c->texture != NULL) {
		gf_graphic_color_t col;
		col.r = 255;
		col.g = 255;
		col.b = 255;
		col.a = 255;

		shift = (double)20 / c->texture->height;
		shift = c->texture->width * shift;
		gf_graphic_draw_texture_2d(gui->draw, cx + 10, cy + 10 - (double)20 / 4, shift, 20, c->texture, col);
		shift += 5;
	}

	if(c->text != NULL) {
		gf_graphic_clip_push(gui->draw, cx, cy, cw - 20 - 10, 20 + 10);
		gf_graphic_text(gui->draw, font, cx + 10 + shift, cy + 10 - (double)GF_GUI_SMALL_FONT_SIZE / 4, GF_GUI_SMALL_FONT_SIZE, c->text, col);
		gf_graphic_clip_pop(gui->draw);
	}

	fid   = gf_gui_get_prop_id(gui, c->key, "frame");
	frame = gf_gui_get_index(gui, fid);
	if(frame != -1) {
		gf_gui_component_t* cf = gui->area[frame];
		cf->width	       = c->width - 10;
		cf->height	       = c->height - 20 - 10 - 5;
		if((prop = gf_prop_get_integer(&c->prop, "resizable")) != GF_PROP_NO_SUCH && prop) {
			cf->height -= 20;
		}
	}
}

void gf_gui_window_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	gf_input_t* input = gui->draw->input;
	if(c->type != GF_GUI_WINDOW) return;

	c->x = input->mouse_x - gf_prop_get_integer(&c->prop, "diff-x");
	c->y = input->mouse_y - gf_prop_get_integer(&c->prop, "diff-y");
}

void gf_gui_window_click(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_WINDOW) return;
}
