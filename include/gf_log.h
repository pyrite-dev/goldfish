/**
 * @file gf_log.h
 * @~english
 * @brief Logger
 */

#ifndef __GF_LOG_H__
#define __GF_LOG_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */

/* Engine */
#include <gf_type/core.h>

/* Standard */
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Default log output
 */
GF_EXPORT FILE* gf_log_default;

/**
 * @~english
 * @brief Output log with line number and function name
 * @param engine Engine instance
 * @param fmt Format string
 */
#define gf_log_function(engine, fmt, ...) gf_log(engine, "%24s: " fmt "\n", __FUNCTION_NAME__, __VA_ARGS__)

/**
 * @~english
 * @brief Output log
 * @param engine Engine instance
 * @param fmt Format string
 */
GF_EXPORT void gf_log(gf_engine_t* engine, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
