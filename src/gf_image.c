#include <gf_pre.h>

/* External library */
#include <stb_image.h>
#include <nanosvg.h>
#include <nanosvgrast.h>

/* Interface */
#include <gf_image.h>

/* Engine */
#include <gf_file.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

unsigned char* gf_image_load(gf_engine_t* engine, const char* path, int* width, int* height) {
	gf_file_t*     f = gf_file_open(engine, path, "r");
	unsigned char* r = NULL;
	unsigned char* b = NULL;
	int	       ch;
	if(f != NULL) {
		b = malloc(f->size + 1);
		gf_file_read(f, b, f->size);
		b[f->size] = 0;

		r = stbi_load_from_memory(b, f->size, width, height, &ch, 4);
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
	}

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

	if(f != NULL) gf_file_close(f);
	if(b != NULL) free(b);
	return r;
}
