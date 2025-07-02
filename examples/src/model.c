#define GF_EXPOSE_CORE
#define GF_EXPOSE_CLIENT
#define GF_EXPOSE_DRAW

#include <gf_core.h>
#include <gf_draw.h>
#include <gf_model.h>

#include <stdio.h>
#include <math.h>

gf_engine_t* engine;
double	     r = 0;

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <signal.h>

void handler(int sig) { gf_engine_shutdown(engine); }
#endif

#ifndef M_PI
#define M_PI 3.14159265
#endif

gf_model_t* model = NULL;

void callback(gf_draw_t* draw) {
	gf_draw_set_rotation(draw, r, r, r);
	gf_model_draw(model, 0, 0, 0, 1, 1, 1);
	r += 60.0 / draw->fps;
}

int main(int argc, char** argv) {
	gf_engine_param_t param;
	if(argc != 2) {
		fprintf(stderr, "Usage: %s path\n", argv[0]);
		return 1;
	}
#ifndef _WIN32
	signal(SIGINT, handler);
#endif

	param.prefix = "/usr/local";
	param.game   = "Model viewer";
	param.base   = "data";

	gf_engine_begin();
	engine = gf_engine_create_ex("Model viewer", 0, param, argv, argc);
	if(engine != NULL) {
		engine->client->draw->lookat[2] = 0;
		engine->client->draw->lookat[1] = 0.5;
		engine->client->draw->camera[2] = -2;
		if((model = gf_model_load(engine->client->draw, argv[1])) != NULL) {
			gf_draw_set_callback(engine->client->draw, callback);
			gf_engine_loop(engine);
			gf_model_destroy(model);
			gf_engine_destroy(engine);
		}
	}
	gf_engine_end();
}
