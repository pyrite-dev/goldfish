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

gf_gui_id_t gf_gui_create_text(gf_gui_t* gui, double x, double y, double w, double h) {
	gf_gui_component_t c;

	gf_gui_create_component(gui, &c, x, y, w, h);

	c.type = GF_GUI_TEXT;

	hmputs(gui->area, c);

	return c.key;
}

void gf_gui_text_render(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_TEXT) return;
}

void gf_gui_text_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_TEXT) return;
}

void gf_gui_text_click(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_TEXT) return;
}
