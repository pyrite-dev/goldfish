/**
 * @file gf_core.h
 * @~english
 * @brief Core
 */

#ifndef __GF_CORE_H__
#define __GF_CORE_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/core.h>

/* Engine */
#include <gf_type/draw.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Initialize engine
 */
GF_EXPORT void gf_engine_begin(void);

/**
 * @~english
 * @brief Cleanup engine
 */
GF_EXPORT void gf_engine_end(void);

/**
 * @~english
 * @brief Create engine instance, with default resource pack path (base.pak)
 * @param title Title to be shown on the window, can be used to specify game title too in the format: `Game title|Window title`
 * @param nogui `0` to enable client-part, otherwise disable client-part
 * @return Engine instance
 */
GF_EXPORT gf_engine_t* gf_engine_create(const char* title, int nogui);

/**
 * @~english
 * @brief Create engine instance
 * @param title Title to be shown on the window
 * @param nogui `0` to enable client-part, otherwise disable client-part
 * @param packpath Path to the base resource pack
 * @param argv Command line argument list
 * @param argc Command line argument count
 * @return Engine instance
 */
GF_EXPORT gf_engine_t* gf_engine_create_ex(const char* title, int nogui, const char* packpath, char** argv, int argc);

/**
 * @~english
 * @brief Start engine main loop
 * @param engine Engine instance
 * @note How engine draws a frame internally is explained in gf_core.c
 */
GF_EXPORT void gf_engine_loop(gf_engine_t* engine);

/**
 * @~english
 * @brief Destroy engine instance
 * @param engine Engine instance
 */
GF_EXPORT void gf_engine_destroy(gf_engine_t* engine);

/**
 * @~english
 * @brief Start engine shutdown
 * @param engine Engine instance
 */
GF_EXPORT void gf_engine_shutdown(gf_engine_t* engine);

/**
 * @~english
 * @brief Set game name
 * @param engine Engine instance
 * @param name Name
 */
GF_EXPORT void gf_engine_name(gf_engine_t* engine, const char* name);

/**
 * @~english
 * @brief Set game prefix
 * @param engine Engine instance
 * @param prefix Prefix
 */
GF_EXPORT void gf_engine_prefix(gf_engine_t* engine, const char* prefix);

#ifdef __cplusplus
}
#endif

#endif
