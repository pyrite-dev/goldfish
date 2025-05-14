#define GF_EXPOSE_GUI
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_INPUT

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>

/* Interface */
#include <gf_gui.h>

/* Engine */
#include <gf_graphic.h>
#include <gf_prop.h>
#include <gf_math.h>

/* Standard */
#include <stdlib.h>

extern const double gf_gui_border_width;
extern const int    gf_gui_border_color_diff;

gf_gui_id_t gf_gui_create_tab(gf_gui_t* gui, double x, double y, double w, double h) {
	gf_gui_component_t* c = malloc(sizeof(*c));
	gf_gui_id_t	    frame;

	gf_gui_create_component(gui, c, x, y, w, h);

	c->type = GF_GUI_TAB;

	arrput(gui->area, c);

	frame = gf_gui_create_frame(gui, gf_gui_border_width, gf_gui_border_width * 2 + GF_GUI_SMALL_FONT_SIZE, w - gf_gui_border_width * 2, h - gf_gui_border_width * 3 - GF_GUI_SMALL_FONT_SIZE);

	gf_gui_set_parent(gui, frame, c->key);

	gf_prop_set_integer(gf_gui_get_prop(gui, c->key), "selected", 0);

	gf_gui_set_prop_id(gui, c->key, "frame", frame);

	gf_gui_sort_component(gui);

	return c->key;
}

void gf_gui_tab_render(gf_gui_t* gui, gf_gui_component_t* c) {
	double	    cx;
	double	    cy;
	double	    cw;
	double	    ch;
	double	    fx;
	double	    fy;
	double	    fw;
	double	    fh;
	gf_gui_id_t frame;
	int	    i;
	double	    fsz = GF_GUI_SMALL_FONT_SIZE;
	double	    propf;
	double	    x;
	gf_font_t*  font;
	int	    incr;
	if(c->type != GF_GUI_TAB) return;

	font = gf_prop_get_ptr_keep(&c->prop, "font");
	if(font == NULL) font = gui->draw->font;
	if(font == NULL) return;

	if((propf = gf_prop_get_floating(&c->prop, "font-size")) != GF_PROP_NO_SUCH) {
		fsz = propf;
	}

	gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
	gf_graphic_clip_pop(gui->draw);

	frame = gf_gui_get_prop_id(gui, c->key, "frame");

	gf_gui_set_xy(gui, frame, gf_gui_border_width, gf_gui_border_width * 2 + fsz);
	gf_gui_set_wh(gui, frame, cw - gf_gui_border_width * 2, ch - gf_gui_border_width * 3 - fsz);

	gf_gui_calc_xywh(gui, gui->area[gf_gui_get_index(gui, frame)], &fx, &fy, &fw, &fh);
	gf_graphic_clip_pop(gui->draw);

	gf_gui_draw_box(gui, GF_GUI_NORMAL, fx - gf_gui_border_width, fy - gf_gui_border_width, fw + gf_gui_border_width * 2, fh + gf_gui_border_width * 2);

	x    = cx;
	incr = 0;
	for(i = 0; i < arrlen(gui->area); i++) {
		if(gui->area[i]->parent == frame) {
			const char*	   text = gf_prop_get_text(&gui->area[i]->prop, "title");
			double		   w	= gf_graphic_text_width(gui->draw, font, fsz, text);
			double		   h	= gf_graphic_text_height(gui->draw, font, fsz, text);
			double		   bw	= w + gf_gui_border_width * 2 + fsz;
			double		   bh	= fsz + gf_gui_border_width * 2;
			gf_graphic_color_t col	= c->font;

			gf_graphic_clip_push(gui->draw, x, cy, bw, bh - gf_gui_border_width);
			gf_gui_draw_box(gui, GF_GUI_NORMAL, x, cy, bw, bh);
			gf_graphic_clip_pop(gui->draw);
			if(incr == gf_prop_get_integer(&c->prop, "selected")) {
				gf_graphic_fill_rect(gui->draw, x + gf_gui_border_width, cy + fsz + gf_gui_border_width, bw - gf_gui_border_width * 2, gf_gui_border_width, gui->base);
				gf_prop_set_integer(&gui->area[i]->prop, "hide", 0);
				col.r -= gf_gui_border_color_diff * 2;
				col.g += gf_gui_border_color_diff * 2;
				col.b -= gf_gui_border_color_diff * 2;
			} else {
				gf_prop_set_integer(&gui->area[i]->prop, "hide", 1);
			}

			gf_graphic_text(gui->draw, font, x + bw / 2 - w / 2, cy + bh / 2 - h / 2, fsz, text, col);

			x += bw;
			incr++;
		}
	}
}

void gf_gui_tab_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_TAB) return;
}

void gf_gui_tab_click(gf_gui_t* gui, gf_gui_component_t* c) {
	gf_input_t* input = gui->draw->input;
	double	    cx;
	double	    cy;
	double	    cw;
	double	    ch;
	int	    i;
	int	    incr;
	gf_gui_id_t frame;
	double	    fsz = GF_GUI_SMALL_FONT_SIZE;
	double	    propf;
	double	    x;
	gf_font_t*  font;
	if(c->type != GF_GUI_TAB) return;

	font = gf_prop_get_ptr_keep(&c->prop, "font");
	if(font == NULL) font = gui->draw->font;
	if(font == NULL) return;

	if((propf = gf_prop_get_floating(&c->prop, "font-size")) != GF_PROP_NO_SUCH) {
		fsz = propf;
	}

	gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
	gf_graphic_clip_pop(gui->draw);

	frame = gf_gui_get_prop_id(gui, c->key, "frame");

	x    = cx;
	incr = 0;
	for(i = 0; i < arrlen(gui->area); i++) {
		if(gui->area[i]->parent == frame) {
			const char* text = gf_prop_get_text(&gui->area[i]->prop, "title");
			double	    w	 = gf_graphic_text_width(gui->draw, font, fsz, text);
			double	    bw	 = w + gf_gui_border_width * 2 + fsz;
			double	    bh	 = fsz + gf_gui_border_width * 2;
			if((x <= input->mouse_x && input->mouse_x <= (x + bw)) && (cy <= input->mouse_y && input->mouse_y <= (cy + bh))) {
				gf_prop_set_integer(gf_gui_get_prop(gui, c->key), "selected", incr);
				break;
			}

			x += bw;
			incr++;
		}
	}
}
