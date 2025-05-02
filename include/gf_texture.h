/**
 * @file gf_texture.h
 * @~english
 * @brief Texture
 */

#ifndef __GF_TEXTURE_H__
#define __GF_TEXTURE_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/texture.h>

/* Engine */
#include <gf_type/draw.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Create texture
 * @param draw Drawing interface
 * @param width Width of texture
 * @param height Height of texture
 * @param data Texture data
 * @return Texture
 */
GF_EXPORT gf_texture_t* gf_texture_create(gf_draw_t* draw, int width, int height, unsigned char* data);

/**
 * @~english
 * @brief Destroy texture
 * @param texture Texture
 */
GF_EXPORT void gf_texture_destroy(gf_texture_t* texture);

/**
 * @~english
 * @brief Keep aspect or not
 * @param texture Texture
 * @param keep `0` to not keep, otherwise keep
 */
GF_EXPORT void gf_texture_keep_aspect(gf_texture_t* texture, int keep);

#ifdef __cplusplus
}
#endif

#endif
