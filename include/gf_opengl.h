/**
 * @file gf_opengl.h
 * @~english
 * @brief OpenGL headers
 * @note User should not include this, this header is used internally
 */

#ifndef __GF_OPENGL_H__
#define __GF_OPENGL_H__

#if defined(USE_RGFW)
#if defined(TYPE_OSMESA)
#define RGFW_OSMESA
#endif

#define RGFW_USE_XDL
#define RGFWDEF extern

#include <RGFW.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#if defined(TYPE_OSMESA)
#include <GL/osmesa.h>
#endif

#if defined(USE_X11)
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#if defined(TYPE_NATIVE)
#include <GL/glx.h>
#endif
#elif defined(USE_GDI)
/* Should require nothing... for now? */
#endif
#endif

#endif
