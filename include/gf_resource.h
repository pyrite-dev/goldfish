/**
 * @file gf_resource.h
 * @~english
 * @brief Resource
 */

#ifndef __GF_RESOURCE_H__
#define __GF_RESOURCE_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/resource.h>

/* Engine */
#include <gf_type/core.h>

/* Standard */
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Create resource
 * @param engine Engine instance
 * @param path Path, use `NULL` to create empty resource
 * @return Resource
 */
GF_EXPORT gf_resource_t* gf_resource_create(gf_engine_t* engine, const char* path);

/**
 * @~english
 * @brief Destroy resource
 * @param resource Resource
 */
GF_EXPORT void gf_resource_destroy(gf_resource_t* resource);

/**
 * @~english
 * @brief Get data from resource
 * @param resource Resource
 * @param name Entry name
 * @param data Pointer to data result
 * @param size Pointer to size result
 * @return `0` if successful, otherwise `-1`
 */
GF_EXPORT int gf_resource_get(gf_resource_t* resource, const char* name, void** data, size_t* size);

/**
 * @~english
 * @brief Add data to resource
 * @param resource Resource
 * @param name Entry name
 * @param data Data
 * @param size Data size
 */
GF_EXPORT void gf_resource_add(gf_resource_t* resource, const char* name, void* data, size_t size);

/**
 * @~english
 * @brief Write resource to file
 * @param resource Resource
 * @param path Path
 * @param progress `0` to not show progress, otherwise show progress
 */
GF_EXPORT void gf_resource_write(gf_resource_t* resource, const char* path, int progress);

#ifdef __cplusplus
}
#endif

#endif
