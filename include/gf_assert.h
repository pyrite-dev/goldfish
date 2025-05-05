/**
 * @file gf_assert.h
 * @~english
 * @brief Assert
 */

#ifndef __GF_ASSERT_H__
#define __GF_ASSERT_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */

/* Engine */
#include <gf_core.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Assert
 * @param engine Engine instance
 * @param expr Expression
 */
#ifdef GF_ASSERT_LUA
#define gf_assert(engine, expr) { int assert_tmp_result = ((expr) ? 0 : gf_assert_body(engine, #expr, __FILE__, __LINE__, __FUNCTION_NAME__)); if(assert_tmp_result){lua_pushstring(s, "Engine assert error");lua_error(s);return 1;} }
#else
#define gf_assert(engine, expr) ((expr) ? 0 : gf_assert_body(engine, #expr, __FILE__, __LINE__, __FUNCTION_NAME__))
#endif

/**
 * @~english
 * @brief Actual function for assert
 * @param engine Engine instance
 * @param expr Expression
 * @param filename Filename
 * @param line Line number
 * @param funcname Function name
 * @note Do not call this - you clearly want to use gf_assert
 * @return Always `1`
 */
GF_EXPORT int gf_assert_body(gf_engine_t* engine, const char* expr, const char* filename, int line, const char* funcname);

#ifdef __cplusplus
}
#endif

#endif
