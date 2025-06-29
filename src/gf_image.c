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
