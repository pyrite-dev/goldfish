/**
 * @file gf_gui_static.h
 * @~english
 * @brief GUI static stuff
 * @note User should not include this, this header is used internally
 * @warning This file is auto-generated using ./tool/gengui.sh at `Wed May 14 23:34:27 JST 2025`
 */

#ifndef __GF_GUI_STATIC_H__
#define __GF_GUI_STATIC_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */

/* Engine */
#include <gf_type/gui.h>
#include <gf_gui_component.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GF_GUI_COMPONENTS 7
/**
 * @~english
 * @brief Initialize GUI component calls
 */
static void gf_gui_init_calls(void){
	extern gf_gui_call_t gf_gui_calls[7];
	/*** Begin button ***/
	gf_gui_calls[0].name = malloc(512);
	strcpy(gf_gui_calls[0].name, "button");
	gf_gui_calls[0].call = gf_gui_create_button;
	/***  End  button ***/
	/*** Begin frame ***/
	gf_gui_calls[1].name = malloc(512);
	strcpy(gf_gui_calls[1].name, "frame");
	gf_gui_calls[1].call = gf_gui_create_frame;
	/***  End  frame ***/
	/*** Begin range ***/
	gf_gui_calls[2].name = malloc(512);
	strcpy(gf_gui_calls[2].name, "range");
	gf_gui_calls[2].call = gf_gui_create_range;
	/***  End  range ***/
	/*** Begin scrollbar ***/
	gf_gui_calls[3].name = malloc(512);
	strcpy(gf_gui_calls[3].name, "scrollbar");
	gf_gui_calls[3].call = gf_gui_create_scrollbar;
	/***  End  scrollbar ***/
	/*** Begin tab ***/
	gf_gui_calls[4].name = malloc(512);
	strcpy(gf_gui_calls[4].name, "tab");
	gf_gui_calls[4].call = gf_gui_create_tab;
	/***  End  tab ***/
	/*** Begin text ***/
	gf_gui_calls[5].name = malloc(512);
	strcpy(gf_gui_calls[5].name, "text");
	gf_gui_calls[5].call = gf_gui_create_text;
	/***  End  text ***/
	/*** Begin window ***/
	gf_gui_calls[6].name = malloc(512);
	strcpy(gf_gui_calls[6].name, "window");
	gf_gui_calls[6].call = gf_gui_create_window;
	/***  End  window ***/
}
/**
 * @~english
 * @brief Run `render` procedure for all components
 * @param gui GUI
 * @param c Component
 */
static void gf_gui_all_render(gf_gui_t* gui, gf_gui_component_t* c){
	extern void gf_gui_button_render(gf_gui_t* gui, gf_gui_component_t* c);
	extern void gf_gui_frame_render(gf_gui_t* gui, gf_gui_component_t* c);
	extern void gf_gui_range_render(gf_gui_t* gui, gf_gui_component_t* c);
	extern void gf_gui_scrollbar_render(gf_gui_t* gui, gf_gui_component_t* c);
	extern void gf_gui_tab_render(gf_gui_t* gui, gf_gui_component_t* c);
	extern void gf_gui_text_render(gf_gui_t* gui, gf_gui_component_t* c);
	extern void gf_gui_window_render(gf_gui_t* gui, gf_gui_component_t* c);
	gf_gui_button_render(gui, c);
	gf_gui_frame_render(gui, c);
	gf_gui_range_render(gui, c);
	gf_gui_scrollbar_render(gui, c);
	gf_gui_tab_render(gui, c);
	gf_gui_text_render(gui, c);
	gf_gui_window_render(gui, c);
}

/**
 * @~english
 * @brief Run `drag` procedure for all components
 * @param gui GUI
 * @param c Component
 */
static void gf_gui_all_drag(gf_gui_t* gui, gf_gui_component_t* c){
	extern void gf_gui_button_drag(gf_gui_t* gui, gf_gui_component_t* c);
	extern void gf_gui_frame_drag(gf_gui_t* gui, gf_gui_component_t* c);
	extern void gf_gui_range_drag(gf_gui_t* gui, gf_gui_component_t* c);
	extern void gf_gui_scrollbar_drag(gf_gui_t* gui, gf_gui_component_t* c);
	extern void gf_gui_tab_drag(gf_gui_t* gui, gf_gui_component_t* c);
	extern void gf_gui_text_drag(gf_gui_t* gui, gf_gui_component_t* c);
	extern void gf_gui_window_drag(gf_gui_t* gui, gf_gui_component_t* c);
	gf_gui_button_drag(gui, c);
	gf_gui_frame_drag(gui, c);
	gf_gui_range_drag(gui, c);
	gf_gui_scrollbar_drag(gui, c);
	gf_gui_tab_drag(gui, c);
	gf_gui_text_drag(gui, c);
	gf_gui_window_drag(gui, c);
}

/**
 * @~english
 * @brief Run `click` procedure for all components
 * @param gui GUI
 * @param c Component
 */
static void gf_gui_all_click(gf_gui_t* gui, gf_gui_component_t* c){
	extern void gf_gui_button_click(gf_gui_t* gui, gf_gui_component_t* c);
	extern void gf_gui_frame_click(gf_gui_t* gui, gf_gui_component_t* c);
	extern void gf_gui_range_click(gf_gui_t* gui, gf_gui_component_t* c);
	extern void gf_gui_scrollbar_click(gf_gui_t* gui, gf_gui_component_t* c);
	extern void gf_gui_tab_click(gf_gui_t* gui, gf_gui_component_t* c);
	extern void gf_gui_text_click(gf_gui_t* gui, gf_gui_component_t* c);
	extern void gf_gui_window_click(gf_gui_t* gui, gf_gui_component_t* c);
	gf_gui_button_click(gui, c);
	gf_gui_frame_click(gui, c);
	gf_gui_range_click(gui, c);
	gf_gui_scrollbar_click(gui, c);
	gf_gui_tab_click(gui, c);
	gf_gui_text_click(gui, c);
	gf_gui_window_click(gui, c);
}

#ifdef __cplusplus
}
#endif

#endif
