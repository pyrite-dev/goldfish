/**
 * @if SHOW_LICENSE
 **********************************************************************************
 *
 * Copyright (c) 2025 Pyrite Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. All advertising materials mentioning features or use of this software must
 *    display the following acknowledgement:
 *    This product includes software developed by the Pyrite Development Team.
 *
 * 4. Neither the name of Pyrite Development Team nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 **********************************************************************************
 * File Name:    gf_model.c
 * Author:       Nishi
 *
 * Functionality:
 * Parse obj and render model.
 *
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_MODEL
#define GF_EXPOSE_DRAW

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>

/* Interface */
#include <gf_model.h>

/* Engine */
#include <gf_math.h>
#include <gf_resource.h>
#include <gf_texture.h>
#include <gf_image.h>
#include <gf_log.h>
#include <gf_graphic.h>
#include <gf_draw_driver.h>

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
	unsigned char* rd;  /* resource data */
	char*	       srd; /* string resource data */
	size_t	       sz;
	int	       i;
	int	       incr	 = 0;
	double*	       v	 = NULL;
	double*	       vt	 = NULL;
	double	       sc	 = 1;
	double	       minx	 = 0;
	double	       maxx	 = 0;
	double	       miny	 = 0;
	double	       maxy	 = 0;
	double	       minz	 = 0;
	double	       maxz	 = 0;
	int	       calc_size = 0;
	gf_resource_t* r	 = gf_resource_create(draw->engine, path);
	if(r == NULL) {
		return NULL;
	}

	if(gf_resource_get(r, "model.scale", (void**)&srd, &sz) == 0) {
		char* s = malloc(sz + 1);
		memcpy(s, srd, sz);
		s[sz] = 0;

		sc = atof(s);

		free(s);
		free(srd);
	} else {
		calc_size = 1;
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

	if(gf_resource_get(r, "model.obj", (void**)&srd, &sz) != 0) {
		gf_texture_destroy(t);
		gf_resource_destroy(r);
		return NULL;
	}

	m = malloc(sizeof(*m));
	memset(m, 0, sizeof(*m));
	m->engine  = draw->engine;
	m->draw	   = draw;
	m->texture = t;
	m->id	   = 0;

	for(i = 0; i <= sz; i++) {
		if(srd[i] == '\n' || i == sz) {
			int    j;
			char*  line = srd + incr;
			char** args = NULL;
			int    argi = 0;
			srd[i]	    = 0;

			for(j = 0; line[j] != 0; j++) {
				/* remove CR */
				if(line[j] == '\r') {
					line[j] = 0;
					continue;
				}
			}
			if(strlen(line) > 0 && line[0] != '#') {
				for(j = 0;; j++) {
					if(line[j] == ' ' || line[j] == 0) {
						char  oldc = line[j];
						char* arg  = line + argi;
						line[j]	   = 0;
						arrput(args, arg);
						argi = j + 1;
						if(oldc == 0) break;
					}
				}
				if(strcmp(args[0], "v") == 0 && arrlen(args) >= 4) {
					double x = atof(args[1]);
					double z = -atof(args[2]);
					double y = atof(args[3]);

					x /= sc;
					y /= sc;
					z /= sc;

					if(calc_size && x < minx) minx = x;
					if(calc_size && x > maxx) maxx = x;
					if(calc_size && y < miny) miny = y;
					if(calc_size && y > maxy) maxy = y;
					if(calc_size && z < minz) minz = z;
					if(calc_size && z > maxz) maxz = z;

					arrput(v, x);
					arrput(v, y);
					arrput(v, z);
				} else if(strcmp(args[0], "vt") == 0 && arrlen(args) >= 3) {
					double x = atof(args[1]);
					double y = 1 - atof(args[2]);

					arrput(vt, x);
					arrput(vt, y);
				} else if(strcmp(args[0], "f") == 0 && arrlen(args) == 4) {
					for(j = 1; j < arrlen(args); j++) {
						char*  tmp;
						int    ind  = (atoi(args[j]) - 1) * 3;
						int    ind2 = -1;
						double x;
						double y;
						double z;
						if((tmp = strchr(args[j], '/')) != NULL) {
							ind2 = (atoi(tmp + 1) - 1) * 2;
						}

						x = v[ind + 0];
						y = v[ind + 1];
						z = v[ind + 2];
						arrput(m->coords, x);
						arrput(m->coords, y);
						arrput(m->coords, z);

						if(ind2 != -1) {
							x = vt[ind2 + 0];
							y = vt[ind2 + 1];
							arrput(m->tcoords, x);
							arrput(m->tcoords, y);
						}
					}
				}
			}
			arrfree(args);

			incr = i + 1;
			if(i == sz) break;
		}
	}
	arrfree(v);
	arrfree(vt);
	free(srd);

	if(calc_size) {
		double diffx = maxx - minx;
		double diffy = maxy - miny;
		double diffz = maxz - minz;
		double diff  = diffx > diffz ? diffx : diffz;
		diff	     = diffy > diff ? diffy : diff;
		gf_log_function(draw->engine, "Autocalculated size, scaling by %.2f", 1 / diff);
		for(i = 0; i < arrlen(m->coords); i++) {
			m->coords[i] /= diff;
		}
	}

	gf_resource_destroy(r);

	return m;
}

void gf_model_draw(gf_model_t* model, double x, double y, double z, double sx, double sy, double sz, double dx, double dy, double dz) {
	gf_draw_driver_begin_texture_2d(model->draw, model->texture);
	model->id = gf_graphic_fast(model->draw, model->id, arrlen(model->coords) / 3, model->coords, model->tcoords, x, y, z, sx, sy, sz, dx, dy, dz);
	gf_draw_driver_end_texture_2d(model->draw);
}

void gf_model_destroy(gf_model_t* model) {
	if(model->id != 0) gf_graphic_destroy_fast(model->draw, model->id);
	if(model->tcoords != NULL) arrfree(model->tcoords);
	if(model->coords != NULL) arrfree(model->coords);
	gf_texture_destroy(model->texture);
	gf_log_function(model->engine, "Destroyed model", "");
	free(model);
}
