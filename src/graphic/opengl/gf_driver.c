#define GF_EXPOSE_DRAW_DRIVER
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_TEXTURE

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
	if(draw->driver->npot) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
	memset(draw_driver, 0, sizeof(*draw_driver));
	draw_driver->engine = engine;

	gf_log_function(engine, "OpenGL renderer: %s", (char*)glGetString(GL_RENDERER));

	glEnable(GL_BLEND);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glCullFace(GL_BACK);

	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightgry);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightwht);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightwht);

	glClearColor(0, 0, 0, 1);

	glPointSize(5);

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
	gf_graphic_perspective(draw, 30, 1.0, 1000.0);

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
}

void gf_draw_driver_after(gf_draw_t* draw) { glFlush(); }
