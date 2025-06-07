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
#include <gf_input.h>
#include <gf_util.h>

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
	int	    prop;
	double	    propf;
	double	    fsz;
	int	    cr;
	double	    shift   = 0;
	int	    focused = 0;
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

	focused = ((prop = gf_prop_get_integer(&c->prop, "focus")) != GF_PROP_NO_SUCH && prop) ? 1 : 0;

	gf_gui_draw_box(gui, GF_GUI_INVERT, cx, cy, cw, ch);

	gf_graphic_clip_push(gui->draw, cx + gf_gui_border_width * 2, cy + gf_gui_border_width * 2, cw - gf_gui_border_width * 4, ch - gf_gui_border_width * 4);
	if(c->text != NULL) {
		char*  before;
		char*  after;
		double tw;
		double x     = cx + gf_gui_border_width * 2;
		double y     = cy + ch / 2 - fsz / 2;
		int    oldcr = cr;

		if(cr > strlen(c->text)) cr = strlen(c->text);
		if(cr < 0) cr = 0;
		if(oldcr != cr) gf_prop_set_integer(&c->prop, "cursor", cr);

		before = malloc(cr + 1);
		after  = malloc(strlen(c->text) - cr + 1);

		memcpy(before, c->text, cr);
		before[cr] = 0;

		memcpy(after, c->text + cr, strlen(c->text) - cr);
		after[strlen(c->text) - cr] = 0;

		if((tw = gf_graphic_text_width(gui->draw, font, fsz, c->text)) > (cw - gf_gui_border_width * 5)) {
			shift += tw - (cw - gf_gui_border_width * 5);
		}

		gf_graphic_text(gui->draw, font, x - shift, y, fsz, c->text, c->font);

		tw = gf_graphic_text_width(gui->draw, font, fsz, before);
		x += tw;
		loc = tw;

		free(before);
		free(after);

		if(focused) {
			if(gf_input_key_pressed(input, GF_INPUT_KEY_LEFT) && cr > 0) {
				gf_prop_set_integer(&c->prop, "cursor", cr - 1);
			} else if(gf_input_key_pressed(input, GF_INPUT_KEY_RIGHT) && cr < strlen(c->text)) {
				gf_prop_set_integer(&c->prop, "cursor", cr + 1);
			} else if(gf_input_key_pressed(input, GF_INPUT_KEY_BACKSPACE) && strlen(c->text) > 0) {
				char* t;
				c->text[strlen(c->text) - 1] = 0;
				t			     = gf_util_strdup(c->text);
				free(c->text);
				c->text = t;
			} else if(gf_input_key_pressed(input, GF_INPUT_KEY_DELETE) && strlen(c->text) > 0) {
				char* t;
				int   i;
				for(i = cr; i < strlen(c->text); i++) {
					c->text[i] = c->text[i + 1];
				}
				t = gf_util_strdup(c->text);
				free(c->text);
				c->text = t;
			}
		}
	}
	if(focused) gf_graphic_fill_rect(gui->draw, cx + loc + gf_gui_border_width * 2 - shift, cy + gf_gui_border_width * 2, 1, ch - gf_gui_border_width * 4, c->font);
	gf_graphic_clip_pop(gui->draw);
}

void gf_gui_entry_drag(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_ENTRY) return;
}

void gf_gui_entry_click(gf_gui_t* gui, gf_gui_component_t* c) {
	if(c->type != GF_GUI_ENTRY) return;
}
