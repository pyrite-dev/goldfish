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
 * @file gf_macro.h
 * @~english
 * @brief Macro
 */

#ifndef __GF_MACRO_H__
#define __GF_MACRO_H__

#include <gf_pre.h>

#ifdef GF_EXPOSE_ALL

#ifndef GF_EXPOSE_CORE
/**
 * @~english
 * @brief Expose core properties
 */
#define GF_EXPOSE_CORE
#endif

#ifndef GF_EXPOSE_DRAW
/**
 * @~english
 * @brief Expose drawing interface properties
 */
#define GF_EXPOSE_DRAW
#endif

#ifndef GF_EXPOSE_DRAW_PLATFORM
/**
 * @~english
 * @brief Expose platform-dependent part of drawing driver properties
 */
#define GF_EXPOSE_DRAW_PLATFORM
#endif

#ifndef GF_EXPOSE_DRAW_DRIVER
/**
 * @~english
 * @brief Expose drawing driver properties
 */
#define GF_EXPOSE_DRAW_DRIVER
#endif

#ifndef GF_EXPOSE_MESH
/**
 * @~english
 * @brief Expose mesh properties
 */
#define GF_EXPOSE_MESH
#endif

#ifndef GF_EXPOSE_MODEL
/**
 * @~english
 * @brief Expose model properties
 */
#define GF_EXPOSE_MODEL
#endif

#ifndef GF_EXPOSE_TEXTURE
/**
 * @~english
 * @brief Expose texture properties
 */
#define GF_EXPOSE_TEXTURE
#endif

#ifndef GF_EXPOSE_PHYSICS
/**
 * @~english
 * @brief Expose physics interface properties
 */
#define GF_EXPOSE_PHYSICS
#endif

#ifndef GF_EXPOSE_GUI
/**
 * @~english
 * @brief Expose GUI properties
 */
#define GF_EXPOSE_GUI
#endif

#ifndef GF_EXPOSE_THREAD
/**
 * @~english
 * @brief Expose thread interface properties
 */
#define GF_EXPOSE_THREAD
#endif

#ifndef GF_EXPOSE_CLIENT
/**
 * @~english
 * @brief Expose client interface properties
 */
#define GF_EXPOSE_CLIENT
#endif

#ifndef GF_EXPOSE_SERVER
/**
 * @~english
 * @brief Expose server interface properties
 */
#define GF_EXPOSE_SERVER
#endif

#ifndef GF_EXPOSE_FONT
/**
 * @~english
 * @brief Expose font properties
 */
#define GF_EXPOSE_FONT
#endif

#ifndef GF_EXPOSE_INPUT
/**
 * @~english
 * @brief Expose input interface properties
 */
#define GF_EXPOSE_INPUT
#endif

#ifndef GF_EXPOSE_AUDIO
/**
 * @~english
 * @brief Expose audio interface properties
 */
#define GF_EXPOSE_AUDIO
#endif

#ifndef GF_EXPOSE_RESOURCE
/**
 * @~english
 * @brief Expose resource properties
 */
#define GF_EXPOSE_RESOURCE
#endif

#ifndef GF_EXPOSE_PROP
/**
 * @~english
 * @brief Expose property container properties
 */
#define GF_EXPOSE_PROP
#endif

#ifndef GF_EXPOSE_LUA
/**
 * @~english
 * @brief Expose Lua properties
 */
#define GF_EXPOSE_LUA
#endif

#ifndef GF_EXPOSE_SOUND
/**
 * @~english
 * @brief Expose sound driver properties
 */
#define GF_EXPOSE_SOUND
#endif

#ifndef GF_EXPOSE_NETWORK
/**
 * @~english
 * @brief Expose network interface properties
 */
#define GF_EXPOSE_NETWORK
#endif

#ifndef GF_EXPOSE_COMMAND
/**
 * @~english
 * @brief Expose command properties
 */
#define GF_EXPOSE_COMMAND
#endif

#ifndef GF_EXPOSE_FILE
/**
 * @~english
 * @brief Expose file I/O properties
 * @note Exposed by default
 */
#define GF_EXPOSE_FILE
#endif

#ifndef GF_EXPOSE_GRAPHIC
/**
 * @~english
 * @brief Expose graphic interface properties
 * @note Exposed by default
 */
#define GF_EXPOSE_GRAPHIC
#endif

#ifndef GF_EXPOSE_MATH
/**
 * @~english
 * @brief Expose math properties
 * @note Exposed by default
 */
#define GF_EXPOSE_MATH
#endif

#ifndef GF_EXPOSE_VERSION
/**
 * @~english
 * @brief Expose version properties
 * @note Exposed by default
 */
#define GF_EXPOSE_VERSION
#endif

#endif

/**
 * @def __FUNCTION_NAME__
 * @~english
 * @brief Macro for compiler-dependent function name macro
 */
#ifndef __FUNCTION_NAME__
#ifdef _MSC_VER
#define __FUNCTION_NAME__ __FUNCTION__
#else
#define __FUNCTION_NAME__ __func__
#endif
#endif

/**
 * @~english
 * @brief Macro to define engine type shorter
 * @param n Name
 * @param b Body
 */
#define GF_DECLARE_TYPE(n, b) struct gf_##n b;

/**
 * @~english
 * @brief Macro to define engine union shorter
 * @param n Name
 * @param b Body
 */
#define GF_DECLARE_UNION(n, b) union gf_##n b;

/**
 * @def GF_EXPORT
 * @~english
 * @brief Macro for platform-dependent symbol export/import
 */
#if defined(_WIN32) && defined(GF_DLL)
#define GF_EXPORT extern __declspec(dllexport)
#elif defined(_WIN32) && defined(GF_BUILT_AS_DLL)
#define GF_EXPORT extern __declspec(dllimport)
#else
#define GF_EXPORT extern
#endif

/**
 * @def GFAPI
 * @~english
 * @brief Macro for calling convention
 */
#if defined(_WIN32)
#define GFAPI __cdecl
#else
#define GFAPI
#endif

/* Expose them by default */
#ifndef GF_EXPOSE_GRAPHIC
#define GF_EXPOSE_GRAPHIC
#endif

#ifndef GF_EXPOSE_MATH
#define GF_EXPOSE_MATH
#endif

#ifndef GF_EXPOSE_VERSION
#define GF_EXPOSE_VERSION
#endif

#ifndef GF_EXPOSE_FILE
#define GF_EXPOSE_FILE
#endif

#endif
