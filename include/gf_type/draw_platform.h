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
GF_DECLARE_TYPE(draw_platform, {
	gf_engine_t* engine;
	Display*     display;
	Window	     window;
	GLXContext   context;
	Atom	     wm_delete_window;
	Pixmap cursor;
});
#elif defined(GF_TYPE_OSMESA)
GF_DECLARE_TYPE(draw_platform, {
	gf_engine_t*   engine;
	Display*       display;
	Window	       window;
	OSMesaContext  context;
	Atom	       wm_delete_window;
	unsigned char* buffer;
	XImage*	       image;
	XVisualInfo    visual;
	GC	       gc;
	Pixmap cursor;
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
	HCURSOR cursor;
});
#elif defined(GF_TYPE_OSMESA)
GF_DECLARE_TYPE(draw_platform, {
	gf_engine_t*   engine;
	HINSTANCE      instance;
	HWND	       window;
	HDC	       dc;
	OSMesaContext  context;
	unsigned char* buffer;
	HBITMAP	       bitmap;
	BITMAPV4HEADER header;
	HDC	       bitmapdc;
	HCURSOR cursor;
});
#endif
#elif defined(GF_USE_RGFW)
GF_DECLARE_TYPE(draw_platform, {
	gf_engine_t* engine;
	RGFW_window* window;
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
