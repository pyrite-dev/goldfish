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
 * File Name:    gf_image.c
 * Author:       Nishi
 *
 * Functionality:
 * Handle images.
 *
 **********************************************************************************
 * @endif
 */

#ifdef __GNUC__
#if __GNUC__ <= 3
#define NO_SVG
#endif
#endif

#include <gf_pre.h>

/* External library */
#ifdef GF_NO_STBI
#include <png.h>
#include <jpeglib.h>
#include <jerror.h>
#else
#include <stb_image.h>
#endif
#ifndef NO_SVG
#include <nanosvg.h>
#include <nanosvgrast.h>
#endif

/* Interface */
#include <gf_image.h>

/* Engine */
#include <gf_file.h>
#include <gf_log.h>
#include <gf_type/compat.h>

/* Standard */
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#ifdef GF_NO_STBI
static void jpeg_err(j_common_ptr cinfo) {}

static unsigned char* gf_image_load_jpeg(gf_engine_t* engine, unsigned char* data, size_t size, int* width, int* height) {
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr	      jerr;
	unsigned char*		      r = NULL;

	if(!(size > 3 && data[0] == 0xff && data[1] == 0xd8 && data[2] == 0xff)) return NULL;

	cinfo.err	    = jpeg_std_error(&jerr);
	jerr.output_message = jpeg_err;
	jerr.error_exit	    = jpeg_err;

	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, data, size);
	if(jpeg_read_header(&cinfo, TRUE) == JPEG_HEADER_OK) {
		unsigned char* row;
		int	       y;
		jpeg_start_decompress(&cinfo);
		*width	= cinfo.output_width;
		*height = cinfo.output_height;

		gf_log_function(engine, "libjpeg %dx%d", (*width), (*height));

		r = malloc(cinfo.output_width * cinfo.output_height * 4);

		row = malloc(cinfo.output_width * 3);
		for(y = 0; y < cinfo.output_height; y++) {
			int i;
			jpeg_read_scanlines(&cinfo, &row, 1);
			for(i = 0; i < cinfo.output_width; i++) {
				unsigned char cr			= row[i * cinfo.num_components + 0];
				unsigned char cg			= row[i * cinfo.num_components + 1];
				unsigned char cb			= row[i * cinfo.num_components + 2];
				r[(y * cinfo.output_width + i) * 4 + 0] = cr;
				r[(y * cinfo.output_width + i) * 4 + 1] = cg;
				r[(y * cinfo.output_width + i) * 4 + 2] = cb;
				r[(y * cinfo.output_width + i) * 4 + 3] = 0xff;
			}
		}
		free(row);
		jpeg_finish_decompress(&cinfo);
	} else {
		gf_log_function(engine, "libjpeg error triggered", "");
	}
	jpeg_destroy_decompress(&cinfo);

	return r;
}

typedef struct pngud {
	unsigned char* data;
	size_t	       remain;
	size_t	       progress;
} pngud_t;

static void png_read_process(png_structp png, png_bytep out, png_size_t bytes) {
	/* unsafe */
	pngud_t* ud = (pngud_t*)png_get_io_ptr(png);
	size_t	 r  = (ud->remain > bytes) ? bytes : ud->remain;
	if(ud == NULL) return;

	memcpy(out, ud->data + ud->progress, r);
	ud->remain -= r;
	ud->progress += r;
}

static unsigned char* gf_image_load_png(gf_engine_t* engine, unsigned char* data, size_t size, int* width, int* height) {
	png_structp    png;
	png_infop      info;
	pngud_t	       ud;
	int	       depth;
	int	       type;
	png_bytep*     rows;
	unsigned char* r = NULL;
	int	       i;

	if(!(size > 8 && data[0] == 0x89 && data[1] == 0x50 && data[2] == 0x4e && data[3] == 0x47 && data[4] == 0x0d && data[5] == 0x0a && data[6] == 0x1a && data[7] == 0x0a)) return NULL;

	png  = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	info = png_create_info_struct(png);

	ud.data	    = data;
	ud.remain   = size;
	ud.progress = 0;

	if(setjmp(png_jmpbuf(png))) {
		gf_log_function(engine, "libpng error triggered", "");
		png_destroy_read_struct(&png, &info, NULL);
		return NULL;
	}
	png_set_read_fn(png, &ud, png_read_process);
	png_read_info(png, info);

	*width	= png_get_image_width(png, info);
	*height = png_get_image_height(png, info);
	depth	= png_get_bit_depth(png, info);
	type	= png_get_color_type(png, info);

	gf_log_function(engine, "libpng %dx%d", (*width), (*height));

	if(depth == 16) png_set_strip_16(png);
	if(type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
	if(type == PNG_COLOR_TYPE_GRAY && depth < 8) png_set_expand_gray_1_2_4_to_8(png);
	if(png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
	if(type == PNG_COLOR_TYPE_RGB || type == PNG_COLOR_TYPE_GRAY || type == PNG_COLOR_TYPE_PALETTE) png_set_filter(png, 0xFF, PNG_FILLER_AFTER);
	if(type == PNG_COLOR_TYPE_GRAY || type == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png);
	png_read_update_info(png, info);

	rows = (png_bytep*)malloc(sizeof(*rows) * (*height));
	for(i = 0; i < (*height); i++) {
		rows[i] = (png_bytep)malloc(png_get_rowbytes(png, info));
	}

	png_read_image(png, rows);

	r = malloc((*width) * (*height) * 4);
	for(i = 0; i < (*height); i++) {
		int j;
		for(j = 0; j < (*width); j++) {
			png_bytep byte = &rows[i][j * 4];
			memcpy(r + (i * (*width) + j) * 4, byte, 4);
		}
	}

	for(i = 0; i < (*height); i++) {
		free(rows[i]);
	}
	free(rows);

	png_destroy_read_struct(&png, &info, NULL);

	return r;
}
#endif

unsigned char* gf_image_load_memory(gf_engine_t* engine, unsigned char* data, size_t size, int* width, int* height) {
	unsigned char* b;
	unsigned char* r;
	int	       ch;

	b = malloc(size + 1);
	memcpy(b, data, size);
	b[size] = 0;

#ifdef GF_NO_STBI
	r = gf_image_load_jpeg(engine, b, size, width, height);
	if(r == NULL) r = gf_image_load_png(engine, b, size, width, height);
#else
	r = stbi_load_from_memory(b, size, width, height, &ch, 4);
#endif
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

	return r;
}

unsigned char* gf_image_load(gf_engine_t* engine, const char* path, int* width, int* height) {
	gf_file_t*     f = gf_file_open(engine, path, "r");
	unsigned char* r = NULL;
	unsigned char* b = NULL;
	if(f != NULL) {
		b = malloc(f->size);
		gf_file_read(f, b, f->size);

		r = gf_image_load_memory(engine, b, f->size, width, height);
	}

	if(f != NULL) gf_file_close(f);
	if(b != NULL) free(b);
	return r;
}
