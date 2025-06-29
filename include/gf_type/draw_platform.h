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
 * @file gf_type/draw_platform.h
 * @~english
 * @brief Type definitions related to platform-dependent part of drawing driver
 */

#ifndef __GF_TYPE_DRAW_PLATFORM_H__
#define __GF_TYPE_DRAW_PLATFORM_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_DRAW_PLATFORM
#define gf_draw_platform_t struct gf_draw_platform

gf_draw_platform_t;

/* External library */
#if defined(GF_DRV_OPENGL)
#include <gf_opengl.h>
#endif

/* Engine */
#include <gf_type/core.h>

/* Standard */

#if defined(GF_DRV_OPENGL)
#if defined(GF_USE_X11)
#if defined(GF_TYPE_NATIVE)
#define gf_draw_platform_keymap_t struct gf_draw_platform_keymap

gf_draw_platform_keymap_t;

GF_DECLARE_TYPE(draw_platform_keymap, {
	int key;
	int value;
});

GF_DECLARE_TYPE(draw_platform, {
	gf_engine_t*		   engine;
	Display*		   display;
	Window			   window;
	GLXContext		   context;
	Atom			   wm_delete_window;
	Pixmap			   cursor;
	gf_draw_platform_keymap_t* keymap;
});
#endif
#elif defined(GF_USE_GDI)
#if defined(GF_TYPE_NATIVE)
GF_DECLARE_TYPE(draw_platform, {
	gf_engine_t* engine;
	HINSTANCE    instance;
	HWND	     window;
	HDC	     dc;
	HGLRC	     glrc;
	HCURSOR	     cursor;
});
#endif
#elif defined(GF_USE_SDL2)
GF_DECLARE_TYPE(draw_platform, {
	gf_engine_t*  engine;
	SDL_Window*   window;
	SDL_GLContext context;
});
#elif defined(GF_USE_GLFW)
GF_DECLARE_TYPE(draw_platform, {
	gf_engine_t* engine;
	GLFWwindow*  window;
});
#endif
#else
/**
 * @struct gf_draw_platform
 * @~english
 * @brief Platform-dependent part of drawing driver
 * @note Definition depends on the drawing driver and the platform, read the header file for definition
 */
GF_DECLARE_TYPE(draw_platform, {});
#endif
#else
typedef void gf_draw_platform_t;
#endif

#endif
