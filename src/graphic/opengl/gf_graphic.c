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
 * File Name:    gf_graphic.c
 * Author:       Nishi
 *
 * Functionality:
 * Driver-dependent graphic code for OpenGL.
 *
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_DRAW
#define GF_EXPOSE_TEXTURE

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>
#include <gf_opengl.h>

/* Interface */
#include <gf_graphic.h>

/* Engine */
#include <gf_draw.h>
#include <gf_texture.h>
#include <gf_draw_driver.h>
#include <gf_math.h>

/* Standard */
#include <stdarg.h>

void gf_graphic_begin_2d(gf_draw_t* draw) {
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, draw->width, draw->height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void gf_graphic_end_2d(gf_draw_t* draw) {
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void gf_graphic_clear(gf_draw_t* draw) { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); }

void gf_graphic_draw_texture_polygon_arr(gf_draw_t* draw, gf_texture_t* texture, gf_graphic_color_t color, int dim, int npair, double* arr) {
	double tw = (double)texture->width / texture->internal_width;
	double th = (double)texture->height / texture->internal_height;
	double sx = 1;
	double sy = 1;
	int    i;
	int    ind = 0;

	if(texture->keep_aspect) {
		if(tw > th) {
			sx = th / tw;
		} else {
			sy = tw / th;
		}
	}

	if(dim == GF_GRAPHIC_2D) gf_graphic_begin_2d(draw);

	gf_draw_driver_set_color(draw, color);
	gf_draw_driver_begin_texture_2d(draw, texture);
	glBegin(GL_TRIANGLE_FAN);

	for(i = 0; i < npair; i++) {
		double tx = arr[ind++] * sx;
		double ty = arr[ind++] * sy;
		double x  = arr[ind++];
		double y  = arr[ind++];
		glTexCoord2f(tx, ty);
		if(dim == GF_GRAPHIC_2D) {
			glVertex2f(x, y);
		} else if(dim == GF_GRAPHIC_3D) {
			double z = arr[ind++];
			glVertex3f(x, y, z);
		}
	}

	glEnd();

	gf_draw_driver_end_texture_2d(draw);
	if(dim == GF_GRAPHIC_2D) gf_graphic_end_2d(draw);
}

void gf_graphic_fill_polygon_arr(gf_draw_t* draw, gf_graphic_color_t color, int dim, int npair, double* arr) {
	int i;
	int ind = 0;

	if(dim == GF_GRAPHIC_2D) gf_graphic_begin_2d(draw);

	gf_draw_driver_set_color(draw, color);
	glBegin(GL_TRIANGLE_FAN);

	for(i = 0; i < npair; i++) {
		double x = arr[ind++];
		double y = arr[ind++];
		if(dim == GF_GRAPHIC_2D) {
			glVertex2f(x, y);
		} else if(dim == GF_GRAPHIC_3D) {
			double z = arr[ind++];
			glVertex3f(x, y, z);
		}
	}

	glEnd();
	if(dim == GF_GRAPHIC_2D) gf_graphic_end_2d(draw);
}

void gf_graphic_points_arr(gf_draw_t* draw, gf_graphic_color_t color, int dim, int npair, double* arr) {
	int i;
	int ind = 0;

	if(dim == GF_GRAPHIC_2D) gf_graphic_begin_2d(draw);

	gf_draw_driver_set_color(draw, color);
	glBegin(GL_POINTS);

	for(i = 0; i < npair; i++) {
		double x = arr[ind++];
		double y = arr[ind++];
		if(dim == GF_GRAPHIC_2D) {
			glVertex2f(x, y);
		} else if(dim == GF_GRAPHIC_3D) {
			double z = arr[ind++];
			glVertex3f(x, y, z);
		}
	}

	glEnd();
	if(dim == GF_GRAPHIC_2D) gf_graphic_end_2d(draw);
}

void gf_graphic_lines_arr(gf_draw_t* draw, gf_graphic_color_t color, int dim, int npair, double* arr) {
	int i;
	int ind = 0;

	if(dim == GF_GRAPHIC_2D) gf_graphic_begin_2d(draw);

	gf_draw_driver_set_color(draw, color);
	glBegin(GL_LINES);

	for(i = 0; i < npair * 2; i++) {
		double x = arr[ind++];
		double y = arr[ind++];
		if(dim == GF_GRAPHIC_2D) {
			glVertex2f(x, y);
		} else if(dim == GF_GRAPHIC_3D) {
			double z = arr[ind++];
			glVertex3f(x, y, z);
		}
	}

	glEnd();
	if(dim == GF_GRAPHIC_2D) gf_graphic_end_2d(draw);
}

void gf_graphic_perspective(gf_draw_t* draw, double fovy, double znear, double zfar) {
	double	 aspect = (double)draw->width / (double)draw->height;
	double	 f	= gf_math_cot(fovy / 180 * GF_MATH_PI / 2);
	GLdouble matrix[16];
	int	 i;

	for(i = 0; i < 16; i++) matrix[i] = 0;
	matrix[4 * 0 + 0] = f / aspect;
	matrix[4 * 1 + 1] = f;
	matrix[4 * 2 + 2] = (zfar + znear) / (znear - zfar);
	matrix[4 * 3 + 2] = ((double)2 * zfar * znear) / (znear - zfar);
	matrix[4 * 2 + 3] = -1;

	glLoadIdentity();
	glLoadMatrixd(matrix);
}

void gf_graphic_set_camera(gf_draw_t* draw) {
	GLdouble	 matrix[16];
	gf_math_vector_t f;
	gf_math_vector_t up;
	gf_math_vector_t s;
	gf_math_vector_t u;
	int		 i;

	f[0] = draw->lookat[0] - draw->camera[0];
	f[1] = draw->lookat[1] - draw->camera[1];
	f[2] = draw->lookat[2] - draw->camera[2];
	gf_math_normalize(f);

	up[0] = 0;
	up[1] = 1;
	up[2] = 0;
	gf_math_normalize(up);

	gf_math_multiply(s, f, up);
	gf_math_normalize(s);

	gf_math_multiply(u, s, f);

	for(i = 0; i < 16; i++) matrix[i] = 0;
	matrix[4 * 0 + 0] = s[0];
	matrix[4 * 1 + 0] = s[1];
	matrix[4 * 2 + 0] = s[2];
	matrix[4 * 0 + 1] = u[0];
	matrix[4 * 1 + 1] = u[1];
	matrix[4 * 2 + 1] = u[2];
	matrix[4 * 0 + 2] = -f[0];
	matrix[4 * 1 + 2] = -f[1];
	matrix[4 * 2 + 2] = -f[2];
	matrix[4 * 3 + 3] = 1;

	glLoadIdentity();
	glLoadMatrixd(matrix);
	glTranslated(-draw->camera[0], -draw->camera[1], -draw->camera[2]);
}

void gf_graphic_clip_push(gf_draw_t* draw, double x, double y, double w, double h) {
	double ax = x;
	double ay = y;
	double aw = w;
	double ah = h;
	if(arrlen(draw->clip) > 0) {
		double oldx = draw->clip[arrlen(draw->clip) - 4];
		double oldy = draw->clip[arrlen(draw->clip) - 3];
		double oldw = draw->clip[arrlen(draw->clip) - 2];
		double oldh = draw->clip[arrlen(draw->clip) - 1];

		if(ax < oldx) {
			ax = oldx;
		}
		if(ay < oldy) {
			ay = oldy;
		}
		if(ax + aw > oldx + oldw) {
			aw = (oldx + oldw) - ax;
		}
		if(ay + ah > oldy + oldh) {
			ah = (oldy + oldh) - ay;
		}
	}
	arrput(draw->clip, ax);
	arrput(draw->clip, ay);
	arrput(draw->clip, aw);
	arrput(draw->clip, ah);
	glEnable(GL_SCISSOR_TEST);
	glScissor(ax, draw->height - ay - ah, aw, ah);
}

void gf_graphic_clip_pop(gf_draw_t* draw) {
	double x;
	double y;
	double w;
	double h;
	if(arrlen(draw->clip) > 0) {
		arrdel(draw->clip, arrlen(draw->clip) - 1);
		arrdel(draw->clip, arrlen(draw->clip) - 1);
		arrdel(draw->clip, arrlen(draw->clip) - 1);
		arrdel(draw->clip, arrlen(draw->clip) - 1);
	}
	if(arrlen(draw->clip) == 0) {
		glDisable(GL_SCISSOR_TEST);
		return;
	}

	x = draw->clip[arrlen(draw->clip) - 4];
	y = draw->clip[arrlen(draw->clip) - 3];
	w = draw->clip[arrlen(draw->clip) - 2];
	h = draw->clip[arrlen(draw->clip) - 1];
	glScissor(x, draw->height - y - h, w, h);
}

void gf_graphic_set_point_size(gf_draw_t* draw, double size) { glPointSize(size); }

void gf_graphic_set_line_width(gf_draw_t* draw, double width) { glLineWidth(width); }

double gf_graphic_get_point_size(gf_draw_t* draw) {
	GLfloat n;
	glGetFloatv(GL_POINT_SIZE, &n);
	return n;
}

double gf_graphic_get_line_width(gf_draw_t* draw) {
	GLfloat n;
	glGetFloatv(GL_LINE_WIDTH, &n);
	return n;
}

void gf_opengl_set_light(gf_draw_t* draw);

unsigned long gf_graphic_fast(gf_draw_t* draw, unsigned long id, int npair, double* coords, double* tcoords, double* ncoords, double x, double y, double z, double sx, double sy, double sz) {
	if(id == 0) {
		GLuint* indexes = NULL;
		int	i;
		for(i = 0; i < npair; i++) arrput(indexes, i);

		id = glGenLists(1);
		glNewList(id, GL_COMPILE);
		glPushMatrix();
		glColor3f(1, 1, 1);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		if(ncoords != NULL) {
			glEnableClientState(GL_NORMAL_ARRAY);
		}

		glVertexPointer(3, GL_DOUBLE, 0, coords);
		glTexCoordPointer(2, GL_DOUBLE, 0, tcoords);
		if(ncoords != NULL) glNormalPointer(GL_DOUBLE, 0, ncoords);
		glDrawElements(GL_TRIANGLES, arrlen(indexes), GL_UNSIGNED_INT, indexes);

		if(ncoords != NULL) {
			glDisableClientState(GL_NORMAL_ARRAY);
		}
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glPopMatrix();
		glEndList();

		arrfree(indexes);
	} else {
		glDisable(GL_CULL_FACE);

		glPushMatrix();
		glTranslatef(x, y, z);
		glScalef(sx, sy, sz);
		glRotatef(draw->rot[0], 1, 0, 0);
		glRotatef(draw->rot[1], 0, 1, 0);
		glRotatef(draw->rot[2], 0, 0, 1);
		glCallList(id);
		glPopMatrix();

		glEnable(GL_CULL_FACE);
	}

	return id;
}

void gf_graphic_destroy_fast(gf_draw_t* draw, unsigned long id) { glDeleteLists(id, 1); }

unsigned char* gf_graphic_get_screen(gf_draw_t* draw, int x, int y, int width, int height) {
	unsigned char* r = malloc(width * height * 4);
	int	       fy;
	glReadPixels(x, draw->height - y - height, width, height, GL_RGBA, GL_UNSIGNED_BYTE, r);

	/* image is flipped, so fix it */
	for(fy = 0; fy < height / 2; fy++) {
		int iy = height - fy - 1;
		int fx;
		for(fx = 0; fx < width * 4; fx++) {
			unsigned char old	 = r[(iy * width * 4) + fx];
			r[(iy * width * 4) + fx] = r[(fy * width * 4) + fx];
			r[(fy * width * 4) + fx] = old;
		}
	}

	return r;
}
