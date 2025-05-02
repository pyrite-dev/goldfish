/**
 * @file gf_type/server.h
 * @~english
 * @brief Type definitions related to server interface
 */

#ifndef __GF_TYPE_SERVER_H__
#define __GF_TYPE_SERVER_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_SERVER
typedef struct gf_server_t gf_server_t;

/* External library */

/* Engine */
#include <gf_type/core.h>
#include <gf_type/physics.h>

/* Standard */

/**
 * @struct gf_server_t
 * @~english
 * @brief Server interface
 *
 * @var gf_server_t::engine
 * @brief Engine instance
 *
 * @var gf_server_t::physics
 * @brief Physics interface
 */
GF_DECLARE_TYPE(server, {
	gf_engine_t*  engine;
	gf_physics_t* physics;
});
#else
typedef void gf_server_t;
#endif

#endif
