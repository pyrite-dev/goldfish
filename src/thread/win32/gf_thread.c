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
 * File Name:    gf_thread.c
 * Author:       Nishi
 *
 * Functionality:
 * TODO: Write here
 *
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_THREAD

#include <gf_pre.h>

/* External library */
#include <windows.h>

/* Interface */
#include <gf_thread.h>

/* Engine */

/* Standard */
#include <stdlib.h>

DWORD WINAPI gf_wrap_thread(void* arg) {
	gf_thread_context_t* ctx = (gf_thread_context_t*)arg;
	ctx->func(ctx->data);
	return 0;
}

gf_thread_t* gf_thread_create(void (*func)(void*), void* userdata) {
	DWORD	     id;
	gf_thread_t* thread  = malloc(sizeof(*thread));
	thread->context.func = func;
	thread->context.data = userdata;
	if((thread->thread = CreateThread(NULL, 0, gf_wrap_thread, &thread->context, 0, &id)) != NULL) return thread;
	free(thread);
	return NULL;
}

void gf_thread_join(gf_thread_t* thread) { WaitForSingleObject(thread->thread, INFINITE); }

void gf_thread_destroy(gf_thread_t* thread) {
	CloseHandle(thread->thread);
	free(thread);
}

gf_thread_mutex_t* gf_thread_mutex_create(void) {
	gf_thread_mutex_t* mutex = malloc(sizeof(*mutex));
#ifdef WIN32_CRITICAL_SECTION
	InitializeCriticalSection(&mutex->mutex);
#else
	mutex->mutex = CreateEvent(NULL, FALSE, TRUE, NULL);
#endif

	return mutex;
}

void gf_thread_mutex_destroy(gf_thread_mutex_t* mutex) {
#ifdef WIN32_CRITICAL_SECTION
	DeleteCriticalSection(&mutex->mutex);
#else
	CloseHandle(mutex->mutex);
#endif
	free(mutex);
}

void gf_thread_mutex_lock(gf_thread_mutex_t* mutex) {
#ifdef WIN32_CRITICAL_SECTION
	EnterCriticalSection(&mutex->mutex);
#else
	WaitForSingleObject(mutex->mutex, INFINITE);
#endif
}

void gf_thread_mutex_unlock(gf_thread_mutex_t* mutex) {
#ifdef WIN32_CRITICAL_SECTION
	LeaveCriticalSection(&mutex->mutex);
#else
	SetEvent(mutex->mutex);
#endif
}

gf_thread_event_t* gf_thread_event_create(void) {
	gf_thread_event_t* event = malloc(sizeof(*event));
	event->event		 = CreateEvent(NULL, FALSE, FALSE, NULL);

	return event;
}

void gf_thread_event_destroy(gf_thread_event_t* event) {
	CloseHandle(event->event);
	free(event);
}

void gf_thread_event_wait(gf_thread_event_t* event) { WaitForSingleObject(event->event, INFINITE); }

void gf_thread_event_signal(gf_thread_event_t* event) { SetEvent(event->event); }
