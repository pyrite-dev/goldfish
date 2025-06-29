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
 * @file gf_type/thread.h
 * @~english
 * @brief Type definitions related to thread interface
 */

#ifndef __GF_TYPE_THREAD_H__
#define __GF_TYPE_THREAD_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_THREAD
#define gf_thread_context_t struct gf_thread_context
#define gf_thread_t struct gf_thread
#define gf_thread_mutex_t struct gf_thread_mutex
#define gf_thread_event_t struct gf_thread_event

gf_thread_context_t;
gf_thread_t;
gf_thread_mutex_t;
gf_thread_event_t;

/* External library */
#if defined(GF_THREAD_POSIX)
#include <pthread.h>
#elif defined(GF_THREAD_WIN32)
#include <windows.h>
#endif

/* Engine */

/* Standard */

/**
 * @struct gf_thread_context
 * @~english
 * @brief Thread context
 *
 * @var gf_thread_context::func
 * @brief Function to be called for thread
 *
 * @var gf_thread_context::data
 * @brief Data to be passed to thread
 */
GF_DECLARE_TYPE(thread_context, {
	void (*func)(void*);
	void* data;
});

#if defined(GF_THREAD_POSIX)
GF_DECLARE_TYPE(thread, {
	gf_thread_context_t context;
	pthread_t	    thread;
});

GF_DECLARE_TYPE(thread_mutex, { pthread_mutex_t mutex; });

GF_DECLARE_TYPE(thread_event, {
	pthread_cond_t	event;
	pthread_mutex_t mutex;
});
#elif defined(GF_THREAD_WIN32)
GF_DECLARE_TYPE(thread, {
	gf_thread_context_t context;
	HANDLE		    thread;
});

#ifdef WIN32_CRITICAL_SECTION
GF_DECLARE_TYPE(thread_mutex, { CRITICAL_SECTION mutex; });
#else
GF_DECLARE_TYPE(thread_mutex, { HANDLE mutex; });
#endif

GF_DECLARE_TYPE(thread_event, { HANDLE event; });
#else
/**
 * @struct gf_thread
 * @~english
 * @brief Platform-dependent thread
 * @note Definition depends on the platform, read the header file for definition
 */
GF_DECLARE_TYPE(thread, {});

/**
 * @struct gf_thread_mutex
 * @~english
 * @brief Platform-dependent mutex
 * @note Definition depends on the platform, read the header file for definition
 */
GF_DECLARE_TYPE(thread_mutex, {});

/**
 * @struct gf_thread_event
 * @~english
 * @brief Platform-dependent event
 * @note Definition depends on the platform, read the header file for definition
 */
GF_DECLARE_TYPE(thread_event, {});
#endif
#else
typedef void gf_thread_mutex_t;
typedef void gf_thread_event_t;
typedef void gf_thread_t;
typedef void gf_thread_context_t;
#endif

#endif
