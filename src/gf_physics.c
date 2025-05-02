#define GF_EXPOSE_PHYSICS

#include <gf_pre.h>

/* External library */
#include <ode/ode.h>

/* Interface */
#include <gf_physics.h>

/* Engine */
#include <gf_log.h>

/* Standard */
#include <stdlib.h>

void gf_physics_begin(void) { dInitODE(); }

void gf_physics_end(void) { dCloseODE(); }

gf_physics_t* gf_physics_create(gf_engine_t* engine) {
	gf_physics_t* physics = malloc(sizeof(*physics));
	physics->engine	      = engine;
	physics->id	      = dWorldCreate();
	dWorldSetGravity(physics->id, 0, 0, -9.81);
	return physics;
}

void gf_physics_destroy(gf_physics_t* physics) {
	dWorldDestroy(physics->id);
	gf_log_function(physics->engine, "Destroyed physics interface", "");
	free(physics);
}
