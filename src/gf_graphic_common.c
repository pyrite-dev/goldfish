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
 * File Name:    gf_graphic_common.c
 * Author:       Nishi
 * 
 * Functionality:
 * TODO: Write here
 * 
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_DRAW
#define GF_EXPOSE_TEXTURE
#define GF_EXPOSE_FONT

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>

/* Interface */
#include <gf_graphic.h>

/* Engine */
#include <gf_draw.h>
#include <gf_font.h>

/* Standard */
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

void gf_graphic_fill_rect(gf_draw_t* draw, double x, double y, double w, double h, gf_graphic_color_t color) { gf_graphic_fill_polygon(draw, color, GF_GRAPHIC_2D, 4, x, y, x, y + h, x + w, y + h, x + w, y); }

/**
 * TODO: Make gf_font do the most job
 */
void gf_graphic_text(gf_draw_t* draw, gf_font_t* userfont, double x, double y, double size, const char* text, gf_graphic_color_t color) {
	int		 i;
	double		 mx = 0;
	gf_font_glyph_t* glyph;
	double		 zoom = 0;
	gf_font_t*	 font = userfont;
	if(font == NULL) font = draw->font;
	if(font != NULL) {
		if(!font->use_glyph) {
			double	       width   = -1;
			double	       height  = -1;
			double	       ih      = 0;
			gf_texture_t** texture = gf_font_render(font, text, size, &width, &height);
			for(i = 0; i < arrlen(texture); i++) {
				gf_graphic_draw_texture_2d(draw, x, y + ih, width, texture[i]->height, texture[i], color);
				ih += texture[i]->height;
			}
			return;
		}
		zoom = size / font->bbox.height;
		for(i = 0; text[i] != 0; i++) {
			if((glyph = gf_font_get(font, text[i])) != NULL) {
				double fax = glyph->bbox.width;
				double fay = glyph->bbox.height;
				double fx  = glyph->bbox.x;
				double fy  = (font->bbox.height + font->bbox.y) - (glyph->bbox.height + glyph->bbox.y);
				gf_graphic_draw_texture_2d(draw, x + mx + fx * zoom, y + fy * zoom, zoom * fax, zoom * fay, glyph->texture, color);
				mx += zoom * glyph->dwidth[0];
			}
		}
	}
}

double gf_graphic_text_wrap(gf_draw_t* draw, gf_font_t* userfont, double x, double y, double w, double size, const char* text, gf_graphic_color_t color) {
	int		 i;
	double		 mx = 0;
	gf_font_glyph_t* glyph;
	double		 zoom = 0;
	gf_font_t*	 font = userfont;
	if(font == NULL) font = draw->font;
	if(font != NULL) {
		double my  = 0;
		double big = 0;
		if(!font->use_glyph) {
			double	       width   = w;
			double	       height  = -1;
			double	       ih      = 0;
			gf_texture_t** texture = gf_font_render(font, text, size, &width, &height);
			for(i = 0; i < arrlen(texture); i++) {
				gf_graphic_draw_texture_2d(draw, x, y + ih, width, texture[i]->height, texture[i], color);
				ih += texture[i]->height;
			}
			return height;
		}
		zoom = size / font->bbox.height;
		for(i = 0; text[i] != 0; i++) {
			if((glyph = gf_font_get(font, text[i])) != NULL) {
				double fax = glyph->bbox.width;
				double fay = glyph->bbox.height;
				double fx  = glyph->bbox.x;
				double fy  = (font->bbox.height + font->bbox.y) - (glyph->bbox.height + glyph->bbox.y);
				if(mx + (zoom * glyph->dwidth[0]) >= w) {
					mx = 0;
					my += size;
				}
				if((my + size) > big) {
					big = my + size;
				}
				gf_graphic_draw_texture_2d(draw, x + mx + fx * zoom, y + my + fy * zoom, zoom * fax, zoom * fay, glyph->texture, color);
				mx += zoom * glyph->dwidth[0];
			}
		}
		return big + size;
	}
	return 0;
}

double gf_graphic_text_width(gf_draw_t* draw, gf_font_t* userfont, double size, const char* text) {
	int		 i;
	double		 mx = 0;
	gf_font_glyph_t* glyph;
	double		 zoom = 0;
	gf_font_t*	 font = userfont;
	if(font != NULL) {
		if(!font->use_glyph) {
			double	       width   = -1;
			double	       height  = -1;
			gf_texture_t** texture = gf_font_render(font, text, size, &width, &height);
			return width;
		}
		zoom = size / font->bbox.height;
		for(i = 0; text[i] != 0; i++) {
			if((glyph = gf_font_get(font, text[i])) != NULL) {
				mx += zoom * glyph->dwidth[0];
			}
		}
	}
	return mx;
}

double gf_graphic_text_height(gf_draw_t* draw, gf_font_t* userfont, double size, const char* text) {
	double	   my	= 0;
	double	   zoom = 0;
	gf_font_t* font = userfont;
	if(font != NULL) {
		if(!font->use_glyph) {
			double	       width   = -1;
			double	       height  = -1;
			gf_texture_t** texture = gf_font_render(font, text, size, &width, &height);
			return height;
		}

		return size;
	}
	return my;
}

void gf_graphic_draw_texture_2d(gf_draw_t* draw, double x, double y, double w, double h, gf_texture_t* texture, gf_graphic_color_t color) {
	if(texture != NULL) gf_graphic_draw_texture_polygon(draw, texture, color, GF_GRAPHIC_2D, 4, 0.0, 0.0, x, y, 0.0, 1.0, x, y + h, 1.0, 1.0, x + w, y + h, 1.0, 0.0, x + w, y);
}

void gf_graphic_draw_texture_polygon(gf_draw_t* draw, gf_texture_t* texture, gf_graphic_color_t color, int dim, int npair, ...) {
	int	i;
	int	ind  = 0;
	int	plen = (dim == GF_GRAPHIC_2D ? 2 : 3) + 2;
	va_list va;
	double* arr = malloc(sizeof(*arr) * plen * npair);
	va_start(va, npair);
	for(i = 0; i < npair; i++) {
		int j;
		for(j = 0; j < plen; j++) {
			arr[ind * plen + j] = va_arg(va, double);
		}
		ind++;
	}
	va_end(va);
	gf_graphic_draw_texture_polygon_arr(draw, texture, color, dim, npair, arr);
	free(arr);
}

void gf_graphic_fill_polygon(gf_draw_t* draw, gf_graphic_color_t color, int dim, int npair, ...) {
	int	i;
	int	ind  = 0;
	int	plen = (dim == GF_GRAPHIC_2D ? 2 : 3);
	va_list va;
	double* arr = malloc(sizeof(*arr) * plen * npair);
	va_start(va, npair);
	for(i = 0; i < npair; i++) {
		int j;
		for(j = 0; j < plen; j++) {
			arr[ind * plen + j] = va_arg(va, double);
		}
		ind++;
	}
	va_end(va);
	gf_graphic_fill_polygon_arr(draw, color, dim, npair, arr);
	free(arr);
}

void gf_graphic_points(gf_draw_t* draw, gf_graphic_color_t color, int dim, int npair, ...) {
	int	i;
	int	ind  = 0;
	int	plen = (dim == GF_GRAPHIC_2D ? 2 : 3);
	va_list va;
	double* arr = malloc(sizeof(*arr) * plen * npair);
	va_start(va, npair);
	for(i = 0; i < npair; i++) {
		int j;
		for(j = 0; j < plen; j++) {
			arr[ind * plen + j] = va_arg(va, double);
		}
		ind++;
	}
	va_end(va);
	gf_graphic_points_arr(draw, color, dim, npair, arr);
	free(arr);
}

void gf_graphic_lines(gf_draw_t* draw, gf_graphic_color_t color, int dim, int npair, ...) {
	int	i;
	int	ind  = 0;
	int	plen = (dim == GF_GRAPHIC_2D ? 2 : 3);
	va_list va;
	double* arr = malloc(sizeof(*arr) * plen * npair * 2);
	va_start(va, npair);
	for(i = 0; i < npair * 2; i++) {
		int j;
		for(j = 0; j < plen; j++) {
			arr[ind * plen + j] = va_arg(va, double);
		}
		ind++;
	}
	va_end(va);
	gf_graphic_lines_arr(draw, color, dim, npair, arr);
	free(arr);
}
