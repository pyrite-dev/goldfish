/**
 * @file gf_type/physics.h
 * @~english
 * @brief Type definitions related to physics interface
 */

#ifndef __GF_TYPE_PHYSICS_H__
#define __GF_TYPE_PHYSICS_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_PHYSICS
typedef struct gf_physics gf_physics_t;

/* External library */
#include <ode/ode.h>

/* Engine */
#include <gf_type/core.h>

/* Standard */

/**
 * @struct gf_physics_t
 * @~english
 * @brief Physics interface
 *
 * @var gf_physics_t::engine
 * @brief Engine instance
 *
 * @var gf_physics_t::id
 * @brief ODE's world ID
 */
GF_DECLARE_TYPE(physics, {
	gf_engine_t* engine;
	dWorldID     id;
});
#else
typedef void gf_physics_t;
#endif

#endif
