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

gf_resource_t* gf_resource_create(gf_engine_t* engine, const char* path) {
	FILE*	       f;
	gf_resource_t* resource = malloc(sizeof(*resource));
	int	       is_dir	= 0;
	char*	       p	= path == NULL ? NULL : gf_file_pick(engine, path);
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
	if(dw == INVALID_FILE_ATTRIBUTES) {
		free(p);
		gf_log_function(engine, "Failed to create resource", "");
		gf_resource_destroy(resource);
		return NULL;
	}
	is_dir = (dw & FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0;
#else
	if(stat(p, &s) != 0) {
		free(p);
		gf_log_function(engine, "Failed to create resource", "");
		gf_resource_destroy(resource);
		return NULL;
	}
	is_dir = S_ISDIR(s.st_mode) ? 1 : 0;
#endif
	if(is_dir) {
		gf_log_function(engine, "Created resource", "");
		resource->path = malloc(strlen(p) + 1);
		strcpy(resource->path, p);
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

	while(1) {
		char		    filename[128];
		unsigned char	    n;
		size_t		    sz = 0;
		int		    i;
		gf_resource_entry_t e;
		fread(&filename, 128, 1, f);
		if(filename[0] == 0) break;

		for(i = 0; i < 4; i++) {
			fread(&n, 1, 1, f);
			sz = sz << 8;
			sz = sz | n;
		}

		e.key	 = (char*)&filename[0];
		e.size	 = sz;
		e.ogsize = 0;

		e.cache	     = NULL;
		e.compressed = malloc(sz);
		fread(e.compressed, sz, 1, f);
		shputs(resource->entries, e);

		gf_log_function(engine, "%s: Compressed to %lu bytes", filename, (unsigned long)sz);
	}
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

		f = fopen(path, "r");
		if(f == NULL) {
			free(path);
			return -1;
		}

		fseek(f, 0, SEEK_END);
		*size = ftell(f);
		fseek(f, 0, SEEK_SET);

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
	gf_resource_t*		   resource;
	int			   progress;
	int			   finish_count;
	FILE*			   f;
	char*			   processed;
	struct gf_thread_mutext_t* processed_lock;
	struct gf_thread_mutext_t* f_lock;
	struct gf_thread_event_t*  workers_finished;
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
		// Skipped worked-on threads
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
	char*				       processed; // array of each entry representing whether it is/has been worked on or not yet
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
