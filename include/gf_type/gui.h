/**
 * @if SHOW_LICENSE
 **********************************************************************************
 *
 * Copyright (c) 2025 Pyrite Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. All advertising materials mentioning features or use of this software must
 *    display the following acknowledgement:
 *    This product includes software developed by the Pyrite Development Team.
 *
 * 4. Neither the name of Pyrite Development Team nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 **********************************************************************************
 * @endif
 */

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
	GF_GUI_CHANGE_EVENT,
	GF_GUI_ACTIVATE_EVENT
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
	GF_GUI_PROGRESS,
	GF_GUI_ENTRY,
	GF_GUI_CHECKBOX
};

#define gf_gui_call_t struct gf_gui_call

gf_gui_call_t;

#ifdef GF_EXPOSE_GUI
#define gf_gui_component_t struct gf_gui_component
#define gf_gui_t struct gf_gui

gf_gui_component_t;
gf_gui_t;

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
 * @struct gf_gui_component
 * @~english
 * @brief Component
 *
 * @var gf_gui_component::key
 * @brief Component ID
 *
 * @var gf_gui_component::type
 * @brief Component type
 * @see GF_GUI_COMPONENT_TYPES
 *
 * @var gf_gui_component::x
 * @brief X coord of component
 *
 * @var gf_gui_component::y
 * @brief X coord of component
 *
 * @var gf_gui_component::width
 * @brief Width of component
 *
 * @var gf_gui_component::height
 * @brief Height of component
 *
 * @var gf_gui_component::pressed
 * @brief `1` if pressed, otherwise `0`
 *
 * @var gf_gui_component::callback
 * @brief Event callback
 *
 * @var gf_gui_component::parent
 * @brief Parent GUI component
 *
 * @var gf_gui_component::prop
 * @brief Property
 *
 * @var gf_gui_component::text
 * @brief Text
 *
 * @var gf_gui_component::font
 * @brief Font color
 *
 * @var gf_gui_component::hover_font
 * @brief Hover font color
 *
 * @var gf_gui_component::texture
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
	gf_texture_t*	   texture;
});

/**
 * @struct gf_gui
 * @~english
 * @brief GUI
 *
 * @var gf_gui::engine
 * @brief Engine instance
 *
 * @var gf_gui::draw
 * @brief Drawing interface
 *
 * @var gf_gui::pressed
 * @brief `1` if something is being pressed, otherwise `0`
 *
 * @var gf_gui::area
 * @brief Created components
 *
 * @var gf_gui::base
 * @brief Base color
 *
 * @var gf_gui::font
 * @brief Font color
 *
 * @var gf_gui::hover
 * @brief Hover
 *
 * @var gf_gui::button_sound
 * @brief Button sound
 */
GF_DECLARE_TYPE(gui, {
	gf_engine_t*	     engine;
	gf_draw_t*	     draw;
	gf_gui_id_t	     pressed;
	gf_gui_component_t** area;
	gf_graphic_color_t   base;
	gf_graphic_color_t   font;
	gf_gui_id_t	     hover;
	char*		     button_sound;
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
