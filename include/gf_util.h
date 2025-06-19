/**
 * @file gf_util.h
 * @~english
 * @brief Utility
 */

#ifndef __GF_UTIL_H__
#define __GF_UTIL_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */

/* Engine */
#include <gf_type/core.h>
#include <gf_type/compat.h>

/* Standard */
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Duplicate string
 * @param str String
 * @return String
 */
GF_EXPORT char* gf_util_strdup(const char* str);

/**
 * @~english
 * @brief Get search path
 * @return Path
 */
GF_EXPORT char* gf_util_get_search(gf_engine_t* engine);

/**
 * @~english
 * @brief Get search path
 * @return Path
 */
GF_EXPORT char** gf_util_get_search_list(gf_engine_t* engine);

/**
 * @~english
 * @brief Get file size
 * @param f File pointer
 * @return Size
 */
GF_EXPORT int gf_util_file_size(FILE* f);

/**
 * @~english
 * @brief Generate random number
 * @param engine Engine instance
 * @return Random number
 */
GF_EXPORT gf_uint64_t gf_util_random(gf_engine_t* engine);

#ifdef __cplusplus
}
#endif

#endif
