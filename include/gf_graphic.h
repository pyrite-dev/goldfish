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
 * @endif
 */

/**
 * @file gf_graphic.h
 * @~english
 * @brief Graphic interface
 */

#ifndef __GF_GRAPHIC_H__
#define __GF_GRAPHIC_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/graphic.h>

/* Engine */
#include <gf_type/draw.h>
#include <gf_type/texture.h>
#include <gf_type/font.h>

/* Standard */
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Dimension parameter for 2D
 */
#define GF_GRAPHIC_2D 2

/**
 * @~english
 * @brief Dimension parameter for 3D
 */
#define GF_GRAPHIC_3D 3

/**
 * @~english
 * @brief Macro to set color safely and shorter
 * @param color gf_graphic_color_t
 */
#define GF_SET_COLOR(color, red, green, blue, alpha) \
	color.r = (red); \
	color.g = (green); \
	color.b = (blue); \
	color.a = (alpha)

/**
 * @~english
 * @brief Clear the screen
 */
GF_EXPORT void gf_graphic_clear(gf_draw_t* draw);

/**
 * @~english
 * @brief Start 2D mode
 */
GF_EXPORT void gf_graphic_begin_2d(gf_draw_t* draw);

/**
 * @~english
 * @brief End 2D mode
 */
GF_EXPORT void gf_graphic_end_2d(gf_draw_t* draw);

/**
 * @~english
 * @brief Draw polygon with texture
 */
GF_EXPORT void gf_graphic_draw_texture_polygon_arr(gf_draw_t* draw, gf_texture_t* texture, gf_graphic_color_t color, int dim, int npair, double* arr);

/**
 * @~english
 * @brief Draw polygon
 */
GF_EXPORT void gf_graphic_fill_polygon_arr(gf_draw_t* draw, gf_graphic_color_t color, int dim, int npair, double* arr);

/**
 * @~english
 * @brief Draw points
 */
GF_EXPORT void gf_graphic_points_arr(gf_draw_t* draw, gf_graphic_color_t color, int dim, int npair, double* arr);

/**
 * @~english
 * @brief Draw lines
 */
GF_EXPORT void gf_graphic_lines_arr(gf_draw_t* draw, gf_graphic_color_t color, int dim, int npair, double* arr);

/**
 * @~english
 * @brief Set perspective
 * @param draw Drawing interface
 * @param fovy FOV
 * @param znear Distance from viewer to the near clipping plane
 * @param zfar Distance from viewer to the far clipping plane
 */
GF_EXPORT void gf_graphic_perspective(gf_draw_t* draw, double fovy, double znear, double zfar);

/**
 * @~english
 * @brief Set camera
 * @param draw Drawing interface
 */
GF_EXPORT void gf_graphic_set_camera(gf_draw_t* draw);

/**
 * @~english
 * @brief Push clipping region
 * @param draw Drawing interface
 * @param x X
 * @param y Y
 * @param w Width
 * @param h Height
 */
GF_EXPORT void gf_graphic_clip_push(gf_draw_t* draw, double x, double y, double w, double h);

/**
 * @~english
 * @brief Pop clipping region
 * @param draw Drawing interface
 */
GF_EXPORT void gf_graphic_clip_pop(gf_draw_t* draw);

/**
 * @~english
 * @brief Set point size
 * @param draw Drawing interface
 * @param size Point size
 */
GF_EXPORT void gf_graphic_set_point_size(gf_draw_t* draw, double size);

/**
 * @~english
 * @brief Set line width
 * @param draw Drawing interface
 * @param width Line width
 */
GF_EXPORT void gf_graphic_set_line_width(gf_draw_t* draw, double width);

/**
 * @~english
 * @brief Get point size
 * @param draw Drawing interface
 * @return Point size
 */
GF_EXPORT double gf_graphic_get_point_size(gf_draw_t* draw);

/**
 * @~english
 * @brief Get line width
 * @param draw Drawing interface
 * @return Line width
 */
GF_EXPORT double gf_graphic_get_line_width(gf_draw_t* draw);

/**
 * @~english
 * @brief Do "Fast" render
 * @param draw Drawing interface
 * @param id "Fast" ID
 * @param npair Pair count
 * @param coords Coords
 * @param tcoords Texture coords
 * @param x X
 * @param y Y
 * @param z Z
 * @param sx X scaling
 * @param sy Y scaling
 * @param sz Z scaling
 * @return Return "fast" ID
 */
GF_EXPORT unsigned long gf_graphic_fast(gf_draw_t* draw, unsigned long id, int npair, double* coords, double* tcoords, double x, double y, double z, double sx, double sy, double sz);

/**
 * @~english
 * @brief Destroy "fast" ID
 * @param draw Drawing interface
 * @param id "Fast" ID
 */
GF_EXPORT void gf_graphic_destroy_fast(gf_draw_t* draw, unsigned long id);

/* Common */

/**
 * @~english
 * @brief Calculate text width
 * @return Text width
 */
GF_EXPORT double gf_graphic_text_width(gf_draw_t* draw, gf_font_t* userfont, double size, const char* text);

/**
 * @~english
 * @brief Calculate text height
 * @return Text height
 */
GF_EXPORT double gf_graphic_text_height(gf_draw_t* draw, gf_font_t* userfont, double size, const char* text);

/**
 * @~english
 * @brief Draw text
 */
GF_EXPORT void gf_graphic_text(gf_draw_t* draw, gf_font_t* userfont, double x, double y, double size, const char* text, gf_graphic_color_t color);

/**
 * @~english
 * @brief Draw text with wrapping
 * @return Height
 */
GF_EXPORT double gf_graphic_text_wrap(gf_draw_t* draw, gf_font_t* userfont, double x, double y, double w, double size, const char* text, gf_graphic_color_t color);

/**
 * @~english
 * @brief Draw filled rectangle
 */
GF_EXPORT void gf_graphic_fill_rect(gf_draw_t* draw, double x, double y, double w, double h, gf_graphic_color_t color);

/**
 * @~english
 * @brief Draw rectangle with texture
 */
GF_EXPORT void gf_graphic_draw_texture_2d(gf_draw_t* draw, double x, double y, double w, double h, gf_texture_t* texture, gf_graphic_color_t color);

/**
 * @~english
 * @brief Draw polygon with texture
 */
GF_EXPORT void gf_graphic_draw_texture_polygon(gf_draw_t* draw, gf_texture_t* texture, gf_graphic_color_t color, int dim, int npair, ...);

/**
 * @~english
 * @brief Draw polygon
 */
GF_EXPORT void gf_graphic_fill_polygon(gf_draw_t* draw, gf_graphic_color_t color, int dim, int npair, ...);

/**
 * @~english
 * @brief Draw points
 */
GF_EXPORT void gf_graphic_points(gf_draw_t* draw, gf_graphic_color_t color, int dim, int npair, ...);

/**
 * @~english
 * @brief Draw lines
 */
GF_EXPORT void gf_graphic_lines(gf_draw_t* draw, gf_graphic_color_t color, int dim, int npair, ...);

#ifdef __cplusplus
}
#endif

#endif
