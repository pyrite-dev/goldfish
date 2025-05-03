#define GF_EXPOSE_GUI
#define GF_EXPOSE_DRAW

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>

/* Interface */
#include <gf_gui.h>
#include <gf_graphic.h>

/* Engine */
#include <gf_prop.h>

/* Standard */

extern const double gf_gui_border_width;

gf_gui_id_t gf_gui_create_text(gf_gui_t* gui, double x, double y, double w, double h) {
	gf_gui_component_t c;
	gf_gui_id_t	   scroll;

	gf_gui_create_component(gui, &c, x, y, w, h);

	c.type = GF_GUI_TEXT;

	hmputs(gui->area, c);

	scroll = gf_gui_create_scrollbar(gui, gf_gui_border_width, gf_gui_border_width, 20 - gf_gui_border_width, h - gf_gui_border_width * 2);
	gf_prop_set_integer(gf_gui_get_prop(gui, scroll), "x-base", 1);
	gf_gui_set_parent(gui, scroll, c.key);
	gf_gui_set_prop_id(gui, c.key, "scrollbar", scroll);

	return c.key;
}

void gf_gui_text_render(gf_gui_t* gui, gf_gui_component_t* c) {
	double	   cx;
	double	   cy;
	double	   cw;
	double	   ch;
	gf_font_t* font;
	if(c->type != GF_GUI_TEXT) return;

	font = gf_prop_get_ptr_keep(&c->prop, "font");
	if(font == NULL) font = gui->draw->font;
	if(font == NULL) return;

	gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
	gf_graphic_clip_pop(gui->draw);

	gf_gui_draw_box(gui, GF_GUI_INVERT, cx, cy, cw, ch);

	gf_gui_set_wh(gui, gf_gui_get_prop_id(gui, c->key, "scrollbar"), 20 - gf_gui_border_width, c->height - gf_gui_border_width * 2);
}

void gf_gui_text_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_TEXT) return;
}

void gf_gui_text_click(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_TEXT) return;
}
