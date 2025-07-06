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
 * File Name:    gf_file.c
 * Author:       Nishi
 *
 * Functionality:
 * Wrap file I/O.
 *
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_FILE
#define GF_EXPOSE_CORE

#include <gf_pre.h>

/* External library */
#ifdef _WIN32
#include <windows.h>
#endif
#include <stb_ds.h>

/* Interface */
#include <gf_file.h>

/* Engine */
#include <gf_resource.h>
#include <gf_util.h>
#include <gf_log.h>

/* Standard */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#ifndef _WIN32
#include <unistd.h>
#endif

static char* file_pick(gf_engine_t* engine, const char* path, int reverse) {
	char** l;
	char*  r	 = NULL;
	int    has_drive = 0;
	int    i;

	if(strlen(path) >= 2) {
		for(i = 0; path[i] != 0; i++) {
			if(path[i] == ':' && (path[i + 1] == '/' || path[i + 1] == '\\')) {
				has_drive = 1;
				break;
			}
		}
	}

	if((strlen(path) >= 3 && path[0] == '.' && path[1] == '.' && path[2] == '/') || (strlen(path) >= 2 && path[0] == '.' && path[1] == '/') || (path[0] == '/') || has_drive) {
		return gf_util_strdup(path);
	}

	l = gf_util_get_search_list(engine);
	if(l != NULL) {
		for(i = (reverse ? (arrlen(l) - 1) : 0); (reverse ? (i >= 0) : (i < arrlen(l))); (reverse ? (i--) : (i++))) {
			char* p = malloc(strlen(l[i]) + 1 + strlen(path) + 1);
			p[0]	= 0;
			strcat(p, l[i]);
			strcat(p, "/");
			strcat(p, path);
			if(gf_file_exists(engine, p)) {
				if(r != NULL) free(r);
				r = p;
			} else {
				free(p);
			}

			free(l[i]);
		}
		arrfree(l);
	}
	return r == NULL ? gf_util_strdup(path) : r;
}

char* gf_file_pick(gf_engine_t* engine, const char* path) { return file_pick(engine, path, 0); }

char* gf_file_pick_reverse(gf_engine_t* engine, const char* path) { return file_pick(engine, path, 1); }

int gf_file_exists(gf_engine_t* engine, const char* path) {
	if(strlen(path) > 6 && memcmp(path, "base:/", 6) == 0) {
		void*  buf;
		size_t len;
		if(engine == NULL || engine->base == NULL) {
			return 0;
		}
		if(gf_resource_get(engine->base, path + 6, &buf, &len) != 0) {
			return 0;
		}
		free(buf);
		return 1;
	} else {
#ifdef _WIN32
		DWORD dw = GetFileAttributes(path);
		return dw != INVALID_FILE_ATTRIBUTES ? 1 : 0;
#else
		return access(path, F_OK) == 0 ? 1 : 0;
#endif
	}
	return 0;
}

gf_file_t* gf_file_open(gf_engine_t* engine, const char* path, const char* mode) {
	gf_resource_t* r = NULL;
	char*	       tmp;
	gf_file_t*     fp = malloc(sizeof(*fp));
	memset(fp, 0, sizeof(*fp));

	/**
	 * path check
	 * FIXME: if resource got registered with name C or something
	 * it might cause problem with Windows
	 *
	 * 1. does path have :?
	 * 2. is string with : and later longer than 2?
	 * 3. is it :/?
	 */
	if((tmp = strchr(path, ':')) != NULL && strlen(tmp) > 2 && memcmp(tmp, ":/", 2) == 0) {
		char* p = gf_util_strdup(path);
		tmp	= strchr(p, ':');
		tmp[0]	= 0;
		r	= shget(engine->resources, p);
		free(p);
	}

	fp->size   = 0;
	fp->pos	   = 0;
	fp->buffer = NULL;
	fp->fp	   = NULL;
	if(r != NULL) {
		if(engine == NULL || r == NULL) {
			free(fp);
			return NULL;
		}
		if(strcmp(mode, "r") == 0) {
			if(gf_resource_get(r, path + 6, &fp->buffer, &fp->size) != 0) {
				free(fp);
				return NULL;
			}
		} else {
			free(fp);
			return NULL;
		}
	} else {
		char* p = gf_file_pick(engine, path);
		if(strcmp(mode, "r") == 0) {
			fp->fp = fopen(p, "rb");
			if(fp->fp == NULL) {
				free(p);
				free(fp);
				return NULL;
			}
			fp->size = gf_util_file_size(fp->fp);
		} else if(strcmp(mode, "w") == 0) {
			fp->fp = fopen(p, "wb");
			if(fp->fp == NULL) {
				free(p);
				free(fp);
				return NULL;
			}
		} else {
			free(p);
			free(fp);
			return NULL;
		}
		free(p);
	}

	return fp;
}

size_t gf_file_read(gf_file_t* fp, void* buffer, size_t size) {
	size_t sz = (fp->size - fp->pos) < size ? (fp->size - fp->pos) : size;
	if(fp->fp != NULL) {
		fread(buffer, sz, 1, fp->fp);
	}
	if(fp->buffer != NULL) {
		memcpy(buffer, (unsigned char*)fp->buffer + fp->pos, sz);
	}
	fp->pos += sz;
	return sz;
}

size_t gf_file_write(gf_file_t* fp, void* buffer, size_t size) {
	size_t sz = size;
	if(fp->fp != NULL) {
		fwrite(buffer, sz, 1, fp->fp);
	}
	fp->pos += sz;
	fp->size += sz;
	return sz;
}

void gf_file_close(gf_file_t* fp) {
	if(fp->fp != NULL) fclose(fp->fp);
	if(fp->buffer != NULL) free(fp->buffer);
	free(fp);
}

#ifdef WIN32
#define PATH_SEPERATOR "\\"
#else
#ifdef __APPLE__
#ifdef __RETRO68__
#define PATH_SEPERATOR ":"
#else
#define PATH_SEPERATOR "/"
#endif
#else
#define PATH_SEPERATOR "/"
#endif
#endif

char* gf_file_path_join(size_t length, ...) {
	long	size = 0;
	int	idx  = 0;
	char*	st   = NULL;
	va_list va;

	va_start(va, length);
	for(idx = 0; idx < length; idx++) {
		size += strlen(va_arg(va, char*)) + 1; /* +1 for the path seperator */
	}
	va_end(va);

	st = malloc(size);
	va_start(va, length);
	for(idx = 0; idx < length; idx++) {
		strcat(st, va_arg(va, char*));
		strcat(st, PATH_SEPERATOR);
	}
	va_end(va);

	/* optimize path */
	for(idx = 0; st[idx] != 0; idx++) {
		/* FIXME: using memcmp for this is horrible idea */
		if(memcmp(&st[idx], PATH_SEPERATOR, 1) == 0) {
			while(memcmp(&st[idx + 1], PATH_SEPERATOR, 1) == 0) {
				int i;
				for(i = idx + 1; st[i] != 0; i++) {
					st[i] = st[i + 1];
				}
			}
		}
	}

	return st;
}

void gf_file_register(gf_engine_t* engine, const char* name, gf_resource_t* resource) {
	gf_log_function(engine, "Registered resource \"%s\"", name);
	shput(engine->resources, name, resource);
}
