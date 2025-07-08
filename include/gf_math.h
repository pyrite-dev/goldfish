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
GF_EXPORT double GFAPI gf_math_log2(double x);

/**
 * @~english
 * @brief Calculate abs(x)
 * @param x Input
 * @return abs(x)
 */
GF_EXPORT double GFAPI gf_math_abs(double x);

/**
 * @~english
 * @brief Calculate round(x)
 * @param x Input
 * @return round(x)
 */
GF_EXPORT double GFAPI gf_math_round(double x);

/**
 * @~english
 * @brief Calculate floor(x)
 * @param x Input
 * @return floor(x)
 */
GF_EXPORT double GFAPI gf_math_floor(double x);

/**
 * @~english
 * @brief Calculate ceil(x)
 * @param x Input
 * @return ceil(x)
 */
GF_EXPORT double GFAPI gf_math_ceil(double x);

/**
 * @~english
 * @brief Calculate normal of triangle v0,v1,v2
 * @param r Result
 * @param v0 Input
 * @param v1 Input
 * @param v2 Input
 */
GF_EXPORT void GFAPI gf_math_normal(gf_math_vector_t r, gf_math_vector_t v0, gf_math_vector_t v1, gf_math_vector_t v2);

/**
 * @~english
 * @brief Calculate cot(x)
 * @param x Input
 * @return cot(x)
 */
GF_EXPORT double GFAPI gf_math_cot(double x);

/**
 * @~english
 * @brief Get nearest 2^n
 * @param x Input
 * @return Nearest 2^n
 */
GF_EXPORT double GFAPI gf_math_nearest_2pow(double x);

/**
 * @~english
 * @brief Normalize vector
 * @param v Input/Output
 */
GF_EXPORT void GFAPI gf_math_normalize(gf_math_vector_t v);

/**
 * @~english
 * @brief Multiply vector by vector
 * @param r Result
 * @param v0 Input
 * @param v1 Input
 */
GF_EXPORT void GFAPI gf_math_multiply(gf_math_vector_t r, gf_math_vector_t v0, gf_math_vector_t v1);

/**
 * @~english
 * @brief Subtract vector by vector
 * @param r Result
 * @param v0 Input
 * @param v1 Input
 */
GF_EXPORT void GFAPI gf_math_subtract(gf_math_vector_t r, gf_math_vector_t v0, gf_math_vector_t v1);

#ifdef __cplusplus
}
#endif

#endif
