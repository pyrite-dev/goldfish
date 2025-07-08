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
 * @file gf_file.h
 * @~english
 * @brief File I/O
 */

#ifndef __GF_FILE_H__
#define __GF_FILE_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/file.h>

/* Engine */
#include <gf_type/core.h>
#include <gf_type/resource.h>

/* Standard */
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Pick file path
 * @param path Path
 * @return Path
 */
GF_EXPORT char* GFAPI gf_file_pick(gf_engine_t* engine, const char* path);

/**
 * @~english
 * @brief Pick file path
 * @param path Path
 * @return Path
 */
GF_EXPORT char* GFAPI gf_file_pick_reverse(gf_engine_t* engine, const char* path);

/**
 * @~english
 * @brief Pick file path
 * @param path Path
 * @return `0` if file/directory does not exist, `1` if it exists
 */
GF_EXPORT int GFAPI gf_file_exists(gf_engine_t* engine, const char* path);

/**
 * @~english
 * @brief Open file
 * @param engine Engine instance
 * @param path Path
 * @param mode Mode
 * @return File
 */
GF_EXPORT gf_file_t* GFAPI gf_file_open(gf_engine_t* engine, const char* path, const char* mode);

/**
 * @~english
 * @brief Read file
 * @param fp File
 * @param buffer Buffer
 * @param size Size
 * @return Read size
 */
GF_EXPORT size_t GFAPI gf_file_read(gf_file_t* fp, void* buffer, size_t size);

/**
 * @~english
 * @brief Write to file
 * @param fp File
 * @param buffer Buffer
 * @param size Size
 * @return Written size
 */
GF_EXPORT size_t GFAPI gf_file_write(gf_file_t* fp, void* buffer, size_t size);

/**
 * @~english
 * @brief Close file
 * @param fp File
 */
GF_EXPORT void GFAPI gf_file_close(gf_file_t* fp);

/**
 * @~english
 * @brief Combine strings based on the given seperator (explicit length version)
 * @param length number of strings to join
 * @param ... Strings to join
 * @return Path
 */
GF_EXPORT char* GFAPI gf_file_path_join(size_t length, ...);

/**
 * @~english
 * @brief Separate path
 * @param path Path
 * @param only `0` if you want concatenated path, otherwise only path name
 * @return Array
 */
GF_EXPORT char** GFAPI gf_file_separate_path(const char* path, int only);

/**
 * @~english
 * @brief Register resource
 * @param engine Engine instance
 * @param name Resource name
 * @param resource Resource
 */
GF_EXPORT void GFAPI gf_file_register(gf_engine_t* engine, const char* name, gf_resource_t* resource);

/**
 * @~english
 * @brief Create directory
 * @param engine Engine instance
 * @param path Path
 */
GF_EXPORT void GFAPI gf_file_create_directory(gf_engine_t* engine, const char* path);

/**
 * @~english
 * @brief Get resource from the path
 * @param engine Engine instance
 * @param path Path
 * @param name Name
 * @return Resource
 */
GF_EXPORT gf_resource_t* GFAPI gf_file_get_resource_from_path(gf_engine_t* engine, const char* path, char** name);

#ifdef __cplusplus
}
#endif

#endif
