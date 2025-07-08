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
#define gf_assert(engine, expr) \
	{ \
		int assert_tmp_result = ((expr) ? 0 : gf_assert_body(engine, #expr, __FILE__, __LINE__, __FUNCTION_NAME__)); \
		if(assert_tmp_result) { \
			lua_pushstring(s, "Engine assert error"); \
			lua_error(s); \
			return 1; \
		} \
	}
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
GF_EXPORT int GFAPI gf_assert_body(gf_engine_t* engine, const char* expr, const char* filename, int line, const char* funcname);

#ifdef __cplusplus
}
#endif

#endif
