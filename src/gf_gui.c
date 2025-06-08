#define GF_EXPOSE_GUI
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_INPUT

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>

/* Interface */
#include <gf_gui.h>

/* Engine */
#include <gf_prop.h>
#include <gf_gui_component.h>
#include <gf_gui_static.h>
#include <gf_graphic.h>
#include <gf_draw.h>
#include <gf_log.h>
#include <gf_texture.h>
#include <gf_math.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

const double gf_gui_border_width      = 2;
const int    gf_gui_border_color_diff = 48;

gf_gui_call_t gf_gui_calls[GF_GUI_COMPONENTS];

gf_gui_t* gf_gui_create(gf_engine_t* engine, gf_draw_t* draw) {
	gf_gui_t* gui = malloc(sizeof(*gui));
	memset(gui, 0, sizeof(*gui));
	gui->engine	  = engine;
	gui->draw	  = draw;
	gui->area	  = NULL;
	gui->button_sound = NULL;

	gui->pressed = -1;

	GF_SET_COLOR(gui->base, 80, 88, 64, 255);
	GF_SET_COLOR(gui->font, 256 - 32, 256 - 32, 256 - 32, 255);

	return gui;
}

void gf_gui_destroy(gf_gui_t* gui) {
	if(gui->area != NULL) {
		while(arrlen(gui->area) > 0) {
			gf_gui_destroy_id(gui, gui->area[0]->key);
		}
		arrfree(gui->area);
	}
	gf_log_function(gui->engine, "Destroyed GUI", "");
	free(gui);
}

void gf_gui_set_button_sound(gf_gui_t* gui, const char* path) {
	if(gui->button_sound != NULL) free(gui->button_sound);

	gui->button_sound = malloc(strlen(path) + 1);
	strcpy(gui->button_sound, path);
}

int gf_gui_get_index(gf_gui_t* gui, gf_gui_id_t id) {
	int i;
	for(i = 0; i < arrlen(gui->area); i++) {
		if(gui->area[i]->key == id) {
			return i;
		}
	}
	return -1;
}

void gf_gui_destroy_id(gf_gui_t* gui, gf_gui_id_t id) {
	int		    i;
	gf_prop_integer_t   prop;
	int		    ind = gf_gui_get_index(gui, id);
	gf_gui_component_t* c;
	if(ind == -1) return;

	c = gui->area[ind];
	for(i = 0; i < arrlen(gui->area); i++) {
		if(gui->area[i]->parent == id) {
			gf_gui_destroy_id(gui, gui->area[i]->key);
			i = -1;
		}
	}

	if((prop = gf_prop_get_integer(&c->prop, "active")) != GF_PROP_NO_SUCH && prop) {
		for(i = arrlen(gui->area) - 1; i >= 0; i--) {
			gf_gui_component_t* c = gui->area[i];
			if(c->parent == -1 && c->key != id) {
				gf_prop_set_integer(&c->prop, "active", 1);
				break;
			}
		}
	}

	if(c->texture != NULL) {
		gf_texture_destroy(c->texture);
	}

	if(c->text != NULL) {
		free(c->text);
	}

	if(c->prop != NULL) {
		gf_prop_destroy(&c->prop);
	}

	arrdel(gui->area, ind);
}

/* note... left top should be the lightest in the border */

void gf_gui_draw_box(gf_gui_t* gui, int mul, double x, double y, double w, double h) {
	gf_graphic_color_t col;

	int cd = mul * gf_gui_border_color_diff;

	col = gui->base;
	col.r += cd;
	col.g += cd;
	col.b += cd;
	gf_graphic_fill_rect(gui->draw, x, y, w, h, col);

	col = gui->base;
	col.r -= cd;
	col.g -= cd;
	col.b -= cd;
	gf_graphic_fill_polygon(gui->draw, col, GF_GRAPHIC_2D, 5, x + w, y + h, x + w, y, x + w - gf_gui_border_width, y + gf_gui_border_width, x + gf_gui_border_width, y + h - gf_gui_border_width, x, y + h);

	col = gui->base;
	gf_graphic_fill_rect(gui->draw, x + gf_gui_border_width, y + gf_gui_border_width, w - gf_gui_border_width * 2, h - gf_gui_border_width * 2, col);
}

void gf_gui_create_component(gf_gui_t* gui, gf_gui_component_t* c, double x, double y, double w, double h) {
	int ind;

	c->key = 0;
	do {
		int i;
		ind = -1;
		for(i = 0; i < arrlen(gui->area); i++) {
			if(gui->area[i]->key == c->key) {
				ind = i;
				break;
			}
		}
		if(ind != -1) {
			c->key++;
		}
	} while(ind != -1);
	c->x	    = gf_math_round(x);
	c->y	    = gf_math_round(y);
	c->width    = w;
	c->height   = h;
	c->parent   = -1;
	c->pressed  = 0;
	c->prop	    = NULL;
	c->callback = NULL;
	c->text	    = NULL;
	c->texture  = NULL;
	gf_prop_create(&c->prop);

	memcpy(&c->font, &gui->font, sizeof(gui->font));
	memcpy(&c->hover_font, &gui->font, sizeof(gui->font));
}

void gf_gui_calc_xywh_noset(gf_gui_t* gui, gf_gui_component_t* c, double* x, double* y, double* w, double* h) {
	double		  pw = 0;
	double		  ph = 0;
	double		  bx = 0;
	double		  by = 0;
	double		  cx = 0;
	double		  cy = 0;
	double		  cw = 0;
	double		  ch = 0;
	double		  mul;
	int		  hp = 0;
	gf_prop_integer_t prop;

	if(c->parent != -1) {
		double x2  = 0;
		double y2  = 0;
		double w2  = 0;
		double h2  = 0;
		int    ind = gf_gui_get_index(gui, c->parent);
		if(ind != -1) {
			gf_gui_calc_xywh_noset(gui, gui->area[ind], &x2, &y2, &w2, &h2);
			gf_graphic_clip_pop(gui->draw);
		}

		pw = w2;
		ph = h2;

		bx = x2;
		by = y2;

		hp = 1;
	} else {
		pw = gui->draw->width;
		ph = gui->draw->height;
	}

	*x += bx;
	mul = 1;
	if((prop = gf_prop_get_integer(&c->prop, "x-base")) == GF_PROP_NO_SUCH || prop == -1) {
	} else if((prop = gf_prop_get_integer(&c->prop, "x-base")) != GF_PROP_NO_SUCH && prop == 1) {
		*x += pw - c->width;
		mul = -1;
	}
	*x += mul * c->x;

	*y += by;
	mul = 1;
	if((prop = gf_prop_get_integer(&c->prop, "y-base")) == GF_PROP_NO_SUCH || prop == -1) {
	} else if((prop = gf_prop_get_integer(&c->prop, "y-base")) != GF_PROP_NO_SUCH && prop == 1) {
		*y += ph - c->height;
		mul = -1;
	}
	*y += mul * c->y;

	if((*w) < c->width) {
		*w = c->width;
	}

	if((*h) < c->height) {
		*h = c->height;
	}

	cx = *x;
	cy = *y;
	cw = *w;
	ch = *h;

	if(hp && pw < (*w)) {
		cw = pw;
	}
	if(hp && ph < (*h)) {
		ch = ph;
	}
	gf_graphic_clip_push(gui->draw, cx, cy, cw, ch);
}

void gf_gui_calc_xywh(gf_gui_t* gui, gf_gui_component_t* c, double* x, double* y, double* w, double* h) {
	*x = 0;
	*y = 0;
	*w = 0;
	*h = 0;
	gf_gui_calc_xywh_noset(gui, c, x, y, w, h);
}

int gf_gui_hidden(gf_gui_t* gui, gf_gui_id_t id) {
	int ind = gf_gui_get_index(gui, id);
	while(ind != -1) {
		int prop;
		if((prop = gf_prop_get_integer(&gui->area[ind]->prop, "hide")) != GF_PROP_NO_SUCH && prop) {
			return 1;
		}
		ind = gf_gui_get_index(gui, gui->area[ind]->parent);
	}
	return 0;
}

void gf_gui_render(gf_gui_t* gui) {
	int		  i;
	gf_input_t*	  input = gui->draw->input;
	double		  cx;
	double		  cy;
	double		  cw;
	double		  ch;
	gf_prop_integer_t prop;
	gui->hover = -1;
	for(i = arrlen(gui->area) - 1; i >= 0; i--) {
		gf_gui_component_t* c		 = gui->area[i];
		int		    ignore_mouse = (prop = gf_prop_get_integer(&c->prop, "ignore-mouse")) != GF_PROP_NO_SUCH && prop;
		gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
		gf_graphic_clip_pop(gui->draw);

		ignore_mouse = ignore_mouse || gf_gui_hidden(gui, c->key);

		if(!ignore_mouse && input->mouse_x != -1 && input->mouse_y != -1 && gui->pressed == -1 && (input->mouse_flag & GF_INPUT_MOUSE_LEFT_MASK) && (cx <= input->mouse_x && input->mouse_x <= cx + cw) && (cy <= input->mouse_y && input->mouse_y <= cy + ch)) {
			gui->pressed = c->key;
			gui->hover   = c->key;
			gf_prop_set_integer(&c->prop, "clicked-x", input->mouse_x);
			gf_prop_set_integer(&c->prop, "clicked-y", input->mouse_y);
			gf_prop_set_integer(&c->prop, "diff-x", input->mouse_x - cx);
			gf_prop_set_integer(&c->prop, "diff-y", input->mouse_y - cy);
			if((prop = gf_prop_get_integer(&c->prop, "resizable")) != GF_PROP_NO_SUCH && prop) {
				gf_prop_set_integer(&c->prop, "old-width", cw);
				gf_prop_set_integer(&c->prop, "old-height", ch);
			}
		} else if(!ignore_mouse && input->mouse_x != -1 && input->mouse_y != -1 && gui->hover == -1 && (cx <= input->mouse_x && input->mouse_x <= cx + cw) && (cy <= input->mouse_y && input->mouse_y <= cy + ch)) {
			gui->hover = c->key;
		} else if(gui->pressed == -1) {
			c->pressed = 0;
			gf_prop_delete(&c->prop, "cancel-drag");
			if(gf_prop_get_integer(&c->prop, "min-width") == GF_PROP_NO_SUCH) {
				gf_prop_set_integer(&c->prop, "min-width", 0);
			}
			if(gf_prop_get_integer(&c->prop, "min-height") == GF_PROP_NO_SUCH) {
				gf_prop_set_integer(&c->prop, "min-height", 0);
			}
		}
	}
	if(gui->pressed == -1 && (input->mouse_flag & GF_INPUT_MOUSE_LEFT_MASK)) {
		/* dummy value for "background" */
		gui->pressed = -2;
	}
	for(i = 0; i < arrlen(gui->area); i++) {
		gf_gui_component_t* c = gui->area[i];
		if(gf_gui_hidden(gui, c->key)) continue;

		gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);

		gf_graphic_clip_push(gui->draw, cx, cy, cw, ch);
		gf_gui_all_render(gui, c);
		gf_graphic_clip_pop(gui->draw);
		gf_graphic_clip_pop(gui->draw);

		if((prop = gf_prop_get_integer(&c->prop, "resizable")) != GF_PROP_NO_SUCH && prop) {
			int j;
			for(j = 0; j < 3; j++) {
				double		   sp  = 5;
				double		   bw  = gf_gui_border_width * 2;
				double		   aln = (double)18 / 3 - bw;
				double		   rx  = cx + cw - sp - j * bw - j * aln;
				double		   ry  = cy + ch - sp - j * bw - j * aln;
				gf_graphic_color_t col = gui->base;

				col.r -= gf_gui_border_color_diff;
				col.g -= gf_gui_border_color_diff;
				col.b -= gf_gui_border_color_diff;

				gf_graphic_fill_polygon(gui->draw, col, GF_GRAPHIC_2D, 4, cx + cw - sp, ry - bw, rx - bw, cy + ch - sp, rx - bw / 2.0, cy + ch - sp, cx + cw - sp, ry - bw / 2.0);

				col = gui->base;
				col.r += gf_gui_border_color_diff;
				col.g += gf_gui_border_color_diff;
				col.b += gf_gui_border_color_diff;

				gf_graphic_fill_polygon(gui->draw, col, GF_GRAPHIC_2D, 4, cx + cw - sp, ry - bw / 2.0, rx - bw / 2.0, cy + ch - sp, rx, cy + ch - sp, cx + cw - sp, ry);
			}
		}
	}

	/* drag */
	if(gui->pressed != -1 && (input->mouse_flag & GF_INPUT_MOUSE_LEFT_MASK)) {
		int ind;
		ind = gf_gui_get_index(gui, gui->pressed);
		if(ind != -1) {
			gf_gui_component_t* c = gui->area[ind];
			gf_gui_id_t	    id;
			int		    cancel = 0;
			int		    spc;
			if((prop = gf_prop_get_integer(&c->prop, "resizable")) != GF_PROP_NO_SUCH && prop) {
				double sp = 5;
				double sz = 18;
				c->width  = gf_prop_get_integer(&c->prop, "old-width");
				c->height = gf_prop_get_integer(&c->prop, "old-height");
				gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
				gf_graphic_clip_pop(gui->draw);
				if((prop = gf_prop_get_integer(&c->prop, "cancel-drag")) == GF_PROP_NO_SUCH) {
					cancel = 1;
					cancel = cancel && ((cx + cw - sp - sz) <= gf_prop_get_integer(&c->prop, "clicked-x"));
					cancel = cancel && ((cx + cw) >= gf_prop_get_integer(&c->prop, "clicked-x"));
					cancel = cancel && ((cy + ch - sp - sz) <= gf_prop_get_integer(&c->prop, "clicked-y"));
					cancel = cancel && ((cy + ch) >= gf_prop_get_integer(&c->prop, "clicked-y"));
					gf_prop_set_integer(&c->prop, "cancel-drag", cancel);
				} else {
					cancel = gf_prop_get_integer(&c->prop, "cancel-drag");
				}
				if(cancel) {
					c->width  = input->mouse_x - gf_prop_get_integer(&c->prop, "clicked-x") + gf_prop_get_integer(&c->prop, "old-width");
					c->height = input->mouse_y - gf_prop_get_integer(&c->prop, "clicked-y") + gf_prop_get_integer(&c->prop, "old-height");
					if((prop = gf_prop_get_integer(&c->prop, "min-width")) != GF_PROP_NO_SUCH && c->width < 20 + 10 + sz + prop) {
						c->width = 20 + 10 + sz + prop;
					}
					if((prop = gf_prop_get_integer(&c->prop, "min-height")) != GF_PROP_NO_SUCH && c->height < 20 + 10 + sz + prop) {
						c->height = 20 + 10 + sz + prop;
					}
				}
			}
			if(!cancel) {
				gf_gui_all_drag(gui, c);
			}
			if(c->parent != -1 || c->type == GF_GUI_WINDOW) {
				gf_gui_move_topmost(gui, c->key);
			}
			id  = c->key;
			spc = (c->parent == -1 && c->type == GF_GUI_ENTRY) ? 1 : 0;
			for(i = 0; i < arrlen(gui->area); i++) {
				c = gui->area[i];
				gf_prop_set_integer(&c->prop, "focus", id == c->key ? 1 : 0);
				if(spc) gf_prop_set_integer(&c->prop, "active", id == c->key ? 1 : 0);
			}
		}
	}

	/* click */
	if((gui->pressed != -1) && !(input->mouse_flag & GF_INPUT_MOUSE_LEFT_MASK)) {
		int ind;
		ind = gf_gui_get_index(gui, gui->pressed);
		if(ind != -1) {
			gf_gui_component_t* c = gui->area[ind];

			gf_gui_all_click(gui, c);
		}
		gui->pressed = -1;
	}
}

void gf_gui_set_callback(gf_gui_t* gui, gf_gui_id_t id, gf_gui_callback_t callback) {
	int ind = gf_gui_get_index(gui, id);
	if(ind == -1) return;

	gui->area[ind]->callback = callback;
}

void gf_gui_set_parent(gf_gui_t* gui, gf_gui_id_t id, gf_gui_id_t parent) {
	int ind = gf_gui_get_index(gui, id);
	if(ind == -1) return;

	gui->area[ind]->parent = parent;
}

gf_gui_id_t gf_gui_get_parent(gf_gui_t* gui, gf_gui_id_t id) {
	int ind = gf_gui_get_index(gui, id);
	if(ind == -1) return -1;

	return gui->area[ind]->parent;
}

void gf_gui_set_text(gf_gui_t* gui, gf_gui_id_t id, const char* text) {
	int ind = gf_gui_get_index(gui, id);
	if(ind == -1) return;

	if(gui->area[ind]->text != NULL) free(gui->area[ind]->text);
	gui->area[ind]->text = malloc(strlen(text) + 1);
	strcpy(gui->area[ind]->text, text);
}

const char* gf_gui_get_text(gf_gui_t* gui, gf_gui_id_t id) {
	int ind = gf_gui_get_index(gui, id);
	if(ind == -1) return NULL;

	return gui->area[ind]->text;
}

void gf_gui_add_recursive(gf_gui_t* gui, gf_gui_component_t*** pnew, gf_gui_id_t parent) {
	int i;

	for(i = 0; i < arrlen(gui->area); i++) {
		gf_gui_component_t* c = gui->area[i];
		if(c->parent == parent) {
			arrput(*pnew, c);
			gf_gui_add_recursive(gui, pnew, c->key);
		}
	}
}

void gf_gui_sort_component(gf_gui_t* gui) {
	int i;
	gf_gui_component_t** new = NULL;

	for(i = 0; i < arrlen(gui->area); i++) {
		gf_gui_component_t* c = gui->area[i];
		if(c->parent == -1 && c->type != GF_GUI_WINDOW) {
			gf_prop_set_integer(&c->prop, "active", 0);
			arrput(new, c);
			gf_gui_add_recursive(gui, &new, c->key);
		}
	}

	for(i = 0; i < arrlen(gui->area); i++) {
		gf_gui_component_t* c = gui->area[i];
		if(c->parent == -1 && c->type == GF_GUI_WINDOW) {
			gf_prop_set_integer(&c->prop, "active", 0);
			arrput(new, c);
			gf_gui_add_recursive(gui, &new, c->key);
		}
	}

	arrfree(gui->area);
	gui->area = new;
	new	  = NULL;

	for(i = arrlen(gui->area) - 1; i >= 0; i--) {
		gf_gui_component_t* c = gui->area[i];
		if(c->parent == -1) {
			gf_prop_set_integer(&c->prop, "active", 1);
			break;
		}
	}
}

void gf_gui_move_topmost(gf_gui_t* gui, gf_gui_id_t id) {
	int i;
	int ind;
	gf_gui_component_t** new = NULL;

	for(i = 0; i < arrlen(gui->area); i++) {
		gf_gui_component_t* c = gui->area[i];
		if(c->parent == -1 && c->key != id) {
			gf_prop_set_integer(&c->prop, "active", 0);
			arrput(new, c);
			gf_gui_add_recursive(gui, &new, c->key);
		}
	}

	ind = gf_gui_get_index(gui, id);
	if(ind != -1) {
		gf_gui_component_t* c = gui->area[ind];
		if(c->parent == -1) {
			gf_prop_set_integer(&c->prop, "active", 1);
			arrput(new, c);
			gf_gui_add_recursive(gui, &new, c->key);
		}
	}

	arrfree(gui->area);
	gui->area = new;

	ind = gf_gui_get_index(gui, id);
	if(ind != -1) {
		gf_gui_component_t* c = gui->area[ind];
		gf_gui_id_t	    p;
		if(c->parent == -1) return;
		while(1) {
			p = c->parent;

			ind = gf_gui_get_index(gui, p);
			if(ind != -1) {
				c = gui->area[ind];
			}
			if(c->parent == -1) break;
		}
		gf_gui_move_topmost(gui, p);
	}
}

void gf_gui_set_prop_id(gf_gui_t* gui, gf_gui_id_t id, const char* key, gf_gui_id_t id2) {
	int   ind = gf_gui_get_index(gui, id);
	void* ptr;
	if(ind == -1) return;
	ptr = malloc(sizeof(id2));
	memcpy(ptr, &id2, sizeof(id2));
	gf_prop_set_ptr(&gui->area[ind]->prop, key, ptr);
}

gf_gui_id_t gf_gui_get_prop_id(gf_gui_t* gui, gf_gui_id_t id, const char* key) {
	int   ind = gf_gui_get_index(gui, id);
	void* ptr;
	if(ind == -1) return GF_PROP_NO_SUCH;
	ptr = gf_prop_get_ptr(&gui->area[ind]->prop, key);
	if(ptr == NULL) return GF_PROP_NO_SUCH;
	return *(gf_gui_id_t*)ptr;
}

gf_prop_t** gf_gui_get_prop(gf_gui_t* gui, gf_gui_id_t id) {
	int ind = gf_gui_get_index(gui, id);
	if(ind == -1) return NULL;
	return &gui->area[ind]->prop;
}

void gf_gui_set_font_color(gf_gui_t* gui, gf_gui_id_t id, gf_graphic_color_t color) {
	int ind = gf_gui_get_index(gui, id);
	if(ind == -1) return;
	memcpy(&gui->area[ind]->font, &color, sizeof(color));
}

gf_graphic_color_t gf_gui_get_font_color(gf_gui_t* gui, gf_gui_id_t id) {
	int ind = gf_gui_get_index(gui, id);
	if(ind == -1) return gui->font;
	return gui->area[ind]->font;
}

void gf_gui_set_hover_font_color(gf_gui_t* gui, gf_gui_id_t id, gf_graphic_color_t color) {
	int ind = gf_gui_get_index(gui, id);
	if(ind == -1) return;
	memcpy(&gui->area[ind]->hover_font, &color, sizeof(color));
}

gf_graphic_color_t gf_gui_get_hover_font_color(gf_gui_t* gui, gf_gui_id_t id) {
	int ind = gf_gui_get_index(gui, id);
	if(ind == -1) return gui->font;
	return gui->area[ind]->hover_font;
}

gf_gui_id_t gf_gui_create_common(gf_gui_t* gui, const char* name, double x, double y, double w, double h) {
	int i;
	for(i = 0; i < sizeof(gf_gui_calls) / sizeof(gf_gui_calls[0]); i++) {
		if(strcmp(gf_gui_calls[i].name, name) == 0) {
			return gf_gui_calls[i].call(gui, x, y, w, h);
		}
	}
	return -1;
}

void gf_gui_set_wh(gf_gui_t* gui, gf_gui_id_t id, double w, double h) {
	int ind = gf_gui_get_index(gui, id);
	if(ind == -1) return;
	gui->area[ind]->width  = w;
	gui->area[ind]->height = h;
}

void gf_gui_get_wh(gf_gui_t* gui, gf_gui_id_t id, double* w, double* h) {
	int ind = gf_gui_get_index(gui, id);
	if(ind == -1) return;
	*w = gui->area[ind]->width;
	*h = gui->area[ind]->height;
}

void gf_gui_set_xy(gf_gui_t* gui, gf_gui_id_t id, double x, double y) {
	int ind = gf_gui_get_index(gui, id);
	if(ind == -1) return;
	gui->area[ind]->x = x;
	gui->area[ind]->y = y;
}

void gf_gui_get_xy(gf_gui_t* gui, gf_gui_id_t id, double* x, double* y) {
	int ind = gf_gui_get_index(gui, id);
	if(ind == -1) return;
	*x = gui->area[ind]->x;
	*y = gui->area[ind]->y;
}
