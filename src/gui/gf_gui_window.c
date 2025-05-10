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
#include <gf_graphic.h>
#include <gf_draw.h>
#include <gf_log.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

extern const double gf_gui_border_width;
extern const int    gf_gui_border_color_diff;

gf_gui_id_t gf_gui_create_window(gf_gui_t* gui, double x, double y, double w, double h) {
	gf_gui_component_t* c = malloc(sizeof(*c));
	gf_gui_id_t	    close_button;
	gf_gui_id_t	    frame;

	gf_gui_create_component(gui, c, x, y, w, h);

	c->type = GF_GUI_WINDOW;

	arrput(gui->area, c);

	close_button = gf_gui_create_button(gui, 5, 5, 20, 20);
	gf_gui_set_parent(gui, close_button, c->key);
	gf_prop_set_integer(gf_gui_get_prop(gui, close_button), "x-base", 1);
	gf_prop_set_integer(gf_gui_get_prop(gui, close_button), "close-parent", 1);
	gf_gui_set_text(gui, close_button, "X");

	frame = gf_gui_create_frame(gui, 5, 10 + GF_GUI_SMALL_FONT_SIZE, w - 10, h - GF_GUI_SMALL_FONT_SIZE - 10 - 5);

	gf_gui_set_parent(gui, frame, c->key);
	gf_prop_set_integer(gf_gui_get_prop(gui, frame), "ignore-mouse", 1);

	gf_gui_set_prop_id(gui, c->key, "frame", frame);

	return c->key;
}

void gf_gui_window_render(gf_gui_t* gui, gf_gui_component_t* c) {
	gf_input_t*	   input = gui->draw->input;
	double		   cx;
	double		   cy;
	double		   cw;
	double		   ch;
	gf_prop_integer_t  prop;
	gf_graphic_color_t col = gui->font;
	gf_gui_id_t	   fid;
	int		   frame;
	gf_font_t*	   font;
	if(c->type != GF_GUI_WINDOW) return;

	font = gf_prop_get_ptr_keep(&c->prop, "font");
	if(font == NULL) font = gui->draw->font;
	if(font == NULL) return;

	gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
	gf_graphic_clip_pop(gui->draw);

	if((prop = gf_prop_get_integer(&c->prop, "active")) == GF_PROP_NO_SUCH || !prop) {
		col.r -= (double)gf_gui_border_color_diff * 3 / 2;
		col.g -= (double)gf_gui_border_color_diff * 3 / 2;
		col.b -= (double)gf_gui_border_color_diff * 3 / 2;
	}

	gf_gui_draw_box(gui, GF_GUI_NORMAL, cx, cy, cw, ch);

	if(c->text != NULL) {
		gf_graphic_clip_push(gui->draw, cx, cy, cw - GF_GUI_SMALL_FONT_SIZE - 10, GF_GUI_SMALL_FONT_SIZE + 10);
		gf_graphic_text(gui->draw, font, cx + 10, cy + 10 - (double)GF_GUI_SMALL_FONT_SIZE / 4, GF_GUI_SMALL_FONT_SIZE, c->text, col);
		gf_graphic_clip_pop(gui->draw);
	}

	fid   = gf_gui_get_prop_id(gui, c->key, "frame");
	frame = gf_gui_get_index(gui, fid);
	if(frame != -1) {
		gf_gui_component_t* cf = gui->area[frame];
		cf->width	       = c->width - 10;
		cf->height	       = c->height - GF_GUI_SMALL_FONT_SIZE - 10 - 5;
		if((prop = gf_prop_get_integer(&c->prop, "resizable")) != GF_PROP_NO_SUCH && prop) {
			cf->height -= GF_GUI_SMALL_FONT_SIZE;
		}
	}
}

void gf_gui_window_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	gf_input_t* input = gui->draw->input;
	if(c->type != GF_GUI_WINDOW) return;

	c->x = input->mouse_x - gf_prop_get_integer(&c->prop, "diff-x");
	c->y = input->mouse_y - gf_prop_get_integer(&c->prop, "diff-y");
}

void gf_gui_window_click(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_WINDOW) return;
}
