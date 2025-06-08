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

gf_draw_t;

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
 */
GF_DECLARE_TYPE(draw, {
	gf_engine_t*	    engine;
	gf_input_t*	    input;
	gf_draw_platform_t* platform;
	gf_draw_driver_t*   driver;
	gf_gui_t*	    gui;
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
	gf_draw_time_t	    last_draw;
	double		    fps;
	gf_font_t** loaded_fonts;
	gf_texture_t* skybox;
	gf_graphic_color_t background;
	int cursor;
	gf_texture_t* cursor_texture;
});
#else
typedef void gf_draw_t;
#endif

#endif
