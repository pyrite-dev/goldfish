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
 * @file gf_type/resource.h
 * @~english
 * @brief Type definitions related to resource
 */

#ifndef __GF_TYPE_RESOURCE_H__
#define __GF_TYPE_RESOURCE_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_RESOURCE
#define gf_resource_entry_t struct gf_resource_entry
#define gf_resource_t struct gf_resource

gf_resource_entry_t;
gf_resource_t;

/* External library */

/* Engine */
#include <gf_type/core.h>

/* Standard */
#include <stdio.h>

/**
 * @struct gf_resource_entry
 * @~english
 * @brief Resource entry
 *
 * @var gf_resource_entry::key
 * @brief Entry name
 *
 * @var gf_resource_entry::size
 * @brief Size
 *
 * @var gf_resource_entry::ogsize
 * @brief Size after decompressing
 *
 * @var gf_resource_entry::compressed
 * @brief Compressed data
 *
 * @var gf_resource_entry::cache
 * @brief Cache
 */
GF_DECLARE_TYPE(resource_entry, {
	char*  key;
	size_t size;
	size_t ogsize;
	char*  compressed;
	char*  cache;
});

/**
 * @struct gf_resource
 * @~english
 * @brief Resource
 *
 * @var gf_resource::engine
 * @brief Engine instance
 *
 * @var gf_resource::entries
 * @brief Resource entries
 *
 * @var gf_resource::path
 * @brief If non-`NULL`, the resource is "Type2"
 */
GF_DECLARE_TYPE(resource, {
	gf_engine_t*	     engine;
	gf_resource_entry_t* entries;
	char*		     path;
});
#else
typedef void gf_resource_entry_t;
typedef void gf_resource_t;
#endif

#endif
