#define GF_EXPOSE_CORE

#include <gf_pre.h>

/* External library */

/* Interface */
#include <gf_log.h>

/* Engine */
#include <gf_core.h>

/* Standard */
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>

FILE* gf_log_default = NULL;

void gf_log(gf_engine_t* engine, const char* fmt, ...) {
	va_list va;
	FILE*	out = NULL;
	if(engine != NULL) {
		out = engine->log;
	}
	if(out != NULL && gf_log_default != out) {
		va_start(va, fmt);
		vfprintf(out, fmt, va);
		va_end(va);
		fflush(out);
	}
	if(gf_log_default != NULL) {
		va_start(va, fmt);
		vfprintf(gf_log_default, fmt, va);
		va_end(va);
		fflush(gf_log_default);
	}
}
