#define GF_EXPOSE_MODEL
#define GF_EXPOSE_DRAW

#include <gf_pre.h>

/* External library */

/* Interface */
#include <gf_model.h>

/* Engine */
#include <gf_math.h>
#include <gf_resource.h>
#include <gf_texture.h>
#include <gf_image.h>
#include <gf_log.h>

/* Standard */
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

gf_model_t* gf_model_load(gf_draw_t* draw, const char* path) {
	gf_model_t*    m;
	gf_texture_t*  t;
	int	       w;
	int	       h;
	unsigned char* d;
	unsigned char* rd; /* resource data */
	size_t	       sz;
	gf_resource_t* r = gf_resource_create(draw->engine, path);
	if(r == NULL) {
		return NULL;
	}

	if(gf_resource_get(r, "model.png", (void**)&rd, &sz) != 0) {
		gf_resource_destroy(r);
		return NULL;
	}

	d = gf_image_load_memory(draw->engine, rd, sz, &w, &h);
	free(rd);
	if(d == NULL) {
		gf_resource_destroy(r);
		return NULL;
	}

	t = gf_texture_create(draw, w, h, d);
	free(d);
	if(t == NULL) {
		gf_resource_destroy(r);
		return NULL;
	}

	m = malloc(sizeof(*m));
	memset(m, 0, sizeof(*m));
	m->engine  = draw->engine;
	m->draw	   = draw;
	m->texture = t;

	gf_resource_destroy(r);

	return r;
}

void gf_model_destroy(gf_model_t* model) {
	gf_texture_destroy(model->texture);
	gf_log_function(model->engine, "Destroyed model", "");
	free(model);
}
