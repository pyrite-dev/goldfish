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
#include <gf_type/mesh.h>
#include <gf_type/texture.h>

/* Standard */

/**
 * @struct gf_model
 * @~english
 * @brief Model
 *
 * @var gf_model::mesh
 * @brief Mesh
 *
 * @var gf_model::texture
 * @brief Texture
 */
GF_DECLARE_TYPE(model, {
	gf_mesh_t*    mesh;
	gf_texture_t* texture;
});
#else
typedef void gf_model_t;
#endif

#endif
