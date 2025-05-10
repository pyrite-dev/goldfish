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
#define GF_DECLARE_TYPE(n, b) typedef struct gf_##n##_t b gf_##n##_t;

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
