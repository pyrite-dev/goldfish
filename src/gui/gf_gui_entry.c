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

	gf_prop_set_integer(&c->prop, "cursor", 0);

	arrput(gui->area, c);

	return c->key;
}

void gf_gui_entry_render(gf_gui_t* gui, gf_gui_component_t* c) {
	gf_input_t* input = gui->draw->input;
	double	    cx;
	double	    cy;
	double	    cw;
	double	    ch;
	double	    loc = 0;
	gf_font_t*  font;
	double	    propf;
	double	    fsz;
	int	    cr;
	if(c->type != GF_GUI_ENTRY) return;

	font = gf_prop_get_ptr_keep(&c->prop, "font");
	if(font == NULL) font = gui->draw->font;
	if(font == NULL) return;

	gf_gui_calc_xywh(gui, c, &cx, &cy, &cw, &ch);
	gf_graphic_clip_pop(gui->draw);

	if((propf = gf_prop_get_floating(&c->prop, "font-size")) != GF_PROP_NO_SUCH) {
		fsz = propf;
	} else {
		fsz = ch - gf_gui_border_width * 4;
	}

	cr = gf_prop_get_integer(&c->prop, "cursor");

	gf_gui_draw_box(gui, GF_GUI_INVERT, cx, cy, cw, ch);

	gf_graphic_clip_push(gui->draw, cx + gf_gui_border_width * 2, cy + gf_gui_border_width * 2, cw - gf_gui_border_width * 4, ch - gf_gui_border_width * 4);
	if(c->text != NULL) {
		char*  before;
		char*  after;
		double tw;
		double x = cx + gf_gui_border_width * 2;
		double y = cy + ch / 2 - fsz / 2;

		if(cr > strlen(c->text)) cr = strlen(c->text);
		if(cr < 0) cr = 0;

		before = malloc(cr + 1);
		after  = malloc(strlen(c->text) - cr + 1);

		memcpy(before, c->text, cr);
		before[cr] = 0;

		memcpy(after, c->text + cr, strlen(c->text) - cr);
		after[strlen(c->text) - cr] = 0;

		tw = gf_graphic_text_width(gui->draw, font, fsz, before);
		gf_graphic_text(gui->draw, font, x, y, fsz, before, c->font);
		x += tw;
		loc = tw;
		gf_graphic_text(gui->draw, font, x, y, fsz, after, c->font);

		free(before);
		free(after);
	}
	gf_graphic_fill_rect(gui->draw, cx + loc + gf_gui_border_width * 2, cy + gf_gui_border_width * 2, 1, ch - gf_gui_border_width * 4, c->font);
	gf_graphic_clip_pop(gui->draw);
}

void gf_gui_entry_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_ENTRY) return;
}

void gf_gui_entry_click(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_ENTRY) return;
}
