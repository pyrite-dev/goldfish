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
