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
 * File Name:    gf_gui_frame.c
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

gf_gui_id_t gf_gui_create_frame(gf_gui_t* gui, double x, double y, double w, double h) {
	gf_gui_component_t* c = malloc(sizeof(*c));

	gf_gui_create_component(gui, c, x, y, w, h);

	c->type = GF_GUI_FRAME;

	arrput(gui->area, c);

	return c->key;
}

void gf_gui_frame_render(gf_gui_t* gui, gf_gui_component_t* c) {
	double	   cx;
	double	   cy;
	double	   cw;
	double	   ch;
	double	   sx;
	double	   sy;
	gf_font_t* font;
	double	   propf;
	double	   fsz = GF_GUI_SMALL_FONT_SIZE;
	if(c->type != GF_GUI_FRAME) return;
	if(c->text == NULL) return;

	font = gf_prop_get_ptr_keep(&c->prop, "font");
	if(font == NULL) font = gui->draw->font;
	if(font == NULL) return;

	gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
	gf_graphic_clip_pop(gui->draw);

	if((propf = gf_prop_get_floating(&c->prop, "font-size")) != GF_PROP_NO_SUCH) {
		fsz = propf;
	}

	sx = cw / 2 - gf_graphic_text_width(gui->draw, font, fsz, c->text) / 2;
	sy = ch / 2 - gf_graphic_text_height(gui->draw, font, fsz, c->text) / 2;

	gf_graphic_clip_push(gui->draw, cx, cy, cw, ch);
	gf_graphic_text(gui->draw, font, cx + sx, cy + sy, fsz, c->text, gui->font);
	gf_graphic_clip_pop(gui->draw);
}

void gf_gui_frame_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_FRAME) return;
}

void gf_gui_frame_click(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_FRAME) return;
}
