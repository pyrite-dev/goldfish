/**
 * @file gf_model.h
 * @~english
 * @brief Model
 */

#ifndef __GF_MODEL_H__
#define __GF_MODEL_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/model.h>

/* Engine */
#include <gf_type/draw.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Load model
 * @param draw Drawing interface
 * @param path Path
 * @return Model
 */
GF_EXPORT gf_model_t* gf_model_load(gf_draw_t* draw, const char* path);

/**
 * @~english
 * @brief Draw model
 * @param draw Drawing interface
 * @param model Model
 * @param x X
 * @param y Y
 * @param z Z
 */
GF_EXPORT void gf_model_draw(gf_draw_t* draw, gf_model_t* model, double x, double y, double z);

/**
 * @~english
 * @brief Destroy model
 * @param model Model
 */
GF_EXPORT void gf_model_destroy(gf_model_t* model);

#ifdef __cplusplus
}
#endif

#endif
