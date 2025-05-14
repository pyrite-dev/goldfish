/**
 * @file gf_prop.h
 * @~english
 * @brief Property
 */

#ifndef __GF_PROP_H__
#define __GF_PROP_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/prop.h>

/* Engine */

/* Standard */

/**
 * @~english
 * @brief Value to be returned for "no such property"
 */
#define GF_PROP_NO_SUCH 0xffffff

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Set text to property
 * @param prop Property
 * @param key Key
 * @param value Value
 */
GF_EXPORT void gf_prop_set_text(gf_prop_t** prop, const char* key, const char* value);

/**
 * @~english
 * @brief Get text from property
 * @param prop Property
 * @param key Key
 * @return Value
 */
GF_EXPORT const char* gf_prop_get_text(gf_prop_t** prop, const char* key);

/**
 * @~english
 * @brief Set integer to property
 * @param prop Property
 * @param key Key
 * @param value Value
 */
GF_EXPORT void gf_prop_set_integer(gf_prop_t** prop, const char* key, gf_prop_integer_t value);

/**
 * @~english
 * @brief Get integer from property
 * @param prop Property
 * @param key Key
 * @return Value
 */
GF_EXPORT gf_prop_integer_t gf_prop_get_integer(gf_prop_t** prop, const char* key);

/**
 * @~english
 * @brief Set floating number to property
 * @param prop Property
 * @param key Key
 * @param value Value
 */
GF_EXPORT void gf_prop_set_floating(gf_prop_t** prop, const char* key, gf_prop_floating_t value);

/**
 * @~english
 * @brief Get floating number from property
 * @param prop Property
 * @param key Key
 * @return Value
 */
GF_EXPORT gf_prop_floating_t gf_prop_get_floating(gf_prop_t** prop, const char* key);

/**
 * @~english
 * @brief Set void pointer to property
 * @param prop Property
 * @param key Key
 * @param value Value
 */
GF_EXPORT void gf_prop_set_ptr(gf_prop_t** prop, const char* key, void* value);

/**
 * @~english
 * @brief Get void pointer from property
 * @param prop Property
 * @param key Key
 * @return Value
 */
GF_EXPORT void* gf_prop_get_ptr(gf_prop_t** prop, const char* key);

/**
 * @~english
 * @brief Set void pointer to property
 * @param prop Property
 * @param key Key
 * @param value Value
 */
GF_EXPORT void gf_prop_set_ptr_keep(gf_prop_t** prop, const char* key, void* value);

/**
 * @~english
 * @brief Get void pointer from property
 * @param prop Property
 * @param key Key
 * @return Value
 */
GF_EXPORT void* gf_prop_get_ptr_keep(gf_prop_t** prop, const char* key);

/**
 * @~english
 * @brief Delete property
 * @param prop Property
 * @param key Key
 */
GF_EXPORT void gf_prop_delete(gf_prop_t** prop, const char* key);

/**
 * @~english
 * @brief Destroy property container
 * @param prop Property container
 */
GF_EXPORT void gf_prop_destroy(gf_prop_t** prop);

/**
 * @~english
 * @brief Create property container
 * @param prop Property container
 */
GF_EXPORT void gf_prop_create(gf_prop_t** prop);

#ifdef __cplusplus
}
#endif

#endif
