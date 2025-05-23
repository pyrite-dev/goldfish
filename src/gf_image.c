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

unsigned char* gf_image_load(gf_engine_t* engine, const char* path, int* width, int* height) {
	gf_file_t*     f = gf_file_open(engine, path, "r");
	unsigned char* r;
	unsigned char* b;
	int	       ch;
	if(f == NULL) return NULL;
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

	gf_file_close(f);
	free(b);
	return r;
}
