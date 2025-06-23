/**
 * @file gf_opengl.h
 * @~english
 * @brief OpenGL headers
 * @note User should not include this, this header is used internally
 */

#ifndef __GF_OPENGL_H__
#define __GF_OPENGL_H__

#if defined(GF_USE_RGFW)
#if defined(GF_TYPE_OSMESA)
#define RGFW_OSMESA
#endif

#define RGFW_USE_XDL
#define RGFWDEF extern
#ifdef GF_USE_GLOAD
#define RGFW_NO_GL_HEADER
#include <gf_gload.h>
#endif

#include <RGFW.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif

#if defined(GF_USE_AGL)
#include <agl.h>
#include <gl.h>
// agl was designed by hitler and forces us to have this global with this exact name and it has to be defined before we include this :)
extern AGLContext agl_ctx;
#include <aglMacro.h>
#elif defined(GF_USE_GLOAD) && defined(GF_TYPE_NATIVE)
#ifdef _WIN32
#undef APIENTRY
#endif
#include <gf_gload.h>
#else
#include <GL/gl.h>
#endif

#if defined(GF_TYPE_OSMESA)
#include <GL/osmesa.h>
#endif

#if defined(GF_USE_X11)
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#if defined(GF_TYPE_NATIVE)
#include <GL/glx.h>
#endif
#elif defined(GF_USE_GDI)
/* Should require nothing... for now? */
#endif


#endif

#endif
