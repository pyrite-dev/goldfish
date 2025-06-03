/**
 * @file gf_type/gui.h
 * @~english
 * @brief Type definitions related to GUI
 */

#ifndef __GF_TYPE_GUI_H__
#define __GF_TYPE_GUI_H__

#include <gf_pre.h>
#include <gf_macro.h>

/**
 * @~english
 * @brief Component ID
 */
typedef int gf_gui_id_t;

/**
 * @~english
 * @brief GUI events
 */
enum GF_GUI_EVENTS {
	GF_GUI_PRESS_EVENT = 0,
	GF_GUI_CHANGE_EVENT
};

/**
 * @~english
 * @brief GUI component types
 */
enum GF_GUI_COMPONENT_TYPES {
	GF_GUI_BUTTON = 0,
	GF_GUI_WINDOW,
	GF_GUI_FRAME,
	GF_GUI_TEXT,
	GF_GUI_SCROLLBAR,
	GF_GUI_RANGE,
	GF_GUI_TAB,
	GF_GUI_PROGRESS
};

#ifdef GF_EXPOSE_GUI
typedef struct gf_gui		  gf_gui_t;
typedef struct gf_gui_component gf_gui_component_t;

/* External library */

/* Engine */
#include <gf_type/draw.h>
#include <gf_type/core.h>
#include <gf_type/prop.h>
#include <gf_type/graphic.h>
#include <gf_type/texture.h>

/* Standard */

typedef void (*gf_gui_callback_t)(gf_engine_t* engine, gf_draw_t* draw, gf_gui_id_t id, int type);

/**
 * @struct gf_gui_component_t
 * @~english
 * @brief Component
 *
 * @var gf_gui_component_t::key
 * @brief Component ID
 *
 * @var gf_gui_component_t::type
 * @brief Component type
 * @see GF_GUI_COMPONENT_TYPES
 *
 * @var gf_gui_component_t::x
 * @brief X coord of component
 *
 * @var gf_gui_component_t::y
 * @brief X coord of component
 *
 * @var gf_gui_component_t::width
 * @brief Width of component
 *
 * @var gf_gui_component_t::height
 * @brief Height of component
 *
 * @var gf_gui_component_t::pressed
 * @brief `1` if pressed, otherwise `0`
 *
 * @var gf_gui_component_t::callback
 * @brief Event callback
 *
 * @var gf_gui_component_t::parent
 * @brief Parent GUI component
 *
 * @var gf_gui_component_t::prop
 * @brief Property
 *
 * @var gf_gui_component_t::text
 * @brief Text
 *
 * @var gf_gui_component_t::font
 * @brief Font color
 *
 * @var gf_gui_component_t::hover_font
 * @brief Hover font color
 *
 * @var gf_gui_component_t::texture
 * @brief Texture
 */
GF_DECLARE_TYPE(gui_component, {
	gf_gui_id_t	   key;
	int		   type;
	double		   x;
	double		   y;
	double		   width;
	double		   height;
	int		   pressed;
	gf_gui_id_t	   parent;
	gf_prop_t*	   prop;
	gf_gui_callback_t  callback;
	char*		   text;
	gf_graphic_color_t font;
	gf_graphic_color_t hover_font;
	gf_texture_t* texture;
});

/**
 * @struct gf_gui_t
 * @~english
 * @brief GUI
 *
 * @var gf_gui_t::engine
 * @brief Engine instance
 *
 * @var gf_gui_t::draw
 * @brief Drawing interface
 *
 * @var gf_gui_t::pressed
 * @brief `1` if something is being pressed, otherwise `0`
 *
 * @var gf_gui_t::area
 * @brief Created components
 *
 * @var gf_gui_t::base
 * @brief Base color
 *
 * @var gf_gui_t::font
 * @brief Font color
 *
 * @var gf_gui_t::hover
 * @brief Hover
 *
 * @var gf_gui_t::button_sound
 * @brief Button sound
 */
GF_DECLARE_TYPE(gui, {
	gf_engine_t*	    engine;
	gf_draw_t*	    draw;
	gf_gui_id_t	    pressed;
	gf_gui_component_t** area;
	gf_graphic_color_t  base;
	gf_graphic_color_t  font;
	gf_gui_id_t	    hover;
	char* button_sound;
});
#else
typedef void* gf_gui_callback_t;

typedef void gf_gui_t;
typedef void gf_gui_component_t;
#endif

GF_DECLARE_TYPE(gui_call, {
	char* name;
	gf_gui_id_t (*call)(gf_gui_t* gui, double x, double y, double w, double h);
});

#endif
