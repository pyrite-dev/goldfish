/**
 * @file gf_type/mesh.h
 * @~english
 * @brief Type definitions related to mesh
 */

#ifndef __GF_TYPE_MESH_H__
#define __GF_TYPE_MESH_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_MESH
#define gf_mesh_triangle_t struct gf_mesh_triangle
#define gf_mesh_t struct gf_mesh

gf_mesh_triangle_t;
gf_mesh_t;

/* External library */

/* Engine */
#include <gf_type/math.h>
#include <gf_type/graphic.h>

/* Standard */

/**
 * @struct gf_mesh_triangle_t
 * @~english
 * @brief Triangle
 *
 * @var gf_mesh_triangle_t::points
 * @brief Triangle points
 *
 * @var gf_mesh_triangle_t::color
 * @brief Triangle color
 */
GF_DECLARE_TYPE(mesh_triangle, {
	gf_math_vector_t   points[3];
	gf_graphic_color_t color;
});

/**
 * @struct gf_mesh_t
 * @~english
 * @brief Mesh
 *
 * @var gf_mesh_t::triangles
 * @brief Triangles
 *
 * @var gf_mesh_t::triangle_count
 * @brief Triangle count
 */
GF_DECLARE_TYPE(mesh, {
	gf_mesh_triangle_t* triangles;
	int		    triangle_count;
});
#else
typedef void gf_mesh_t;
typedef void gf_mesh_triangle_t;
#endif

#endif
