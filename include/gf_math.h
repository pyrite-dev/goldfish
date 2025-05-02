/**
 * @file gf_math.h
 * @~english
 * @brief Math
 */

#ifndef __GF_MATH_H__
#define __GF_MATH_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/math.h>

/* Engine */

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Pi
 */
#define GF_MATH_PI 3.14159265

/**
 * @~english
 * @brief Copy vector
 * @param from Source
 * @param to Destination
 */
#define GF_MATH_VECTOR_COPY(from, to) \
	to[0] = from[0]; \
	to[1] = from[1]; \
	to[2] = from[2]

/**
 * @~english
 * @brief Calculate log2(x)
 * @param x Input
 * @return log2(x)
 */
GF_EXPORT double gf_math_log2(double x);

/**
 * @~english
 * @brief Calculate normal of triangle v0,v1,v2
 * @param r Result
 * @param v0 Input
 * @param v1 Input
 * @param v2 Input
 */
GF_EXPORT void gf_math_normal(gf_math_vector_t r, gf_math_vector_t v0, gf_math_vector_t v1, gf_math_vector_t v2);

/**
 * @~english
 * @brief Calculate cot(x)
 * @param x Input
 * @return cot(x)
 */
GF_EXPORT double gf_math_cot(double x);

/**
 * @~english
 * @brief Get nearest 2^n
 * @param x Input
 * @return Nearest 2^n
 */
GF_EXPORT double gf_math_nearest_2pow(double x);

/**
 * @~english
 * @brief Normalize vector
 * @param v Input/Output
 */
GF_EXPORT void gf_math_normalize(gf_math_vector_t v);

/**
 * @~english
 * @brief Multiply vector by vector
 * @param r Result
 * @param v0 Input
 * @param v1 Input
 */
GF_EXPORT void gf_math_multiply(gf_math_vector_t r, gf_math_vector_t v0, gf_math_vector_t v1);

/**
 * @~english
 * @brief Subtract vector by vector
 * @param r Result
 * @param v0 Input
 * @param v1 Input
 */
GF_EXPORT void gf_math_subtract(gf_math_vector_t r, gf_math_vector_t v0, gf_math_vector_t v1);

#ifdef __cplusplus
}
#endif

#endif
