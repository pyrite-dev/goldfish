/**
 * @file gf_type/font.h
 * @~english
 * @brief Type definitions related to font
 */

#ifndef __GF_TYPE_FONT_H__
#define __GF_TYPE_FONT_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_FONT
#define gf_font_store_t struct gf_font_store
#define gf_font_bbox_t struct gf_font_bbox
#define gf_font_glyph_t struct gf_font_glyph
#define gf_font_cache_t struct gf_font_cache
#define gf_font_t struct gf_font

gf_font_store_t;
gf_font_bbox_t;
gf_font_glyph_t;
gf_font_cache_t;
gf_font_t;

/* External library */
#include <stb_truetype.h>

/* Engine */
#include <gf_type/draw.h>
#include <gf_type/texture.h>

/* Standard */

/**
 * @struct gf_font_store
 * @~english
 * @brief Internal struct used for parsing
 *
 * @var gf_font_store::glyph_index
 * @brief Current glyph index
 *
 * @var gf_font_store::glyph_index
 * @brief Current line index of glyph
 *
 * @var gf_font_store::buffer
 * @brief Glyph buffer
 */
GF_DECLARE_TYPE(font_store, {
	int	       glyph_index;
	int	       line_index;
	unsigned char* buffer;
});

/**
 * @struct gf_font_bbox
 * @~english
 * @brief Bounding box
 *
 * @var gf_font_bbox::width
 * @brief Width
 *
 * @var gf_font_bbox::height
 * @brief Height
 *
 * @var gf_font_bbox::x
 * @brief X coord
 *
 * @var gf_font_bbox::y
 * @brief Y coord
 */
GF_DECLARE_TYPE(font_bbox, {
	int width;
	int height;
	int x;
	int y;
});

/**
 * @struct gf_font_glyph
 * @~english
 * @brief Glyph
 *
 * @var gf_font_glyph::code
 * @brief Character code
 *
 * @var gf_font_glyph::texture
 * @brief Texture
 *
 * @var gf_font_glyph::bpl
 * @brief Bytes per line
 *
 * @var gf_font_glyph::bbox
 * @brief Bounding box
 *
 * @var gf_font_glyph::dwidth
 * @brief Device width
 */
GF_DECLARE_TYPE(font_glyph, {
	int	       code;
	gf_texture_t*  texture;
	int	       bpl;
	gf_font_bbox_t bbox;
	int	       dwidth[2];
});

/**
 * @struct gf_font_cache
 * @~english
 * @brief Font cache
 *
 * @var gf_font_cache::text
 * @brief Text
 *
 * @var gf_font_cache::size
 * @brief Size
 *
 * @var gf_font_cache::width
 * @brief Width
 *
 * @var gf_font_cache::height
 * @brief Height
 *
 * @var gf_font_cache::texture
 * @brief Texture
 *
 * @var gf_font_cache::lw
 * @brief Limit width
 *
 * @var gf_font_cache::lh
 * @brief Limit height
 */
GF_DECLARE_TYPE(font_cache, {
	char*	       text;
	double	       size;
	double	       width;
	double	       height;
	gf_texture_t** texture;
	double	       lw;
	double	       lh;
});

/**
 * @struct gf_font
 * @~english
 * @brief Font
 *
 * @var gf_font::draw
 * @brief Drawing interface
 *
 * @var gf_font::glyph
 * @brief Glyph data
 *
 * @var gf_font::count
 * @brief Glyph count
 *
 * @var gf_font::bbox
 * @brief Bounding box
 *
 * @var gf_font::ttf
 * @brief TTF information
 *
 * @var gf_font::use_glyph
 * @brief `1` if you should use texture inside gf_font::glyph, `0` otherwise
 *
 * @var gf_font::cache
 * @brief Cache
 */
GF_DECLARE_TYPE(font, {
	gf_draw_t*	  draw;
	gf_font_glyph_t** glyph;
	int		  count;
	gf_font_bbox_t	  bbox;
	unsigned char*	  buffer;
	stbtt_fontinfo	  ttf;
	int		  use_glyph;
	gf_font_cache_t*  cache;
});
#else
typedef void gf_font_glyph_t;
typedef void gf_font_bbox_t;
typedef void gf_font_cache_t;
typedef void gf_font_t;
#endif

#endif
