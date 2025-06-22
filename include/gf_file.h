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
GF_EXPORT char* gf_file_pick(gf_engine_t* engine, const char* path);

/**
 * @~english
 * @brief Pick file path
 * @param path Path
 * @return Path
 */
GF_EXPORT char* gf_file_pick_reverse(gf_engine_t* engine, const char* path);

/**
 * @~english
 * @brief Pick file path
 * @param path Path
 * @return `0` if file/directory does not exist, `1` if it exists
 */
GF_EXPORT int gf_file_exists(gf_engine_t* engine, const char* path);

/**
 * @~english
 * @brief Open file
 * @param engine Engine instance
 * @param path Path
 * @param mode Mode
 * @return File
 */
GF_EXPORT gf_file_t* gf_file_open(gf_engine_t* engine, const char* path, const char* mode);

/**
 * @~english
 * @brief Read file
 * @param fp File
 * @param buffer Buffer
 * @param size Size
 * @return Read size
 */
GF_EXPORT size_t gf_file_read(gf_file_t* fp, void* buffer, size_t size);

/**
 * @~english
 * @brief Write to file
 * @param fp File
 * @param buffer Buffer
 * @param size Size
 * @return Written size
 */
GF_EXPORT size_t gf_file_write(gf_file_t* fp, void* buffer, size_t size);

/**
 * @~english
 * @brief Close file
 * @param fp File
 */
GF_EXPORT void gf_file_close(gf_file_t* fp);

/**
 * @~english
 * @brief Combine strings based on the given seperator (explicit length version)
 * @param length number of strings to join
 * @param ... Strings to join
 * @return Path
 */
GF_EXPORT char* gf_path_join(size_t length, ...);

#ifdef __cplusplus
}
#endif

#endif
