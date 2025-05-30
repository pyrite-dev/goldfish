/**
 * @file gf_checksum.h
 * @~english
 * @brief Checksum
 */

#ifndef __GF_CHECKSUM_H__
#define __GF_CHECKSUM_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */

/* Engine */

/* Standard */
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Size of checksum in bytes
 */
#define GF_CHECKSUM_SIZE 4

/**
 * @~english
 * @brief Size of checksum chunk
 * @todo Is this a good size?
 */
#define GF_CHECKSUM_CHUNK 65536

/**
 * @~english
 * @brief Calculate checksum
 * @param data Data
 * @param size Size of data
 * @return Checksum
 */
GF_EXPORT void* gf_checksum_calculate(void* data, size_t size);

/**
 * @~english
 * @brief Check if checksum is valid or not
 * @param data Data
 * @param size Size of data
 * @param checksum Checksum
 * @return `1` if checksum is valid, `0` otherwise
 */
GF_EXPORT int gf_checksum_valid(void* data, size_t size, void* checksum);

#ifdef __cplusplus
}
#endif

#endif
