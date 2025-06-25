/**
 * @file gf_type/model.h
 * @~english
 * @brief Type definitions related to model
 */

#ifndef __GF_TYPE_MODEL_H__
#define __GF_TYPE_MODEL_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_MODEL
#define gf_model_t struct gf_model

gf_model_t;

/* External library */

/* Engine */
#include <gf_type/texture.h>
#include <gf_type/core.h>
#include <gf_type/draw.h>

/* Standard */

/**
 * @struct gf_model
 * @~english
 * @brief Model
 *
 * @var gf_model::engine
 * @brief Engine instance
 *
 * @var gf_model::texture
 * @brief Texture
 *
 * @var gf_model::coords
 * @brief Coords
 *
 * @var gf_model::tcoords
 * @brief Texture coords
 *
 * @var gf_model::id
 * @brief "Fast" ID
 */
GF_DECLARE_TYPE(model, {
	gf_engine_t*  engine;
	gf_draw_t*    draw;
	gf_texture_t* texture;
	double*	      coords;
	double*	      tcoords;
	unsigned long id;
});
#else
typedef void gf_model_t;
#endif

#endif
