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
