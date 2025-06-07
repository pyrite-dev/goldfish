/**
 * @file gf_type/resource.h
 * @~english
 * @brief Type definitions related to resource
 */

#ifndef __GF_TYPE_RESOURCE_H__
#define __GF_TYPE_RESOURCE_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_RESOURCE
#define gf_resource_entry_t struct gf_resource_entry
#define gf_resource_t struct gf_resource

gf_resource_entry_t;
gf_resource_t;

/* External library */

/* Engine */
#include <gf_type/core.h>

/* Standard */
#include <stdio.h>

/**
 * @struct gf_resource_entry
 * @~english
 * @brief Resource entry
 *
 * @var gf_resource_entry::key
 * @brief Entry name
 *
 * @var gf_resource_entry::size
 * @brief Size
 *
 * @var gf_resource_entry::ogsize
 * @brief Size after decompressing
 *
 * @var gf_resource_entry::compressed
 * @brief Compressed data
 *
 * @var gf_resource_entry::cache
 * @brief Cache
 */
GF_DECLARE_TYPE(resource_entry, {
	char*  key;
	size_t size;
	size_t ogsize;
	char*  compressed;
	char*  cache;
});

/**
 * @struct gf_resource
 * @~english
 * @brief Resource
 *
 * @var gf_resource::engine
 * @brief Engine instance
 *
 * @var gf_resource::entries
 * @brief Resource entries
 *
 * @var gf_resource::path
 * @brief If non-`NULL`, the resource is "Type2"
 */
GF_DECLARE_TYPE(resource, {
	gf_engine_t*	     engine;
	gf_resource_entry_t* entries;
	char* path;
});
#else
typedef void gf_resource_entry_t;
typedef void gf_resource_t;
#endif

#endif
