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
 * File Name:    gf_gui_entry.c
 * Author:       Nishi
 * 
 * Functionality:
 * TODO: Write here
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
#include <gf_input.h>
#include <gf_util.h>

/* Standard */
#include <stdlib.h>

extern const double gf_gui_border_width;
extern const int    gf_gui_border_color_diff;

gf_gui_id_t gf_gui_create_entry(gf_gui_t* gui, double x, double y, double w, double h) {
	gf_gui_component_t* c = malloc(sizeof(*c));

	gf_gui_create_component(gui, c, x, y, w, h);

	c->type = GF_GUI_ENTRY;

	gf_prop_set_integer(&c->prop, "cursor", 0);

	arrput(gui->area, c);

	c->text	   = malloc(1);
	c->text[0] = 0;

	return c->key;
}

void gf_gui_entry_render(gf_gui_t* gui, gf_gui_component_t* c) {
	gf_input_t* input = gui->draw->input;
	double	    cx;
	double	    cy;
	double	    cw;
	double	    ch;
	double	    loc = 0;
	gf_font_t*  font;
	int	    prop;
	double	    propf;
	double	    fsz;
	int	    cr;
	double	    shift   = 0;
	int	    focused = 0;
	int	    changed = 0;
	if(c->type != GF_GUI_ENTRY) return;

	font = gf_prop_get_ptr_keep(&c->prop, "font");
	if(font == NULL) font = gui->draw->font;
	if(font == NULL) return;

	gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
	gf_graphic_clip_pop(gui->draw);

	if((propf = gf_prop_get_floating(&c->prop, "font-size")) != GF_PROP_NO_SUCH) {
		fsz = propf;
	} else {
		fsz = ch - gf_gui_border_width * 4;
	}

	cr = gf_prop_get_integer(&c->prop, "cursor");

	focused = ((prop = gf_prop_get_integer(&c->prop, "focus")) != GF_PROP_NO_SUCH && prop) ? 1 : 0;

	gf_gui_draw_box(gui, GF_GUI_INVERT, cx, cy, cw, ch);

	gf_graphic_clip_push(gui->draw, cx + gf_gui_border_width * 2, cy + gf_gui_border_width * 2, cw - gf_gui_border_width * 4, ch - gf_gui_border_width * 4);
	if(c->text != NULL) {
		char*  before;
		char*  after;
		double tw;
		double x     = cx + gf_gui_border_width * 2;
		double y     = cy + ch / 2 - fsz / 2;
		int    oldcr = cr;

		if(cr > strlen(c->text)) cr = strlen(c->text);
		if(cr < 0) cr = 0;
		if(oldcr != cr) gf_prop_set_integer(&c->prop, "cursor", cr);

		before = malloc(cr + 1);
		after  = malloc(strlen(c->text) - cr + 1);

		memcpy(before, c->text, cr);
		before[cr] = 0;

		memcpy(after, c->text + cr, strlen(c->text) - cr);
		after[strlen(c->text) - cr] = 0;

		if((tw = gf_graphic_text_width(gui->draw, font, fsz, c->text)) > (cw - gf_gui_border_width * 5)) {
			shift += tw - (cw - gf_gui_border_width * 5);
		}

		gf_graphic_text(gui->draw, font, x - shift, y, fsz, c->text, c->font);

		tw = gf_graphic_text_width(gui->draw, font, fsz, before);
		x += tw;
		loc = tw;

		free(before);
		free(after);

		if(focused) {
			if(gf_input_key_pressed(input, GF_INPUT_KEY_LEFT) && cr > 0) {
				gf_prop_set_integer(&c->prop, "cursor", cr - 1);
			} else if(gf_input_key_pressed(input, GF_INPUT_KEY_RIGHT) && cr < strlen(c->text)) {
				gf_prop_set_integer(&c->prop, "cursor", cr + 1);
			} else if(gf_input_key_pressed(input, GF_INPUT_KEY_HOME) && cr > 0) {
				gf_prop_set_integer(&c->prop, "cursor", 0);
			} else if(gf_input_key_pressed(input, GF_INPUT_KEY_END) && cr < strlen(c->text)) {
				gf_prop_set_integer(&c->prop, "cursor", strlen(c->text));
			} else if(gf_input_key_pressed(input, GF_INPUT_KEY_BACKSPACE) && strlen(c->text) > 0) {
				char* t;
				int   i;
				if(cr > 0) {
					for(i = cr - 1; i < strlen(c->text); i++) {
						c->text[i] = c->text[i + 1];
					}
					gf_prop_set_integer(&c->prop, "cursor", cr - 1);
					changed = 1;
				}
				t = gf_util_strdup(c->text);
				free(c->text);
				c->text = t;
			} else if(gf_input_key_pressed(input, GF_INPUT_KEY_DELETE) && strlen(c->text) > 0) {
				char* t;
				int   i;
				if(cr < strlen(c->text)) {
					for(i = cr; i < strlen(c->text); i++) {
						c->text[i] = c->text[i + 1];
					}
					changed = 1;
				}
				t = gf_util_strdup(c->text);
				free(c->text);
				c->text = t;
			}
		}
	}
	if(focused) {
		char kc;
		gf_graphic_fill_rect(gui->draw, cx + loc + gf_gui_border_width * 2 - shift, cy + gf_gui_border_width * 2, 1, ch - gf_gui_border_width * 4, c->font);
		if((kc = gf_input_key_char(input)) != 0) {
			if(c->text == NULL) {
				c->text	   = malloc(2);
				c->text[0] = kc;
				c->text[1] = 0;
			} else {
				char* t = malloc(strlen(c->text) + 2);
				int   i;
				for(i = 0; i < cr; i++) {
					t[i] = c->text[i];
				}
				t[cr] = kc;
				for(i = cr; i < strlen(c->text); i++) {
					t[i + 1] = c->text[i];
				}
				t[strlen(c->text) + 1] = 0;
				free(c->text);
				c->text = t;
			}
			gf_prop_set_integer(&c->prop, "cursor", cr + 1);
			changed = 1;
		}
	}
	if(changed && c->callback != NULL) {
		c->callback(gui->engine, gui->draw, gui->pressed, GF_GUI_CHANGE_EVENT);
	}
	if(gf_input_key_pressed(input, GF_INPUT_KEY_ENTER) && c->callback != NULL) {
		c->callback(gui->engine, gui->draw, gui->pressed, GF_GUI_ACTIVATE_EVENT);
	}
	gf_graphic_clip_pop(gui->draw);
}

void gf_gui_entry_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_ENTRY) return;
}

void gf_gui_entry_click(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_ENTRY) return;
}
