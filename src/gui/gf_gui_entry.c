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

gf_gui_id_t gf_gui_create_entry(gf_gui_t* gui, double x, double y, double w, double h) {
	gf_gui_component_t* c = malloc(sizeof(*c));

	gf_gui_create_component(gui, c, x, y, w, h);

	c->type = GF_GUI_ENTRY;

	arrput(gui->area, c);

	return c->key;
}

void gf_gui_entry_render(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_ENTRY) return;
}

void gf_gui_entry_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_ENTRY) return;
}

void gf_gui_entry_click(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_ENTRY) return;
}
