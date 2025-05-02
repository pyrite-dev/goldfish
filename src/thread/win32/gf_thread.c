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
	gf_thread_t* thread  = malloc(sizeof(*thread));
	thread->context.func = func;
	thread->context.data = userdata;
	if((thread->thread = CreateThread(NULL, 0, gf_wrap_thread, &thread->context, 0, NULL)) != NULL) return thread;
	free(thread);
	return NULL;
}

void gf_thread_join(gf_thread_t* thread) { WaitForSingleObject(thread->thread, INFINITE); }

void gf_thread_destroy(gf_thread_t* thread) {
	CloseHandle(thread->thread);
	free(thread);
}
