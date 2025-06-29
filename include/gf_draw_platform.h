/**
 * @file gf_draw_platform.h
 * @~english
 * @brief Platform-dependent part of drawing driver
 * @warning You should not use functions written here, since engine does it for you
 */

#ifndef __GF_DRAW_PLATFORM_H__
#define __GF_DRAW_PLATFORM_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/draw_platform.h>

/* Engine */
#include <gf_type/draw.h>
#include <gf_type/core.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Create platform-dependent part of drawing driver
 * @param engine Engine instance
 * @param draw Drawing interface
 * @return Platform-dependent part of drawing driver
 */
GF_EXPORT gf_draw_platform_t* gf_draw_platform_create(gf_engine_t* engine, gf_draw_t* draw);

/**
 * @~english
 * @brief Destroy platform-dependent part of drawing driver
 * @param platform Platform-dependent part of drawing driver
 */
GF_EXPORT void gf_draw_platform_destroy(gf_draw_platform_t* platform);

/**
 * @~english
 * @brief Do platform-dependent step
 * @param draw Drawing interface
 * @return `0` if successful, otherwise if failed
 */
GF_EXPORT int gf_draw_platform_step(gf_draw_t* draw);

/**
 * @~english
 * @brief Check if renderer has extension
 * @param draw Drawing interface
 * @param query Extension to be checked
 * @return `1` if extension is present, otherwise if it isn't
 */
GF_EXPORT int gf_draw_platform_has_extension(gf_draw_t* draw, const char* query);

/**
 * @~english
 * @brief Initialize platform-dependent part of drawing driver
 */
GF_EXPORT void gf_draw_platform_begin(void);

/**
 * @~english
 * @brief Cleanup platform-dependent part of drawing driver
 */
GF_EXPORT void gf_draw_platform_end(void);

/**
 * @~english
 * @brief Get window handle for platform-specific operations
 * @param draw Drawing interface
 * @return Platform-specific window handle
 */
GF_EXPORT void* gf_draw_platform_get_window_handle(gf_draw_t* draw);

#ifdef __cplusplus
}
#endif

#endif
