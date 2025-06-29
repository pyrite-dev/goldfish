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
 * File Name:    gf_resource.c
 * Author:       Nishi
 *
 * Functionality:
 * TODO: Write here
 *
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_RESOURCE

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>
#include <zlib.h>

/* Interface */
#include <gf_resource.h>

/* Engine */
#include <gf_core.h>
#include <gf_log.h>
#include <gf_file.h>
#include <gf_thread.h>
#include <gf_util.h>
#include <gf_type/thread.h>

/* Standard */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#ifndef _WIN32
#include <sys/stat.h>
#endif

#define CHUNK 32767
#define NUM_THREADS 8

static void gf_resource_fread(void* out, size_t size, void* ud) { fread(out, size, 1, (FILE*)ud); }

static void gf_resource_rread(void* out, size_t size, void* ud) { gf_file_read((gf_file_t*)ud, out, size); }

static void gf_resource_read(gf_resource_t* resource, void* ud, void (*func)(void*, size_t, void*)) {
	while(1) {
		char		    filename[128];
		unsigned char	    n;
		size_t		    sz = 0;
		int		    i;
		gf_resource_entry_t e;
		func(&filename, 128, ud);
		if(filename[0] == 0) break;

		for(i = 0; i < 4; i++) {
			func(&n, 1, ud);
			sz = sz << 8;
			sz = sz | n;
		}

		e.key	 = (char*)&filename[0];
		e.size	 = sz;
		e.ogsize = 0;

		e.cache	     = NULL;
		e.compressed = malloc(sz);
		func(e.compressed, sz, ud);
		shputs(resource->entries, e);

		gf_log_function(resource->engine, "%s: Compressed to %lu bytes", filename, (unsigned long)sz);
	}
}

gf_resource_t* gf_resource_create(gf_engine_t* engine, const char* path) {
	FILE*	       f;
	gf_resource_t* resource = malloc(sizeof(*resource));
	int	       is_dir	= 0;
	char*	       p	= path == NULL ? NULL : gf_file_pick(engine, path);
	gf_file_t*     file;
#ifdef _WIN32
	DWORD dw;
#else
	struct stat s;
#endif

	memset(resource, 0, sizeof(*resource));
	resource->engine  = engine;
	resource->entries = NULL;
	sh_new_strdup(resource->entries);

	if(path == NULL) {
		gf_log_function(engine, "Created empty resource", "");
		return resource;
	}

	is_dir = 0;
#ifdef _WIN32
	dw = GetFileAttributes(p);
	if(dw != INVALID_FILE_ATTRIBUTES) is_dir = (dw & FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0;
#else
	if(stat(p, &s) == 0) is_dir = S_ISDIR(s.st_mode) ? 1 : 0;
#endif
	if(is_dir) {
		gf_log_function(engine, "Created resource", "");
		resource->path = malloc(strlen(p) + 1);
		strcpy(resource->path, p);
		free(p);
		return resource;
	}

	file = gf_file_open(engine, path, "r");
	if(file != NULL) {
		gf_resource_read(resource, file, gf_resource_rread);
		gf_file_close(file);
		free(p);
		return resource;
	}

	f = fopen(p, "rb");

	if(f == NULL) {
		gf_log_function(engine, "Failed to create resource", "");
		gf_resource_destroy(resource);
		return NULL;
	}

	gf_log_function(engine, "Created resource", "");

	gf_resource_read(resource, f, gf_resource_fread);
	fclose(f);
	free(p);

	return resource;
}

int gf_resource_get(gf_resource_t* resource, const char* name, void** data, size_t* size) {
	int		     ind;
	gf_resource_entry_t* e;

	if(resource->path != NULL) {
		char* path = malloc(strlen(resource->path) + 1 + strlen(name) + 1);
		FILE* f;

		path[0] = 0;
		strcpy(path + strlen(path), resource->path);
		strcpy(path + strlen(path), "/");
		strcpy(path + strlen(path), name);

		f = fopen(path, "rb");
		if(f == NULL) {
			free(path);
			return -1;
		}

		*size = gf_util_file_size(f);

		*data = malloc(*size);
		fread(*data, *size, 1, f);

		fclose(f);

		free(path);

		gf_log_function(resource->engine, "%s: File found", name);

		return 0;
	}

	ind = shgeti(resource->entries, name);
	if(ind == -1) return -1;

	*size = 0;
	*data = NULL;

	e = &resource->entries[ind];
	if(e->cache == NULL) {
		unsigned char  out[CHUNK];
		z_stream       stream;
		int	       ret;
		int	       have;
		size_t	       dsz = e->size;
		unsigned char* ptr = e->compressed;

		e->ogsize = 0;
		e->cache  = NULL;

		stream.zalloc	= Z_NULL;
		stream.zfree	= Z_NULL;
		stream.opaque	= Z_NULL;
		stream.avail_in = 0;
		stream.next_in	= Z_NULL;
		inflateInit(&stream);
		gf_log_function(resource->engine, "%s: Not cached, decompressing", name);
		do {
			size_t rds	= CHUNK > dsz ? dsz : CHUNK;
			stream.avail_in = rds;
			if(stream.avail_in == 0) break;
			stream.next_in = ptr;
			do {
				stream.avail_out = CHUNK;
				stream.next_out	 = out;
				ret		 = inflate(&stream, Z_NO_FLUSH);
				switch(ret) {
				case Z_NEED_DICT:
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					inflateEnd(&stream);
					if(e->cache != NULL) {
						free(e->cache);
						e->cache = NULL;
					}
					e->ogsize = 0;
					return -1;
				}
				have = CHUNK - stream.avail_out;

				if(e->cache == NULL) {
					e->cache = malloc(have);
					memcpy(e->cache, out, have);
				} else {
					unsigned char* old = e->cache;
					e->cache	   = malloc(e->ogsize + have);

					memcpy(e->cache, old, e->ogsize);
					memcpy(e->cache + e->ogsize, out, have);
					free(old);
				}

				e->ogsize += have;
			} while(stream.avail_out == 0);
			ptr += rds;
			dsz -= rds;
		} while(ret != Z_STREAM_END);
		inflateEnd(&stream);
		gf_log_function(resource->engine, "%s: Compression rate is %.2f%%", name, (double)e->ogsize / e->size * 100);
	} else {
		gf_log_function(resource->engine, "%s: Using cache", name);
	}

	if(e->cache != NULL) {
		*data = malloc(e->ogsize);
		*size = e->ogsize;

		memcpy(*data, e->cache, *size);
		return 0;
	}

	return -1;
}

void gf_resource_add(gf_resource_t* resource, const char* name, void* data, size_t size) {
	gf_resource_entry_t e;
	if(resource->path != NULL) return;
	if(size == 0) return;

	e.key	 = (char*)name;
	e.size	 = 0;
	e.ogsize = size;

	e.compressed = NULL;
	e.cache	     = malloc(size);
	memcpy(e.cache, data, size);
	shputs(resource->entries, e);
}

struct gf_resource_write_worker_args_t {
	gf_resource_t*	   resource;
	int		   progress;
	int		   finish_count;
	FILE*		   f;
	char*		   processed;
	gf_thread_mutex_t* processed_lock;
	gf_thread_mutex_t* f_lock;
	gf_thread_event_t* workers_finished;
};

void gf_resource_write_worker(void* args) {
	struct gf_resource_write_worker_args_t* wargs = (struct gf_resource_write_worker_args_t*)args;

	int	      i;
	unsigned char fn[128];

	for(i = 0; i < shlen(wargs->resource->entries); i++) {
		int		     j;
		size_t		     sz	  = 0;
		size_t		     sz2  = 0;
		char*		     data = NULL;
		gf_resource_entry_t* e;
		/* Skipped worked-on threads */
		gf_thread_mutex_lock(wargs->processed_lock);
		if(wargs->processed[i]) {
			gf_thread_mutex_unlock(wargs->processed_lock);
			continue;
		}
		wargs->processed[i] = 1;
		gf_thread_mutex_unlock(wargs->processed_lock);

		e = &wargs->resource->entries[i];

		if(e->size != 0) {
			sz = e->size;
		} else {
			unsigned char  out[CHUNK];
			int	       flush;
			z_stream       stream;
			size_t	       dsz = e->ogsize;
			unsigned char* ptr = e->cache;
			int	       have;

			stream.zalloc = Z_NULL;
			stream.zfree  = Z_NULL;
			stream.opaque = Z_NULL;
			deflateInit(&stream, Z_DEFAULT_COMPRESSION);

			do {
				size_t wts	= CHUNK > dsz ? dsz : CHUNK;
				stream.avail_in = wts;
				stream.next_in	= ptr;
				flush		= CHUNK >= dsz ? Z_FINISH : Z_NO_FLUSH;
				do {
					stream.avail_out = CHUNK;
					stream.next_out	 = out;
					deflate(&stream, flush);
					have = CHUNK - stream.avail_out;

					if(data == NULL) {
						data = malloc(have);
						memcpy(data, out, have);
					} else {
						unsigned char* old = data;
						data		   = malloc(sz + have);
						memcpy(data, old, sz);
						memcpy(data + sz, out, have);
						free(old);
					}

					sz += have;
				} while(stream.avail_out == 0);
				ptr += wts;
				dsz -= wts;
			} while(flush != Z_FINISH);
			deflateEnd(&stream);
		}

		gf_thread_mutex_lock(wargs->f_lock);
		memset(fn, 0, 128);
		strcpy(fn, e->key);
		fwrite(fn, 128, 1, wargs->f);

		sz2 = sz;
		for(j = 0; j < 4; j++) {
			unsigned char c = 0;
			c		= ((sz >> 24) & 0xff);
			fwrite(&c, 1, 1, wargs->f);

			sz = sz << 8;
		}
		sz = sz2;

		if(e->size != 0) {
			fwrite(e->compressed, sz, 1, wargs->f);
		} else if(data != NULL) {
			fwrite(data, sz, 1, wargs->f);
			e->compressed = data;
			e->size	      = sz;
		}

		if(wargs->progress && data != NULL) {
			printf("%s ... %.2f%%\n", e->key, (double)e->ogsize / e->size * 100);
		}

		gf_thread_mutex_unlock(wargs->f_lock);
	}

	wargs->finish_count++;
	if(wargs->finish_count >= NUM_THREADS) {
		gf_thread_event_signal(wargs->workers_finished);
	}
}

void gf_resource_write(gf_resource_t* resource, const char* path, int progress) {
	int				       j;
	FILE*				       f;
	unsigned char			       fn[128];
	char*				       processed; /* array of each entry representing whether it is/has been worked on or not yet */
	gf_thread_mutex_t*		       processed_lock;
	gf_thread_mutex_t*		       f_lock;
	gf_thread_event_t*		       workers_finished;
	struct gf_resource_write_worker_args_t wargs;
	if(resource->path != NULL) return;

	f = fopen(path, "wb");
	if(f == NULL) return;

	processed_lock	 = gf_thread_mutex_create();
	f_lock		 = gf_thread_mutex_create();
	workers_finished = gf_thread_event_create();

	j	  = sizeof(char) * shlen(resource->entries);
	processed = malloc(j);
	memset(processed, 0, j);

	wargs.resource	       = resource;
	wargs.progress	       = progress;
	wargs.finish_count     = 0;
	wargs.f		       = f;
	wargs.processed	       = processed;
	wargs.processed_lock   = processed_lock;
	wargs.f_lock	       = f_lock;
	wargs.workers_finished = workers_finished;

	for(j = 0; j < NUM_THREADS; j++) {
		gf_thread_create(gf_resource_write_worker, &wargs);
	}

	gf_thread_event_wait(workers_finished);

	gf_thread_mutex_destroy(processed_lock);
	gf_thread_mutex_destroy(f_lock);
	gf_thread_event_destroy(workers_finished);
	free(processed);

	memset(fn, 0, 128);
	fwrite(fn, 128, 1, f);
	fclose(f);
}

void gf_resource_destroy(gf_resource_t* resource) {
	if(resource->entries != NULL) {
		int i;
		for(i = 0; i < shlen(resource->entries); i++) {
			gf_resource_entry_t* e = &resource->entries[i];
			if(e->cache != NULL) free(e->cache);
			if(e->compressed != NULL) free(e->compressed);
		}
		shfree(resource->entries);
	}
	if(resource->path != NULL) {
		free(resource->path);
	}
	gf_log_function(resource->engine, "Destroyed resource", "");
	free(resource);
}
