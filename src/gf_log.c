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
#include <string.h>
#include <stdlib.h>

FILE* gf_log_default = NULL;

void gf_log(gf_engine_t* engine, const char* fmt, ...) {
	va_list va;
	FILE*	out = NULL;
	int	l   = -1;
	char*	buffer;
	if(engine != NULL) {
		out = engine->log;
	}
	if(out != NULL && gf_log_default != out) {
		va_start(va, fmt);
		l = vfprintf(out, fmt, va);
		va_end(va);
		fflush(out);
	}
	if(gf_log_default != NULL) {
		va_start(va, fmt);
		l = vfprintf(gf_log_default, fmt, va);
		va_end(va);
		fflush(gf_log_default);
	}

	if(l != -1 && engine != NULL) {
		char* old = engine->log_list;

		buffer = malloc(l + 1);
		memset(buffer, 0, l + 1);

		va_start(va, fmt);
		vsprintf(buffer, fmt, va);
		va_end(va);

		engine->log_list = malloc(strlen(old) + strlen(buffer) + 1);
		strcpy(engine->log_list, old);
		strcpy(engine->log_list + strlen(old), buffer);
		free(old);

		free(buffer);
	}
}
