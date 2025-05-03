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

extern const int gf_gui_border_color_diff;

gf_gui_id_t gf_gui_create_scrollbar(gf_gui_t* gui, double x, double y, double w, double h) {
	gf_gui_component_t c;

	gf_gui_create_component(gui, &c, x, y, w, h);

	c.type = GF_GUI_SCROLLBAR;

	hmputs(gui->area, c);

	return c.key;
}

void gf_gui_scrollbar_render(gf_gui_t* gui, gf_gui_component_t* c) {
	double		   cx;
	double		   cy;
	double		   cw;
	double		   ch;
	int		   prop;
	gf_graphic_color_t col = gui->font;
	if(c->type != GF_GUI_SCROLLBAR) return;

	gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
	gf_graphic_clip_pop(gui->draw);

	col.r += (double)gf_gui_border_color_diff * 3 / 2;
	col.g += (double)gf_gui_border_color_diff * 3 / 2;
	col.b += (double)gf_gui_border_color_diff * 3 / 2;

	gf_graphic_fill_rect(gui->draw, cx, cy, cw, ch, col);
}

void gf_gui_scrollbar_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_SCROLLBAR) return;
}

void gf_gui_scrollbar_click(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_SCROLLBAR) return;
}
