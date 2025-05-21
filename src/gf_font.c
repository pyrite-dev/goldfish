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
#include <gf_math.h>
#include <gf_unicode.h>
#include <gf_type/compat.h>

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

gf_texture_t* gf_font_render(gf_font_t* font, const char* text, double size, double* width, double* height) {
	int		i;
	double		scale;
	int		ascent;
	int		descent;
	int		linegap;
	gf_font_cache_t cache;
	unsigned char*	buffer;
	int		ix;
	int		iy;
	int		last = 0;
	gf_int32_t*	wc;
	int		incr  = 0;
	const char*	texts = text;

	double lw = *width;
	double lh = *height;

	/* TODO: Implement this */
	if(font->use_glyph) {
		return NULL;
	}

	for(i = 0; i < arrlen(font->cache); i++) {
		if(strcmp(font->cache[i].text, text) == 0 && font->cache[i].size == size && font->cache[i].lw == lw && font->cache[i].lh == lh) {
			*width	= font->cache[i].width;
			*height = font->cache[i].height;
			return font->cache[i].texture;
		}
	}

	wc   = malloc(strlen(text) * sizeof(*wc) * 5 + 1);
	incr = 0;
	while(texts[0] != 0) {
		int sz = gf_unicode_8_to_32(texts, wc + incr);
		if(sz == 0) {
			free(wc);
			wc    = malloc(9 * sizeof(*wc));
			wc[0] = '(';
			wc[1] = 'I';
			wc[2] = 'N';
			wc[3] = 'V';
			wc[4] = 'U';
			wc[5] = 'T';
			wc[6] = 'F';
			wc[7] = '8';
			wc[8] = ')';
			wc[9] = 0;
			break;
		}
		incr++;
		wc[incr] = 0;
		texts += sz;
	}

	stbtt_GetFontVMetrics(&font->ttf, &ascent, &descent, &linegap);

	scale	   = (double)size / (ascent - descent);
	cache.size = size;

	ascent	= gf_math_ceil(ascent * scale);
	descent = gf_math_ceil(descent * scale);
	linegap = gf_math_ceil(linegap * scale);

	cache.width  = 0;
	cache.height = 0;
	cache.lw     = lw;
	cache.lh     = lh;

	ix	     = 0;
	iy	     = 0;
	cache.height = ascent - descent + linegap;
	for(i = 0; wc[i] != 0; i++) {
		int ax;
		int lsb;
		int kern;
		int x1;
		int y1;
		int x2;
		int y2;
		int fx;

		if(wc[i] == '\n') {
			int gh	    = ascent - descent + linegap;
			cache.width = 0;
			iy += gh;
			cache.height += gh;
		} else {
			stbtt_GetCodepointBitmapBox(&font->ttf, wc[i], scale, scale, &x1, &y1, &x2, &y2);

			stbtt_GetCodepointHMetrics(&font->ttf, wc[i], &ax, &lsb);
			kern = 0;
			if(wc[i + 1] != 0) kern = stbtt_GetCodepointKernAdvance(&font->ttf, wc[i], wc[i + 1]);
			fx   = gf_math_round(ax * scale) + gf_math_round(kern * scale);
			last = lsb;

			if(lw >= 0 && (cache.width + fx) >= lw) {
				int gh	    = ascent - descent + linegap;
				cache.width = 0;
				iy += gh;
				cache.height += gh;
			}
			cache.width += fx;
		}

		if(lh >= 0 && iy >= lh) {
			cache.height = lh;
			break;
		}
	}

	if(lw >= 0) {
		cache.width = lw;
	} else {
		cache.width += gf_math_round(last * scale);
	}

	buffer = malloc(cache.width * cache.height * 4);
	memset(buffer, 0, cache.width * cache.height * 4);

	ix = 0;
	iy = 0;
	for(i = 0; wc[i] != 0; i++) {
		int	       ax;
		int	       lsb;
		int	       x1;
		int	       y1;
		int	       x2;
		int	       y2;
		int	       kern;
		int	       gx;
		int	       gy;
		unsigned char* gbuf;
		int	       y;
		int	       fx;

		if(wc[i] == '\n') {
			ix = 0;
			iy += ascent - descent + linegap;
		} else {
			stbtt_GetCodepointHMetrics(&font->ttf, wc[i], &ax, &lsb);
			stbtt_GetCodepointBitmapBox(&font->ttf, wc[i], scale, scale, &x1, &y1, &x2, &y2);

			kern = 0;
			if(wc[i + 1] != 0) kern = stbtt_GetCodepointKernAdvance(&font->ttf, wc[i], wc[i + 1]);
			fx = gf_math_round(ax * scale) + gf_math_round(kern * scale);

			if(lw >= 0 && (ix + fx) >= lw) {
				ix = 0;
				iy += ascent - descent + linegap;
			}
			y = iy + ascent + y1;

			gbuf = malloc((x2 - x1) * (y2 - y1));
			stbtt_MakeCodepointBitmap(&font->ttf, gbuf, x2 - x1, y2 - y1, x2 - x1, scale, scale, wc[i]);

			for(gy = 0; gy < (y2 - y1); gy++) {
				if((y + gy) >= cache.height) {
					break;
				}
				for(gx = 0; gx < (x2 - x1); gx++) {
					int c	= gbuf[gy * (x2 - x1) + gx];
					int ind = (ix + gx) + gf_math_round(lsb * scale) + ((gy + y) * cache.width);
					if((ix + gx) + gf_math_round(lsb * scale) >= cache.width) {
						break;
					}
					ind *= 4;
					buffer[ind + 0] = 255;
					buffer[ind + 1] = 255;
					buffer[ind + 2] = 255;
					if(buffer[ind + 3] >= (255 - c)) {
						buffer[ind + 3] = 255;
					} else {
						buffer[ind + 3] += c;
					}
				}
			}
			free(gbuf);
			ix += fx;
		}
	}

	cache.text = malloc(strlen(text) + 1);
	strcpy(cache.text, text);
	cache.texture = gf_texture_create(font->draw, cache.width, cache.height, buffer);
	arrput(font->cache, cache);

	free(buffer);
	free(wc);

	*width	= cache.width;
	*height = cache.height;

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
	unsigned char	ttf_magic[5];
	gf_font_store_t store;
	store.line_index  = -1;
	store.glyph_index = 0;
	memset(font, 0, sizeof(*font));
	font->count	= 0;
	font->use_glyph = 1;
	font->draw	= draw;

	memset(ttf_magic, 0, 5);
	ttf_magic[1] = 1;

	buf	  = malloc(size + 1);
	buf[size] = 0;
	memcpy(buf, data, size);

	if(size > 5 && memcmp(data, ttf_magic, 5) == 0 && stbtt_InitFont(&font->ttf, buf, 0)) {
		font->use_glyph = 0;
		font->buffer	= buf;
		arrput(draw->loaded_fonts, font);
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
	arrput(draw->loaded_fonts, font);
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
	for(i = 0; i < arrlen(font->draw->loaded_fonts); i++) {
		if(font->draw->loaded_fonts[i] == font) {
			arrdel(font->draw->loaded_fonts, i);
			i--;
		}
	}
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
		arrfree(font->cache);
	}
	if(font->buffer != NULL) {
		free(font->buffer);
	}
	free(font);
}
