/**
 * @file gf_unicode.h
 * @~english
 * @brief Unicode
 */

#ifndef __GF_UNICODE_H__
#define __GF_UNICODE_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */

/* Engine */
#include <gf_type/compat.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Converts UTF-8 character to UTF-32 character
 * @param input Input
 * @param output Output
 * @return Bytes of input that had to be consumed, `0` on failure
 */
GF_EXPORT int gf_unicode_8_to_32(const char* input, gf_int32_t* output);

#ifdef __cplusplus
}
#endif

#endif
