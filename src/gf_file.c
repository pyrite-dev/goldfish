#define GF_EXPOSE_FILE
#define GF_EXPOSE_CORE

#include <gf_pre.h>

/* External library */

/* Interface */
#include <gf_file.h>

/* Engine */
#include <gf_resource.h>

/* Standard */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

gf_file_t* gf_file_open(gf_engine_t* engine, const char* path, const char* mode) {
	gf_file_t* fp = malloc(sizeof(*fp));
	memset(fp, 0, sizeof(*fp));

	fp->size   = 0;
	fp->pos	   = 0;
	fp->buffer = NULL;
	fp->fp	   = NULL;
	if(strlen(path) > 6 && memcmp(path, "base:/", 6) == 0) {
		if(engine->base == NULL) {
			free(fp);
			return NULL;
		}
		if(strcmp(mode, "r") == 0) {
			if(gf_resource_get(engine->base, path + 6, &fp->buffer, &fp->size) != 0) {
				free(fp);
				return NULL;
			}
		} else {
			free(fp);
			return NULL;
		}
	} else {
		if(strcmp(mode, "r") == 0) {
			fp->fp = fopen(path, "rb");
			if(fp->fp == NULL) {
				free(fp);
				return NULL;
			}
			fseek(fp->fp, 0, SEEK_END);
			fp->size = ftell(fp->fp);
			fseek(fp->fp, 0, SEEK_SET);
		} else if(strcmp(mode, "w") == 0) {
			fp->fp = fopen(path, "wb");
		} else {
			free(fp);
			return NULL;
		}
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
