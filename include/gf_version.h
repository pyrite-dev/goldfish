/**
 * @file gf_version.h
 * @~english
 * @brief Version
 */

#ifndef __GF_VERSION_H__
#define __GF_VERSION_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/version.h>

/* Engine */

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Get version of engine
 */
GF_EXPORT void gf_version_get(gf_version_t* version);

#ifdef __cplusplus
}
#endif

#endif
