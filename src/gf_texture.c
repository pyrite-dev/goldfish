#define GF_EXPOSE_TEXTURE
#define GF_EXPOSE_DRAW_DRIVER

#include <gf_pre.h>

/* External library */

/* Interface */
#include <gf_texture.h>

/* Engine */
#include <gf_draw_driver.h>

/* Standard */
#include <stdlib.h>

gf_texture_t* gf_texture_create(gf_draw_t* draw, int width, int height, unsigned char* data) {
	gf_texture_t*		  texture = malloc(sizeof(*texture));
	gf_draw_driver_texture_t* ddtexture;
	texture->internal_width	 = width;
	texture->internal_height = height;
	ddtexture		 = gf_draw_driver_register_texture(draw, width, height, &texture->internal_width, &texture->internal_height, data);
	if(ddtexture == NULL) {
		free(texture);
		return NULL;
	}
	texture->width		     = width;
	texture->height		     = height;
	texture->draw_driver_texture = ddtexture;
	texture->keep_aspect	     = 0;
	return texture;
}

void gf_texture_destroy(gf_texture_t* texture) {
	gf_draw_driver_destroy_texture(texture->draw_driver_texture);
	free(texture);
}

void gf_texture_keep_aspect(gf_texture_t* texture, int keep) { texture->keep_aspect = keep; }
