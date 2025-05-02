/**
 * @file gf_draw_driver.h
 * @~english
 * @brief Drawing driver
 * @warning You should not use functions written here, since engine does it for you
 */

#ifndef __GF_DRAW_DRIVER_H__
#define __GF_DRAW_DRIVER_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/draw_driver.h>

/* Engine */
#include <gf_type/draw.h>
#include <gf_type/texture.h>
#include <gf_type/graphic.h>
#include <gf_type/core.h>
#include <gf_type/input.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Initialize drawing driver
 * @param engine Engine instance
 * @param draw Drawing interface
 * @return Drawing driver
 */
GF_EXPORT gf_draw_driver_t* gf_draw_driver_create(gf_engine_t* engine, gf_draw_t* draw);

/**
 * @~english
 * @brief Destroy drawing driver
 * @param driver Drawing driver
 */
GF_EXPORT void gf_draw_driver_destroy(gf_draw_driver_t* driver);

/**
 * @~english
 * @brief Check if renderer has extension
 * @param draw Drawing interface
 * @param query Extension to be checked
 * @return `1` if extension is present, otherwise if it isn't
 */
GF_EXPORT int gf_draw_driver_has_extension(gf_draw_t* draw, const char* query);

/**
 * @~english
 * @brief Do stuffs required on resizing window
 * @param draw Drawing interface
 */
GF_EXPORT void gf_draw_driver_reshape(gf_draw_t* draw);

/**
 * @~english
 * @brief Register driver-dependent texture
 * @param draw Drawing interface
 * @param width Width of texture
 * @param height Height of texture
 * @param iwidth Internal width to be returned
 * @param iheight Internal height to be returned
 * @param data Texture data
 * @return Driver-dependent texture
 */
GF_EXPORT gf_draw_driver_texture_t* gf_draw_driver_register_texture(gf_draw_t* draw, int width, int height, int* iwidth, int* iheight, unsigned char* data);

/**
 * @~english
 * @brief Destroy driver-dependent texture
 * @param texture Driver-dependent texture
 */
GF_EXPORT void gf_draw_driver_destroy_texture(gf_draw_driver_texture_t* texture);

/**
 * @~english
 * @brief Begin 2D texture mode
 * @param draw Drawing interface
 * @param texture Texture
 */
GF_EXPORT void gf_draw_driver_begin_texture_2d(gf_draw_t* draw, gf_texture_t* texture);

/**
 * @~english
 * @brief End 2D texture mode
 * @param draw Drawing interface
 */
GF_EXPORT void gf_draw_driver_end_texture_2d(gf_draw_t* draw);

/**
 * @~english
 * @brief Set color
 * @param draw Drawing interface
 * @param color Color
 */
GF_EXPORT void gf_draw_driver_set_color(gf_draw_t* draw, gf_graphic_color_t color);

/**
 * @~english
 * @brief Called before gf_draw_frame
 * @param draw Drawing interface
 */
GF_EXPORT void gf_draw_driver_before(gf_draw_t* draw);

/**
 * @~english
 * @brief Called after gf_draw_frame
 * @param draw Drawing interface
 */
GF_EXPORT void gf_draw_driver_after(gf_draw_t* draw);

#ifdef __cplusplus
}
#endif

#endif
