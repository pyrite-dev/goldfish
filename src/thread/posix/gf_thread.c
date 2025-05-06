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
