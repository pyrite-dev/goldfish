/**
 * @file gf_type/texture.h
 * @~english
 * @brief Type definitions related to texture
 */

#ifndef __GF_TYPE_TEXTURE_H__
#define __GF_TYPE_TEXTURE_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_TEXTURE
#define gf_texture_t struct gf_texture

gf_texture_t;

/* External library */

/* Engine */
#include <gf_type/draw_driver.h>

/* Standard */

/**
 * @struct gf_texture_t
 * @~english
 * @brief Texture
 *
 * @var gf_texture_t::draw_driver_texture
 * @brief Driver-dependent texture
 *
 * @var gf_texture_t::width
 * @brief Width of texture
 *
 * @var gf_texture_t::height
 * @brief Height of texture
 *
 * @var gf_texture_t::internal_width
 * @brief Internal width of texture
 *
 * @var gf_texture_t::internal_height
 * @brief Internal height of texture
 *
 * @var gf_texture_t::keep_aspect
 * @brief Keep aspect or not
 */
GF_DECLARE_TYPE(texture, {
	gf_draw_driver_texture_t* draw_driver_texture;
	int			  width;
	int			  height;
	int			  internal_width;
	int			  internal_height;
	int			  keep_aspect;
});
#else
typedef void gf_texture_t;
#endif

#endif
