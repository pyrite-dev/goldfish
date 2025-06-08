#ifdef __GNUC__
#if __GNUC__ <= 3
#define NO_SVG
#endif
#endif

#include <gf_pre.h>

/* External library */
#include <stb_image.h>
#ifndef NO_SVG
#include <nanosvg.h>
#include <nanosvgrast.h>
#endif

/* Interface */
#include <gf_image.h>

/* Engine */
#include <gf_file.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

unsigned char* gf_image_load_memory(gf_engine_t* engine, unsigned char* data, size_t size, int* width, int* height) {
	unsigned char* b;
	unsigned char* r;
	int	       ch;

	b = malloc(size + 1);
	memcpy(b, data, size);
	b[size] = 0;

	r = stbi_load_from_memory(b, size, width, height, &ch, 4);
#ifndef NO_SVG
	if(r == NULL) {
		NSVGimage* img = nsvgParse(b, "px", 128);
		if(img != NULL) {
			NSVGrasterizer* rast = nsvgCreateRasterizer();

			*width	= img->width;
			*height = img->height;

			r = malloc((*width) * (*height) * 4);
			nsvgRasterize(rast, img, 0, 0, 1, r, (*width), (*height), (*width) * 4);
			nsvgDeleteRasterizer(rast);
			nsvgDelete(img);
		}
	}
#endif
	free(b);

	/* TODO: should we have state to control this behavior? */
	if(r == NULL) {
		int x;
		int y;
		*width	= 64;
		*height = 64;
		r	= malloc((*width) * (*height) * 4);
		memset(r, 0, (*width) * (*height) * 4);
		for(y = 0; y < (*height); y++) {
			for(x = 0; x < (*width); x++) {
				int n	= ((x > ((*width) / 2) ? 1 : 0) + (y > ((*height) / 2) ? 1 : 0)) % 2;
				int ind = (y * (*width) + x) * 4;
				if(n == 1) {
					r[ind + 0] = 255;
					r[ind + 2] = 255;
				}
				r[ind + 3] = 255;
			}
		}
	}

	return r;
}

unsigned char* gf_image_load(gf_engine_t* engine, const char* path, int* width, int* height) {
	gf_file_t*     f = gf_file_open(engine, path, "r");
	unsigned char* r = NULL;
	unsigned char* b = NULL;
	int	       ch;
	if(f != NULL) {
		b = malloc(f->size);
		gf_file_read(f, b, f->size);

		r = gf_image_load_memory(engine, b, f->size, width, height);
	}

	if(f != NULL) gf_file_close(f);
	if(b != NULL) free(b);
	return r;
}
