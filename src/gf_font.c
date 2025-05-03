#define GF_EXPOSE_FONT
#define GF_EXPOSE_DRAW

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>

/* Interface */
#include <gf_font.h>

/* Engine */
#include <gf_log.h>
#include <gf_texture.h>
#include <gf_file.h>

/* Standard */
#include <string.h>
#include <stdlib.h>

gf_font_glyph_t* gf_font_get(gf_font_t* font, int code) {
	int i;
	if(code < 0x20) return NULL;
	for(i = 0; i < font->count; i++) {
		if(font->glyph[i] != NULL && font->glyph[i]->code == code) {
			return font->glyph[i];
		}
	}
	return NULL;
}

gf_texture_t* gf_font_render(gf_font_t* font, const char* text, double size, double* width) {
	int		i;
	double		scale;
	int		ascent;
	int		descent;
	int		linegap;
	int		iheight;
	gf_font_cache_t cache;
	unsigned char*	buffer;

	/* TODO: Implement this */
	if(font->use_glyph) {
		return NULL;
	}
	for(i = 0; i < arrlen(font->cache); i++) {
		if(strcmp(font->cache[i].text, text) == 0 && font->cache[i].size == size) {
			*width = font->cache[i].width;
			return font->cache[i].texture;
		}
	}

	scale	    = stbtt_ScaleForPixelHeight(&font->ttf, size);
	cache.width = 0;

	stbtt_GetFontVMetrics(&font->ttf, &ascent, &descent, &linegap);

	ascent	= ascent * scale;
	descent = descent * scale;

	for(i = 0; i < strlen(text); i++) {
		int ax;
		int lsb;
		stbtt_GetCodepointHMetrics(&font->ttf, text[i], &ax, &lsb);
		cache.width += ax * scale;
	}

	iheight = ascent - descent + linegap;

	buffer = malloc(cache.width * iheight * 4);
	memset(buffer, 0, cache.width * iheight * 4);

	cache.text = malloc(strlen(text) + 1);
	strcpy(cache.text, text);
	cache.size    = size;
	cache.texture = gf_texture_create(font->draw, cache.width, iheight, buffer);
	arrput(font->cache, cache);

	free(buffer);

	*width = cache.width;

	return cache.texture;
}

/**
 * This is a private method to parse BDF line
 */
void gf_font_parse_line(gf_draw_t* draw, const char* path, gf_font_store_t* store, gf_font_t* font, char* line) {
	int   i;
	char* args[32];
	int   argc = 0;
	int   incr = 0;
	int   dq   = 0;
	for(i = 0;; i++) {
		if((dq == 0 && line[i] == ' ') || line[i] == 0) {
			char oldc = line[i];
			line[i]	  = 0;

			args[argc] = line + incr;
			if(args[argc][0] == '"') args[argc]++;
			if(args[argc][strlen(args[argc]) - 1] == '"') args[argc][strlen(args[argc]) - 1] = 0;

			argc++;

			incr = i + 1;

			if(oldc == 0) break;
		} else if(line[i] == '"') {
			dq = 1 - dq;
		}
	}
	if(store->line_index != -1 && store->line_index < font->glyph[store->glyph_index]->bbox.height) {
		int	       wid     = font->glyph[store->glyph_index]->bbox.width;
		unsigned char* linebuf = store->buffer + store->line_index * wid * 4;
		for(i = 0; line[i] != 0; i++) {
			int n = 0;
			int j;
			if('0' <= line[i] && line[i] <= '9') {
				n = line[i] - '0';
			} else if('a' <= line[i] && line[i] <= 'f') {
				n = 10 + line[i] - 'a';
			} else if('A' <= line[i] && line[i] <= 'F') {
				n = 10 + line[i] - 'A';
			}
			for(j = 0; j < (wid > 4 ? 4 : wid); j++) {
				if((n >> 3) & 1) {
					memset(linebuf + 16 * i + 4 * j, 255, 4);
				}
				n = n << 1;
			}
			wid -= 4;
		}
		store->line_index++;
	} else if(argc > 0 && strcmp(args[0], "STARTCHAR") == 0) {
		font->glyph[store->glyph_index] = malloc(sizeof(**font->glyph));
		memset(font->glyph[store->glyph_index], 0, sizeof(**font->glyph));
	} else if(argc > 0 && strcmp(args[0], "ENDCHAR") == 0) {
		font->glyph[store->glyph_index]->texture = gf_texture_create(draw, font->glyph[store->glyph_index]->bbox.width, font->glyph[store->glyph_index]->bbox.height, store->buffer);
		free(store->buffer);
		store->glyph_index++;
		store->line_index = -1;
	} else if(argc > 0 && strcmp(args[0], "BITMAP") == 0) {
		store->line_index = 0;
	} else if(argc == 2) {
		if(strcmp(args[0], "COPYRIGHT") == 0) {
			gf_log_function(NULL, "%s: %s", path, args[1]);
		} else if(strcmp(args[0], "NOTICE") == 0) {
			gf_log_function(NULL, "%s: %s", path, args[1]);
		} else if(strcmp(args[0], "FOUNDRY") == 0) {
			gf_log_function(NULL, "%s: Made by %s", path, args[1]);
		} else if(strcmp(args[0], "CHARS") == 0) {
			int j;
			gf_log_function(NULL, "%s: %s characters", path, args[1]);
			font->count = atoi(args[1]);
			font->glyph = malloc(font->count * sizeof(*font->glyph));
			for(j = 0; j < font->count; j++) font->glyph[j] = NULL;
		} else if(strcmp(args[0], "ENCODING") == 0) {
			font->glyph[store->glyph_index]->code = atoi(args[1]);
		}
	} else if(argc == 3) {
		if(strcmp(args[0], "DWIDTH") == 0) {
			font->glyph[store->glyph_index]->dwidth[0] = atoi(args[1]);
			font->glyph[store->glyph_index]->dwidth[1] = atoi(args[2]);
		}
	} else if(argc == 5) {
		if(strcmp(args[0], "FONTBOUNDINGBOX") == 0) {
			font->bbox.width  = atoi(args[1]);
			font->bbox.height = atoi(args[2]);
			font->bbox.x	  = atoi(args[3]);
			font->bbox.y	  = atoi(args[4]);
		} else if(strcmp(args[0], "BBX") == 0) {
			font->glyph[store->glyph_index]->bbox.width  = atoi(args[1]);
			font->glyph[store->glyph_index]->bbox.height = atoi(args[2]);
			font->glyph[store->glyph_index]->bbox.x	     = atoi(args[3]);
			font->glyph[store->glyph_index]->bbox.y	     = atoi(args[4]);

			store->buffer = malloc(atoi(args[1]) * atoi(args[2]) * 4);
			memset(store->buffer, 0, atoi(args[1]) * atoi(args[2]) * 4);
		}
	}
}

gf_font_t* gf_font_create(gf_draw_t* draw, const char* path, const void* data, size_t size) {
	gf_font_t*	font = malloc(sizeof(*font));
	char*		buf;
	int		i    = 0;
	int		incr = 0;
	gf_font_store_t store;
	store.line_index  = -1;
	store.glyph_index = 0;
	memset(font, 0, sizeof(*font));
	font->count	= 0;
	font->use_glyph = 1;
	font->draw	= draw;

	buf	  = malloc(size + 1);
	buf[size] = 0;
	memcpy(buf, data, size);

	if(stbtt_InitFont(&font->ttf, buf, 0)) {
		font->use_glyph = 0;
		font->buffer	= buf;
		return font;
	}

	for(i = 0;; i++) {
		if(buf[i] == 0 || buf[i] == '\n') {
			char  oldc = buf[i];
			char* line = buf + incr;
			buf[i]	   = 0;
			incr	   = i + 1;

			if(strlen(line) >= 1 && line[strlen(line) - 1] == '\r') {
				line[strlen(line) - 1] = 0;
			}

			gf_font_parse_line(draw, path, &store, font, line);

			if(oldc == 0) break;
		}
	}

	free(buf);
	return font;
}

gf_font_t* gf_font_create_file(gf_draw_t* draw, const char* path) {
	char*	   buf;
	gf_font_t* font;
	gf_file_t* f = gf_file_open(draw->engine, path, "r");
	if(f == NULL) return NULL;

	gf_log_function(NULL, "%s: %lu bytes", path, (unsigned long)f->size);
	buf	     = malloc(f->size + 1);
	buf[f->size] = 0;

	gf_file_read(f, buf, f->size);

	font = gf_font_create(draw, path, buf, f->size);

	gf_file_close(f);
	free(buf);

	return font;
}

void gf_font_destroy(gf_font_t* font) {
	int i;
	if(font->glyph != NULL) {
		for(i = 0; i < font->count; i++) {
			gf_texture_destroy(font->glyph[i]->texture);
		}
		free(font->glyph);
	}
	if(font->cache != NULL) {
		for(i = 0; i < arrlen(font->cache); i++) {
			free(font->cache[i].text);
			gf_texture_destroy(font->cache[i].texture);
		}
		free(font->cache);
	}
	if(font->buffer != NULL) {
		free(font->buffer);
	}
	free(font);
}
