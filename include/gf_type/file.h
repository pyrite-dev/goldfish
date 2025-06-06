/**
 * @file gf_type/file.h
 * @~english
 * @brief Type definitions related to file I/O
 */

#ifndef __GF_TYPE_FILE_H__
#define __GF_TYPE_FILE_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_FILE
#define gf_file_t struct gf_file

gf_file_t;

/* External library */

/* Engine */

/* Standard */
#include <stddef.h>
#include <stdio.h>

/**
 * @struct gf_file
 * @~english
 * @brief File
 *
 * @var gf_file::fp
 * @brief File pointer
 *
 * @var gf_file::buffer
 * @brief Buffer
 *
 * @var gf_file::pos
 * @brief File position
 *
 * @var gf_file::size
 * @brief File size
 */
GF_DECLARE_TYPE(file, {
	FILE*  fp;
	void*  buffer;
	size_t pos;
	size_t size;
});
#else
#error "should not happen!"
#endif

#endif
