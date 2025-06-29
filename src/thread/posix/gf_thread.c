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
#include <pthread.h>

/* Interface */
#include <gf_thread.h>

/* Engine */

/* Standard */
#include <stdlib.h>

void* gf_wrap_thread(void* arg) {
	gf_thread_context_t* ctx = (gf_thread_context_t*)arg;
	ctx->func(ctx->data);
	return NULL;
}

gf_thread_t* gf_thread_create(void (*func)(void*), void* userdata) {
	gf_thread_t* thread  = malloc(sizeof(*thread));
	thread->context.func = func;
	thread->context.data = userdata;
	if(pthread_create(&thread->thread, NULL, gf_wrap_thread, &thread->context) == 0) return thread;
	free(thread);
	return NULL;
}

void gf_thread_join(gf_thread_t* thread) {
	void* value;
	pthread_join(thread->thread, &value);
}

void gf_thread_destroy(gf_thread_t* thread) { free(thread); }

gf_thread_mutex_t* gf_thread_mutex_create(void) {
	gf_thread_mutex_t* mutex = malloc(sizeof(*mutex));
	pthread_mutex_init(&mutex->mutex, NULL);

	return mutex;
}

void gf_thread_mutex_destroy(gf_thread_mutex_t* mutex) {
	pthread_mutex_destroy(&mutex->mutex);
	free(mutex);
}

void gf_thread_mutex_lock(gf_thread_mutex_t* mutex) { pthread_mutex_lock(&mutex->mutex); }

void gf_thread_mutex_unlock(gf_thread_mutex_t* mutex) { pthread_mutex_unlock(&mutex->mutex); }

gf_thread_event_t* gf_thread_event_create(void) {
	gf_thread_event_t* event = malloc(sizeof(*event));
	pthread_cond_init(&event->event, NULL);
	pthread_mutex_init(&event->mutex, NULL);

	return event;
}

void gf_thread_event_destroy(gf_thread_event_t* event) {
	pthread_mutex_destroy(&event->mutex);
	pthread_cond_destroy(&event->event);
	free(event);
}

void gf_thread_event_wait(gf_thread_event_t* event) {
	pthread_mutex_lock(&event->mutex);
	pthread_cond_wait(&event->event, &event->mutex);
	pthread_mutex_unlock(&event->mutex);
}

void gf_thread_event_signal(gf_thread_event_t* event) { pthread_cond_signal(&event->event); }
