/**
 * @file gf_opengl.h
 * @~english
 * @brief OpenGL headers
 * @note User should not include this, this header is used internally
 */

#ifndef __GF_OPENGL_H__
#define __GF_OPENGL_H__

#define GF_DO_SWAP_INTERVAL

#ifdef _WIN32
#include <windows.h>
#endif

#if defined(GF_USE_GLOAD) && defined(GF_TYPE_NATIVE)
#ifdef _WIN32
#undef APIENTRY
#endif
#include <gf_gload.h>
#else
#include <GL/gl.h>
#endif

#if defined(GF_USE_X11)
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#if defined(GF_TYPE_NATIVE)
#include <GL/glx.h>
#endif
#elif defined(GF_USE_GDI)
/* Should require nothing... for now? */
#elif defined(GF_USE_SDL2)
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#elif defined(GF_USE_GLFW)
#include <GLFW/glfw3.h>
#endif

#endif
