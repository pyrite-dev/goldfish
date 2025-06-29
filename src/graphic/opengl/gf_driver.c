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
 * File Name:    gf_driver.c
 * Author:       Nishi
 * 
 * Functionality:
 * TODO: Write here
 * 
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_DRAW_DRIVER
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_TEXTURE
#define GF_EXPOSE_CORE

#include <gf_pre.h>

/* External library */
#include <gf_opengl.h>

/* Interface */
#include <gf_draw_driver.h>

/* Engine */
#include <gf_draw_platform.h>
#include <gf_texture.h>
#include <gf_draw.h>
#include <gf_log.h>
#include <gf_math.h>
#include <gf_graphic.h>
#include <gf_prop.h>
#include <gf_model.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

GLfloat lightwht[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightgry[] = {0.6f, 0.6f, 0.6f, 1.0f};
GLfloat lightdim[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat lightblk[] = {0.0f, 0.0f, 0.0f, 1.0f};

gf_draw_driver_texture_t* gf_draw_driver_register_texture(gf_draw_t* draw, int width, int height, int* iwidth, int* iheight, unsigned char* data) {
	gf_draw_driver_texture_t* r = malloc(sizeof(*r));
	int			  w = gf_math_nearest_2pow(width);
	int			  h = gf_math_nearest_2pow(height);
	double			  x, y, sx, sy;
	unsigned char*		  d;

	if(draw->driver->npot) {
		w = width;
		h = height;
	}
	if(w > draw->driver->max_texture_size) w = draw->driver->max_texture_size;
	if(h > draw->driver->max_texture_size) h = draw->driver->max_texture_size;

	d = malloc(w * h * 4);

	*iwidth	 = w;
	*iheight = h;

	sx = (double)width / w;
	sy = (double)height / h;

	if(w == width && h == height) {
		memcpy(d, data, w * h * 4);
	} else {
		memset(d, 0, w * h * 4);
		for(y = 0; y < h; y++) {
			for(x = 0; x < w; x++) {
				int ox	  = x * sx;
				int oy	  = y * sy;
				int pos	  = (y * w + x) * 4;
				int ogpos = (oy * width + ox) * 4;
				memcpy(d + pos + 0, data + ogpos + 0, 4);
			}
		}
	}

	glGenTextures(1, &r->id);
	glBindTexture(GL_TEXTURE_2D, r->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, d);

	if(strcmp(gf_prop_get_text(&draw->engine->config, "texture-filter"), "linear") == 0) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	} else if(strcmp(gf_prop_get_text(&draw->engine->config, "texture-filter"), "nearest") == 0) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
#if 0
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#endif
	glBindTexture(GL_TEXTURE_2D, 0);

	return r;
}

void gf_draw_driver_destroy_texture(gf_draw_driver_texture_t* t) {
	glDeleteTextures(1, &t->id);
	free(t);
}

gf_draw_driver_t* gf_draw_driver_create(gf_engine_t* engine, gf_draw_t* draw) {
	gf_draw_driver_t* draw_driver = malloc(sizeof(*draw_driver));
	int		  i;
	const char*	  str;
	GLint		  maxsz;
	char*		  renderer = (char*)glGetString(GL_RENDERER);
	memset(draw_driver, 0, sizeof(*draw_driver));
	draw_driver->engine = engine;

	gf_log_function(engine, "OpenGL renderer: %s", renderer == NULL ? "(what is wrong with your renderer?)" : renderer);

	glEnable(GL_BLEND);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LINE_SMOOTH);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glCullFace(GL_BACK);

	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightgry);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightwht);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightwht);

	glClearColor(0, 0, 0, 1);

	draw_driver->npot = 0;

	str = glGetString(GL_VERSION);
	if(str != NULL) {
		for(i = 0;; i++) {
			if(str[i] == '.' || str[i] == 0) {
				char* num = malloc(i + 1);
				memcpy(num, str, i);
				num[i] = 0;
				if(atoi(num) >= 2) {
					draw_driver->npot = 1;
				}
				gf_log_function(engine, "OpenGL %s.x available", num);
				free(num);
				break;
			}
		}
	}
	draw_driver->npot = draw_driver->npot || gf_draw_driver_has_extension(draw, "GL_ARB_texture_non_power_of_two");
	draw_driver->npot = draw_driver->npot || gf_draw_driver_has_extension(draw, "GL_ARB_texture_rectangle");
	draw_driver->npot = draw_driver->npot || gf_draw_driver_has_extension(draw, "GL_NV_texture_rectangle");
	if(draw_driver->npot) {
		gf_log_function(engine, "NPOT extension available", "");
	} else {
		gf_log_function(engine, "NPOT extension not available", "");
	}
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxsz);
	draw_driver->max_texture_size = maxsz;
	gf_log_function(engine, "Max texture size: %dx%d", draw_driver->max_texture_size, draw_driver->max_texture_size);

	return draw_driver;
}

int gf_draw_driver_has_extension(gf_draw_t* draw, const char* query) {
	int	    ret = gf_draw_platform_has_extension(draw, query);
	const char* ext = NULL;
	const char* ptr;
	const int   len = strlen(query);
	if(ret != 0) return ret;

	ext = glGetString(GL_EXTENSIONS);
	ptr = strstr(ext, query);
	return ((ptr != NULL) && ((ptr[len] == ' ') || (ptr[len] == '\0')));
}

void gf_draw_driver_reshape(gf_draw_t* draw) {
	glViewport(0, 0, (GLint)draw->width, (GLint)draw->height);
	glMatrixMode(GL_PROJECTION);
	gf_graphic_perspective(draw, 70, 0.1, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void gf_draw_driver_begin_texture_2d(gf_draw_t* draw, gf_texture_t* texture) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->draw_driver_texture->id);
}

void gf_draw_driver_end_texture_2d(gf_draw_t* draw) {
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void gf_draw_driver_set_color(gf_draw_t* draw, gf_graphic_color_t color) { glColor4f(color.r / 255, color.g / 255, color.b / 255, color.a / 255); }

void gf_draw_driver_destroy(gf_draw_driver_t* driver) {
	gf_log_function(driver->engine, "Destroyed drawing driver", "");
	free(driver);
}

void gf_draw_driver_before(gf_draw_t* draw) {
	GLfloat lightpos[4];
	GF_MATH_VECTOR_COPY(draw->light, lightpos);
	lightpos[3] = draw->light[3];

	gf_draw_driver_reshape(draw);

	gf_graphic_set_camera(draw);

	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	gf_graphic_clear(draw);
	gf_graphic_fill_rect(draw, 0, 0, draw->width, draw->height, draw->background);

	/* TODO: maybe move this skybox routine */
	if(draw->draw_3d && draw->skybox != NULL) {
		gf_graphic_color_t col;
		int		   i;
		col.r = 255;
		col.g = 255;
		col.b = 255;
		col.a = 255;

		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);

		for(i = 0; i < 6; i++) {
			double seq[4 * 5];
			int    j;
			double sz = 500;
			double d  = 0.0025;

			if(i == 0) {
				/* up */
				seq[5 * 0 + 0] = 1.0 / 4.0;
				seq[5 * 0 + 1] = 0.0 / 4.0;
				seq[5 * 0 + 2] = -1;
				seq[5 * 0 + 3] = 1 - d;
				seq[5 * 0 + 4] = 1;

				seq[5 * 1 + 0] = 1.0 / 4.0;
				seq[5 * 1 + 1] = 1.0 / 3.0;
				seq[5 * 1 + 2] = -1;
				seq[5 * 1 + 3] = 1 - d;
				seq[5 * 1 + 4] = -1;

				seq[5 * 2 + 0] = 2.0 / 4.0;
				seq[5 * 2 + 1] = 1.0 / 3.0;
				seq[5 * 2 + 2] = 1;
				seq[5 * 2 + 3] = 1 - d;
				seq[5 * 2 + 4] = -1;

				seq[5 * 3 + 0] = 2.0 / 4.0;
				seq[5 * 3 + 1] = 0.0 / 4.0;
				seq[5 * 3 + 2] = 1;
				seq[5 * 3 + 3] = 1 - d;
				seq[5 * 3 + 4] = 1;
			} else if(i == 1) {
				/* left */
				seq[5 * 0 + 0] = 1.0 / 4.0;
				seq[5 * 0 + 1] = 1.0 / 3.0;
				seq[5 * 0 + 2] = -1;
				seq[5 * 0 + 3] = 1;
				seq[5 * 0 + 4] = -1 + d;

				seq[5 * 1 + 0] = 1.0 / 4.0;
				seq[5 * 1 + 1] = 2.0 / 3.0;
				seq[5 * 1 + 2] = -1;
				seq[5 * 1 + 3] = -1;
				seq[5 * 1 + 4] = -1 + d;

				seq[5 * 2 + 0] = 2.0 / 4.0;
				seq[5 * 2 + 1] = 2.0 / 3.0;
				seq[5 * 2 + 2] = 1;
				seq[5 * 2 + 3] = -1;
				seq[5 * 2 + 4] = -1 + d;

				seq[5 * 3 + 0] = 2.0 / 4.0;
				seq[5 * 3 + 1] = 1.0 / 3.0;
				seq[5 * 3 + 2] = 1;
				seq[5 * 3 + 3] = 1;
				seq[5 * 3 + 4] = -1 + d;
			} else if(i == 2) {
				/* down */
				seq[5 * 0 + 0] = 1.0 / 4.0;
				seq[5 * 0 + 1] = 2.0 / 3.0;
				seq[5 * 0 + 2] = -1;
				seq[5 * 0 + 3] = -1 + d;
				seq[5 * 0 + 4] = -1;

				seq[5 * 1 + 0] = 1.0 / 4.0;
				seq[5 * 1 + 1] = 3.0 / 3.0;
				seq[5 * 1 + 2] = -1;
				seq[5 * 1 + 3] = -1 + d;
				seq[5 * 1 + 4] = 1;

				seq[5 * 2 + 0] = 2.0 / 4.0;
				seq[5 * 2 + 1] = 3.0 / 3.0;
				seq[5 * 2 + 2] = 1;
				seq[5 * 2 + 3] = -1 + d;
				seq[5 * 2 + 4] = 1;

				seq[5 * 3 + 0] = 2.0 / 4.0;
				seq[5 * 3 + 1] = 2.0 / 3.0;
				seq[5 * 3 + 2] = 1;
				seq[5 * 3 + 3] = -1 + d;
				seq[5 * 3 + 4] = -1;
			} else if(i == 3) {
				/* back */
				seq[5 * 0 + 0] = 0.0 / 4.0;
				seq[5 * 0 + 1] = 1.0 / 3.0;
				seq[5 * 0 + 2] = -1 + d;
				seq[5 * 0 + 3] = 1;
				seq[5 * 0 + 4] = 1;

				seq[5 * 1 + 0] = 1.0 / 4.0;
				seq[5 * 1 + 1] = 1.0 / 3.0;
				seq[5 * 1 + 2] = -1 + d;
				seq[5 * 1 + 3] = 1;
				seq[5 * 1 + 4] = -1;

				seq[5 * 2 + 0] = 1.0 / 4.0;
				seq[5 * 2 + 1] = 2.0 / 3.0;
				seq[5 * 2 + 2] = -1 + d;
				seq[5 * 2 + 3] = -1;
				seq[5 * 2 + 4] = -1;

				seq[5 * 3 + 0] = 0.0 / 4.0;
				seq[5 * 3 + 1] = 2.0 / 3.0;
				seq[5 * 3 + 2] = -1 + d;
				seq[5 * 3 + 3] = -1;
				seq[5 * 3 + 4] = 1;
			} else if(i == 4) {
				/* front */
				seq[5 * 0 + 0] = 2.0 / 4.0;
				seq[5 * 0 + 1] = 1.0 / 3.0;
				seq[5 * 0 + 2] = 1 - d;
				seq[5 * 0 + 3] = 1;
				seq[5 * 0 + 4] = -1;

				seq[5 * 1 + 0] = 3.0 / 4.0;
				seq[5 * 1 + 1] = 1.0 / 3.0;
				seq[5 * 1 + 2] = 1 - d;
				seq[5 * 1 + 3] = 1;
				seq[5 * 1 + 4] = 1;

				seq[5 * 2 + 0] = 3.0 / 4.0;
				seq[5 * 2 + 1] = 2.0 / 3.0;
				seq[5 * 2 + 2] = 1 - d;
				seq[5 * 2 + 3] = -1;
				seq[5 * 2 + 4] = 1;

				seq[5 * 3 + 0] = 2.0 / 4.0;
				seq[5 * 3 + 1] = 2.0 / 3.0;
				seq[5 * 3 + 2] = 1 - d;
				seq[5 * 3 + 3] = -1;
				seq[5 * 3 + 4] = -1;
			} else if(i == 5) {
				/* right */
				seq[5 * 0 + 0] = 3.0 / 4.0;
				seq[5 * 0 + 1] = 1.0 / 3.0;
				seq[5 * 0 + 2] = 1;
				seq[5 * 0 + 3] = 1;
				seq[5 * 0 + 4] = 1 - d;

				seq[5 * 1 + 0] = 4.0 / 4.0;
				seq[5 * 1 + 1] = 1.0 / 3.0;
				seq[5 * 1 + 2] = -1;
				seq[5 * 1 + 3] = 1;
				seq[5 * 1 + 4] = 1 - d;

				seq[5 * 2 + 0] = 4.0 / 4.0;
				seq[5 * 2 + 1] = 2.0 / 3.0;
				seq[5 * 2 + 2] = -1;
				seq[5 * 2 + 3] = -1;
				seq[5 * 2 + 4] = 1 - d;

				seq[5 * 3 + 0] = 3.0 / 4.0;
				seq[5 * 3 + 1] = 2.0 / 3.0;
				seq[5 * 3 + 2] = 1;
				seq[5 * 3 + 3] = -1;
				seq[5 * 3 + 4] = 1 - d;
			}

			for(j = 0; j < 4; j++) {
				int k;
				for(k = 2; k < 5; k++) {
					seq[5 * j + k] *= sz;
					seq[5 * j + k] += draw->camera[k - 2];
				}
			}
			gf_graphic_draw_texture_polygon_arr(draw, draw->skybox, col, GF_GRAPHIC_3D, 4, seq);
		}

		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glClear(GL_DEPTH_BUFFER_BIT);
	}
}

void gf_draw_driver_after(gf_draw_t* draw) { glFlush(); }
