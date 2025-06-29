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
 * File Name:    gf_assert.c
 * Author:       Nishi
 *
 * Functionality:
 * Handle assert calls.
 *
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_CORE

#include <gf_pre.h>

/* External library */
#ifdef _WIN32
#include <windows.h>
#endif

/* Interface */
#include <gf_assert.h>

/* Engine */

/* Standard */
#include <stdlib.h>
#include <string.h>

int gf_assert_body(gf_engine_t* engine, const char* expr, const char* filename, int line, const char* funcname) {
	char* msg = malloc(4096);
	msg[0]	  = 0;

	sprintf(msg + strlen(msg), "GoldFish Assertion Error!\n");
	sprintf(msg + strlen(msg), "Expression: %s\n", expr);
	sprintf(msg + strlen(msg), "File: %s:%d\n", filename, line);
	sprintf(msg + strlen(msg), "Function Name: %s\n", funcname);
	sprintf(msg + strlen(msg), "\n");
	sprintf(msg + strlen(msg), "If you are the game developer, you fucked up something.\n");
	sprintf(msg + strlen(msg), "If you are a player, simply report this to the game developer.\n");
#ifdef _WIN32
	MessageBox(NULL, msg, "GoldFish Assertion Error", MB_OK | MB_ICONERROR);
#else
	fprintf(stderr, "----- Begin GoldFish Assertion Error -----\n");
	fprintf(stderr, "%s", msg);
	fprintf(stderr, "-----  End  GoldFish Assertion Error -----\n");
#endif
	free(msg);
	if(engine) engine->error = 1;

	return 1;
}
