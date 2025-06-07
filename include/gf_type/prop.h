/**
 * @file gf_type/prop.h
 * @~english
 * @brief Type definitions related to property
 */

#ifndef __GF_TYPE_PROP_H__
#define __GF_TYPE_PROP_H__

#include <gf_pre.h>
#include <gf_macro.h>

/**
 * @~english
 * @brief Integer type for property
 */
typedef long gf_prop_integer_t;

/**
 * @~english
 * @brief Floating number type for property
 */
typedef double gf_prop_floating_t;

#ifdef GF_EXPOSE_PROP
#define gf_prop_t struct gf_prop

gf_prop_t;

/* External library */

/* Engine */

/* Standard */

/**
 * @struct gf_prop
 * @~english
 * @brief Property container
 */
GF_DECLARE_TYPE(prop, {
	char* key;
	void* value;
	void* keep;
});
#else
typedef void gf_prop_t;
#endif

#endif
