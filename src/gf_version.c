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
 * File Name:    gf_version.c
 * Author:       Nishi
 *
 * Functionality:
 * Code for version stuff
 *
 **********************************************************************************
 * @endif
 */

#include <gf_pre.h>

/* External library */
#include <lua.h>
#include <zlib.h>
#include <pcre.h>

/* Interface */
#include <gf_version.h>

/* Engine */
#include <gf_util.h>

/* Standard */
#include <string.h>
#include <stdlib.h>

#define GF_VERSION "1.0.0"

static unsigned long date_to_num(void) {
	char*	      s	  = gf_util_strdup(__DATE__);
	int	      len = strlen(s);
	int	      c	  = 0;
	int	      i;
	int	      incr = 0;
	unsigned long n	   = 0;
	for(i = 0; i <= len; i++) {
		if(s[i] == ' ' || i == len) {
			char* str = s + incr;
			s[i]	  = 0;
			if(c == 0) {
				if(strcmp(str, "Jan") == 0) {
					n += 1 * 100;
				} else if(strcmp(str, "Feb") == 0) {
					n += 2 * 100;
				} else if(strcmp(str, "Mar") == 0) {
					n += 3 * 100;
				} else if(strcmp(str, "Apr") == 0) {
					n += 4 * 100;
				} else if(strcmp(str, "May") == 0) {
					n += 5 * 100;
				} else if(strcmp(str, "Jun") == 0) {
					n += 6 * 100;
				} else if(strcmp(str, "Jul") == 0) {
					n += 7 * 100;
				} else if(strcmp(str, "Aug") == 0) {
					n += 8 * 100;
				} else if(strcmp(str, "Sep") == 0) {
					n += 9 * 100;
				} else if(strcmp(str, "Oct") == 0) {
					n += 10 * 100;
				} else if(strcmp(str, "Nov") == 0) {
					n += 11 * 100;
				} else if(strcmp(str, "Dec") == 0) {
					n += 12 * 100;
				}
			} else if(c == 1) {
				n += atoi(str);
			} else if(c == 2) {
				n += atoi(str) * 100 * 100;
			}
			c++;
			incr = i + 1;
		}
	}
	free(s);

	return n;
}

void gf_version_get(gf_version_t* version) {
	char* cpstr = malloc(512);
	int   i;
	int   incr = 0;
	int   old  = 0;

	strcpy(version->date, __DATE__);

	strcpy(version->full, GF_VERSION);
#ifdef DEBUG
	strcat(version->full, "-debug");
#else
	strcat(version->full, "-release");
#endif
	sprintf(version->full + strlen(version->full), "-%lu", date_to_num());

	strcpy(version->zlib, ZLIB_VERSION);

	strcpy(cpstr, LUA_RELEASE);
	strcpy(version->lua, cpstr + 4);

	strcpy(version->pcre, pcre_version());

#if defined(GF_DRV_OPENGL)
	strcpy(version->driver, "OpenGL");
#if defined(GF_TYPE_NATIVE)
	strcat(version->driver, " (Native)");
#else
	strcat(version->driver, " (Unknown)");
#endif

#if defined(GF_USE_X11)
	strcpy(version->backend, "X11");
#elif defined(GF_USE_GDI)
	strcpy(version->backend, "GDI");
#elif defined(GF_USE_SDL2)
	strcpy(version->backend, "SDL2");
#elif defined(GF_USE_GLFW)
	strcpy(version->backend, "GLFW");
#endif
#endif

#if defined(GF_THREAD_WIN32)
	strcpy(version->thread, "Win32");
#elif defined(GF_THREAD_POSIX)
	strcpy(version->thread, "POSIX");
#endif

	strcpy(cpstr, GF_VERSION);
	for(i = 0;; i++) {
		if(cpstr[i] == '.' || cpstr[i] == 0) {
			int num;
			cpstr[i] = 0;
			num	 = atoi(cpstr + old);
			if(incr == 0) {
				version->major = num;
			} else if(incr == 1) {
				version->minor = num;
			} else if(incr == 2) {
				version->patch = num;
			}
			old = i + 1;
			incr++;
			if(incr == 3) break;
		}
	}
}
