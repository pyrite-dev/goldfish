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
 * File Name:    gf_log.c
 * Author:       Nishi
 * 
 * Functionality:
 * TODO: Write here
 * 
 **********************************************************************************
 * @endif
 */

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
