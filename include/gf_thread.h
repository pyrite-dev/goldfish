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
 * @file gf_thread.h
 * @~english
 * @brief Thread interface
 */

#ifndef __GF_THREAD_H__
#define __GF_THREAD_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/thread.h>

/* Engine */

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Create thread
 * @param func Function to be called for thread
 * @param userdata Data to be passed to thread
 * @return Thread
 */
GF_EXPORT gf_thread_t* gf_thread_create(void (*func)(void*), void* userdata);

/**
 * @~english
 * @brief Join thread
 * @param thread Thread
 */
GF_EXPORT void gf_thread_join(gf_thread_t* thread);

/**
 * @~english
 * @brief Destroy thread
 * @param thread Thread
 */
GF_EXPORT void gf_thread_destroy(gf_thread_t* thread);

/**
 * @~english
 * @brief Create mutex
 * @return Mutex
 */
GF_EXPORT gf_thread_mutex_t* gf_thread_mutex_create(void);

/**
 * @~english
 * @brief Destroy mutex
 * @param mutex Mutex
 */
GF_EXPORT void gf_thread_mutex_destroy(gf_thread_mutex_t* mutex);

/**
 * @~english
 * @brief Lock mutex
 * @param mutex Mutex
 */
GF_EXPORT void gf_thread_mutex_lock(gf_thread_mutex_t* mutex);

/**
 * @~english
 * @brief Unlock mutex
 * @param mutex Mutex
 */
GF_EXPORT void gf_thread_mutex_unlock(gf_thread_mutex_t* mutex);

/**
 * @~english
 * @brief Create event
 * @return Event
 */
GF_EXPORT gf_thread_event_t* gf_thread_event_create(void);

/**
 * @~english
 * @brief Destroy event
 * @param event Event
 */
GF_EXPORT void gf_thread_event_destroy(gf_thread_event_t* event);

/**
 * @~english
 * @brief Wait for event
 * @param event Event
 */
GF_EXPORT void gf_thread_event_wait(gf_thread_event_t* event);

/**
 * @~english
 * @brief Signal event
 * @param event Event
 */
GF_EXPORT void gf_thread_event_signal(gf_thread_event_t* event);

#ifdef __cplusplus
}
#endif

#endif
