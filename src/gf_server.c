#define GF_EXPOSE_SERVER

#include <gf_pre.h>

/* External library */

/* Interface */
#include <gf_server.h>

/* Engine */
#include <gf_physics.h>
#include <gf_log.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

void gf_server_begin(void) { gf_physics_begin(); }

void gf_server_end(void) { gf_physics_end(); }

gf_server_t* gf_server_create(gf_engine_t* engine) {
	gf_server_t* server = malloc(sizeof(*server));
	memset(server, 0, sizeof(*server));
	server->engine	= engine;
	server->physics = gf_physics_create(engine);
	if(server->physics == NULL) {
		gf_log_function(engine, "Failed to create physics interface", "");
		gf_server_destroy(server);
		return NULL;
	}
	return server;
}

void gf_server_destroy(gf_server_t* server) {
	gf_server_shutdown(server);
	if(server->physics != NULL) gf_physics_destroy(server->physics);
	gf_log_function(server->engine, "Destroyed server interface", "");
	free(server);
}

void gf_server_shutdown(gf_server_t* server) { gf_log_function(server->engine, "Server shutdown complete", ""); }
