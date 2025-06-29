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
 * @file gf_type/version.h
 * @~english
 * @brief Type definitions related to version
 */

#ifndef __GF_TYPE_VERSION_H__
#define __GF_TYPE_VERSION_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_VERSION
#define gf_version_t struct gf_version

gf_version_t;

/* External library */

/* Engine */

/* Standard */

/**
 * @struct gf_version
 * @~english
 * @brief Version
 *
 * @var gf_version::major
 * @brief Major version
 *
 * @var gf_version::minor
 * @brief Minor version
 *
 * @var gf_version::patch
 * @brief Patch version
 *
 * @var gf_version::date
 * @brief Build date
 *
 * @var gf_version::full
 * @brief Full version
 *
 * @var gf_version::backend
 * @brief Name of the rendering backend
 *
 * @var gf_version::driver
 * @brief Name of the rendering driver
 *
 * @var gf_version::lua
 * @brief Lua version
 *
 * @var gf_version::zlib
 * @brief zlib version
 *
 * @var gf_version::pcre
 * @brief PCRE version
 *
 * @var gf_version::thread
 * @brief Name of the thread model used
 */
GF_DECLARE_TYPE(version, {
	int  major;
	int  minor;
	int  patch;
	char date[64];
	char full[64];
	char backend[32];
	char driver[32];
	char lua[32];
	char zlib[32];
	char pcre[32];
	char thread[32];
});
#else
#error "should not happen!"
#endif

#endif
