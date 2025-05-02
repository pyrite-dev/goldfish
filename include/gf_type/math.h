/**
 * @file gf_type/math.h
 * @~english
 * @brief Type definitions related to math
 */

#ifndef __GF_TYPE_MATH_H__
#define __GF_TYPE_MATH_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_MATH
/* External library */

/* Engine */

/* Standard */

/**
 * @~english
 * @brief Vector
 * @note gf_math_vector_t[3] is used internally
 */
typedef double gf_math_vector_t[4];
#else
#error "should not happen!"
#endif

#endif
