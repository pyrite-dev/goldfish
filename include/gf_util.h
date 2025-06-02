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

/* Standard */

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

#ifdef __cplusplus
}
#endif

#endif
