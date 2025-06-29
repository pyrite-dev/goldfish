/**
 * @if SHOW_LICENSE
 **********************************************************************************
 * 
 * Copyright (c) 2025 Pyrite Development Team. All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification
 * are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer. 
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 * 
 * 3. All advertising materials mentioning features or use of this software must
 *    display the following acknowledgement: 
 *    This product includes software developed by the Pyrite Development Team. 
 * 
 * 4. Neither the name of Pyrite Development Team nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 **********************************************************************************
 * @endif
 */

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
