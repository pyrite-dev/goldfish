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
