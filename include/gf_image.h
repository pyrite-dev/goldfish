/**
 * @file gf_image.h
 * @~english
 * @brief Image
 */

#ifndef __GF_IMAGE_H__
#define __GF_IMAGE_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */

/* Engine */
#include <gf_type/core.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Load image from memory
 * @param engine Engine instance
 * @param data Data
 * @param size Size
 * @param width Width
 * @param height Height
 * @return Data
 */
GF_EXPORT unsigned char* gf_image_load_memory(gf_engine_t* engine, unsigned char* data, size_t size, int* width, int* height);

/**
 * @~english
 * @brief Load image
 * @param engine Engine instance
 * @param path Path
 * @param width Width
 * @param height Height
 * @return Data
 */
unsigned char* gf_image_load(gf_engine_t* engine, const char* path, int* width, int* height);

#ifdef __cplusplus
}
#endif

#endif
