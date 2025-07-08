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
 * @file gf_type/draw.h
 * @~english
 * @brief Type definitions related to drawing interface
 */

#ifndef __GF_TYPE_DRAW_H__
#define __GF_TYPE_DRAW_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_DRAW_USE_CLOCK
#include <time.h>

typedef clock_t gf_draw_time_t;
#else
#ifdef _WIN32
#include <windows.h>

typedef DWORD gf_draw_time_t;
#else
#include <sys/time.h>

typedef struct timeval gf_draw_time_t;
#endif
#endif

#ifdef GF_EXPOSE_DRAW
#define gf_draw_t struct gf_draw
#define gf_draw_intro_t struct gf_draw_intro

gf_draw_t;
gf_draw_intro_t;

/* External library */

/* Engine */
#include <gf_type/draw_platform.h>
#include <gf_type/draw_driver.h>
#include <gf_type/texture.h>
#include <gf_type/core.h>
#include <gf_type/math.h>
#include <gf_type/gui.h>
#include <gf_type/font.h>
#include <gf_type/input.h>
#include <gf_type/graphic.h>

/* Standard */
#include <time.h>

/**
 * @~english
 * @brief Drawing callback
 * @param draw Drawing interface
 */
typedef void(GFAPI* gf_draw_callback_t)(gf_draw_t* draw);

GF_DECLARE_TYPE(draw_intro, {
	int	      finished;
	int	      frame;
	gf_texture_t* powered;
});

/**
 * @struct gf_draw
 * @~english
 * @brief Drawing interface
 *
 * @var gf_draw::engine
 * @brief Engine instance
 *
 * @var gf_draw::input
 * @brief Input interface
 *
 * @var gf_draw::platform
 * @brief Platform-dependent part of drawing driver
 *
 * @var gf_draw::driver
 * @brief Drawing driver
 *
 * @var gf_draw::gui
 * @brief GUI
 *
 * @var gf_draw::console
 * @brief Console
 *
 * @var gf_draw::close
 * @brief `1` if it was requested to be closed, otherwise `0`
 *
 * @var gf_draw::x
 * @brief X coord of window
 *
 * @var gf_draw::y
 * @brief Y coord of window
 *
 * @var gf_draw::width
 * @brief Width of window
 *
 * @var gf_draw::height
 * @brief Height of window
 *
 * @var gf_draw::running
 * @brief `1` if running, otherwise `0`
 *
 * @var gf_draw::draw_3d
 * @brief `1` if engine should start rendering 3D stuffs, otherwise `0`
 *
 * @var gf_draw::title
 * @brief Window title
 *
 * @var gf_draw::light
 * @brief Light location
 * @note gf_draw::light[3] should be 1
 *
 * @var gf_draw::lookat
 * @brief Where to look at
 *
 * @var gf_draw::camera
 * @brief Camera location
 *
 * @var gf_draw::camera
 * @brief Camera location
 *
 * @var gf_draw::clip
 * @brief Clip region stack
 *
 * @var gf_draw::font
 * @brief Default font
 *
 * @var gf_draw::monospace_font
 * @brief Default monospace font
 *
 * @var gf_draw::last_draw
 * @brief Last time when frame was drawn
 *
 * @var gf_draw::fps
 * @brief FPS
 *
 * @var gf_draw::loaded_fonts
 * @brief Loaded fonts
 *
 * @var gf_draw::cursor
 * @brief Draw cursor or not
 *
 * @var gf_draw::cursor_texture
 * @brief Cursor texture
 *
 * @var gf_draw::skybox
 * @brief Skybox
 *
 * @var gf_draw::background
 * @brief Background color
 *
 * @var gf_draw::callback
 * @brief Callback
 *
 * @var gf_draw::rot
 * @brief Rotation
 */
GF_DECLARE_TYPE(draw, {
	gf_engine_t*	    engine;
	gf_input_t*	    input;
	gf_draw_platform_t* platform;
	gf_draw_driver_t*   driver;
	gf_gui_t*	    gui;
	gf_gui_id_t	    console;
	int		    close;
	int		    x;
	int		    y;
	int		    width;
	int		    height;
	int		    running;
	int		    draw_3d;
	char		    title[128];
	gf_math_vector_t    light;
	gf_math_vector_t    lookat;
	gf_math_vector_t    camera;
	double*		    clip;
	gf_font_t*	    font;
	gf_font_t*	    monospace_font;
	gf_draw_time_t	    last_draw;
	double		    fps;
	gf_font_t**	    loaded_fonts;
	gf_texture_t*	    skybox;
	gf_graphic_color_t  background;
	int		    cursor;
	gf_texture_t*	    cursor_texture;
	gf_draw_intro_t	    intro;
	gf_draw_callback_t  callback;
	gf_math_vector_t    rot;
});
#else
typedef void  gf_draw_t;
typedef void* gf_draw_callback_t;
#endif

#endif
