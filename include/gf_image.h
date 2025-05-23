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
