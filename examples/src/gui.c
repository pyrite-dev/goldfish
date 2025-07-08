#define GF_EXPOSE_CORE
#define GF_EXPOSE_CLIENT
#define GF_EXPOSE_DRAW

#include <gf_core.h>
#include <gf_draw.h>
#include <gf_gui.h>
#include <gf_prop.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

gf_engine_t* engine;

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <signal.h>

void handler(int sig) { gf_engine_shutdown(engine); }
#endif

#ifndef M_PI
#define M_PI 3.14159265
#endif

int main(int argc, char** argv) {
	gf_engine_param_t param;
#ifndef _WIN32
	signal(SIGINT, handler);
#endif

	param.prefix = "/usr/local";
	param.game   = "GUI viewer";
	param.base   = "data";

	gf_engine_begin();
	engine = gf_engine_create_ex("GUI viewer", 0, param, argv, argc);
	if(engine != NULL) {
		gf_gui_id_t id, id2, id3, id4;
		gf_gui_t*   gui = engine->client->draw->gui;

		id  = gf_gui_create_window(gui, 0, 0, 400, 100);
		id2 = gf_gui_create_button(gui, 0, 0, 100, 25);
		id3 = gf_gui_create_entry(gui, 100, 0, 250, 25);
		id4 = gf_gui_create_progress(gui, 0, 25, 100, 25);
		gf_gui_set_parent(gui, id2, gf_gui_get_prop_id(gui, id, "frame"));
		gf_gui_set_parent(gui, id3, gf_gui_get_prop_id(gui, id, "frame"));
		gf_gui_set_parent(gui, id4, gf_gui_get_prop_id(gui, id, "frame"));
		gf_gui_set_text(gui, id, "Window");
		gf_gui_set_text(gui, id2, "Button");
		gf_gui_set_text(gui, id3, "Entry");
		gf_prop_set_floating(gf_gui_get_prop(gui, id4), "value", 25);

		id = gf_gui_create_window(gui, 0, 100, 400, 100);
		gf_gui_set_text(gui, id, "Resizable window");
		gf_prop_set_integer(gf_gui_get_prop(gui, id), "resizable", 1);

		gf_engine_loop(engine);
		gf_engine_destroy(engine);
	}
	gf_engine_end();
}
