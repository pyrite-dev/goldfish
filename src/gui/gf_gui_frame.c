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

gf_gui_id_t gf_gui_create_frame(gf_gui_t* gui, double x, double y, double w, double h) {
	gf_gui_component_t c;

	gf_gui_create_component(gui, &c, x, y, w, h);

	c.type = GF_GUI_FRAME;

	hmputs(gui->area, c);

	return c.key;
}

void gf_gui_frame_render(gf_gui_t* gui, gf_gui_component_t* c) {
	double	   cx;
	double	   cy;
	double	   cw;
	double	   ch;
	double	   sx;
	double	   sy;
	gf_font_t* font;
	if(c->type != GF_GUI_FRAME) return;
	if(c->text == NULL) return;

	font = gf_prop_get_ptr_keep(&c->prop, "font");
	if(font == NULL) font = gui->draw->font;
	if(font == NULL) return;

	gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
	gf_graphic_clip_pop(gui->draw);

	sx = cw / 2 - gf_graphic_text_width(gui->draw, font, 24, c->text) / 2;
	sy = ch / 2 - 24.0 / 2;

	gf_graphic_clip_push(gui->draw, cx, cy, cw, ch);
	gf_graphic_text(gui->draw, font, cx + sx, cy + sy, 24, c->text, gui->font);
	gf_graphic_clip_pop(gui->draw);
}

void gf_gui_frame_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_FRAME) return;
}

void gf_gui_frame_click(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_FRAME) return;
}
