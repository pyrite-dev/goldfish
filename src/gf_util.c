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
 * File Name:    gf_util.c
 * Author:       Nishi
 *
 * Functionality:
 * Utility code.
 *
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_CORE

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>
#ifdef _WIN32
#include <shlobj.h>
#endif

/* Interface */
#include <gf_util.h>

/* Engine */
#include <gf_prop.h>
#include <gf_file.h>
#include <gf_log.h>

/* Standard */
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <io.h>
#else
#include <pwd.h>
#include <unistd.h>
#endif

char* gf_util_strdup(const char* str) {
	char* s = malloc(strlen(str) + 1);
	strcpy(s, str);
	return s;
}

static void add_search(gf_engine_t* engine, char*** l, const char* p, int need_game_name) {
	char* nam = (engine == NULL || engine->name == NULL) ? "game_name_here" : engine->name;
	char* r;
	if(need_game_name) {
		r = gf_file_path_join(2, p, nam);
	} else {
		r = gf_util_strdup(p);
	}

	arrput(*l, r);
}

static void add_user_search(gf_engine_t* engine, char*** l) {
#ifdef _WIN32
	/* not good way */
	char	     shp[MAX_PATH];
	LPITEMIDLIST pidl;
	char*	     u = getenv("USERPROFILE");
	if(SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pidl) == S_OK) {
		SHGetPathFromIDList(pidl, shp);
		CoTaskMemFree(pidl);
		add_search(engine, l, shp, 1);
	}
	if(u != NULL) {
		add_search(engine, l, u, 1);
	}
#if WINVER >= 0x0500
	if(SHGetSpecialFolderLocation(NULL, CSIDL_PROFILE, &pidl) == S_OK) {
		SHGetPathFromIDList(pidl, shp);
		CoTaskMemFree(pidl);
		add_search(engine, l, shp, 1);
	}
#endif
#else
	struct passwd* pwd = getpwuid(getuid());
	if(pwd != NULL) {
		char* p = gf_file_path_join(3, pwd->pw_dir, ".local", "share");
		add_search(engine, l, p, 1);
		free(p);
	}
#endif
}

char* gf_util_get_search(gf_engine_t* engine) {
	char**	    arr = NULL;
	char*	    r;
	int	    i;
	int	    sz = 0;
	char*	    n;
	const char* pref;

	if(engine != NULL && (pref = gf_prop_get_text(&engine->config, "prefix")) != NULL) {
		/* windows users probably prefer this, i do not know */
#ifdef _WIN32
		add_search(engine, &arr, pref, 0);
#else
		add_search(engine, &arr, pref, 1);
#endif
	}

	add_user_search(engine, &arr);

	for(i = 0; i < arrlen(arr); i++) {
		if(i > 0) sz += 1;
		sz += strlen(arr[i]);
	}

	r    = malloc(sz + 1);
	r[0] = 0;

	for(i = 0; i < arrlen(arr); i++) {
		if(i > 0) strcat(r, ";");
		strcat(r, arr[i]);

		free(arr[i]);
	}

	arrfree(arr);

	return r;
}

char** gf_util_get_search_list(gf_engine_t* engine) {
	char** l = NULL;
	char*  r = gf_util_get_search(engine);
	int    i;
	int    incr = 0;
	for(i = 0;; i++) {
		if(r[i] == ';' || r[i] == 0) {
			char  oldc = r[i];
			char* s;
			r[i] = 0;

			s = gf_util_strdup(r + incr);

			arrput(l, s);

			incr = i + 1;
			if(oldc == 0) break;
		}
	}
	free(r);

	return l;
}

char* gf_util_get_user_path(gf_engine_t* engine) {
	char** list = gf_util_get_search_list(engine);
	int    i;
	int    picked = -1;
	char*  r      = NULL;
	for(i = arrlen(list) - 1; i >= 0; i--) {
		char*  pth = gf_util_strdup(list[i]);
		char** l   = gf_file_separate_path(pth, 0);
		int    j;
		char*  p;
		FILE*  f;

		for(j = 0; j < arrlen(l); j++) {
			gf_file_create_directory(engine, l[j]);
			free(l[j]);
		}
		arrfree(l);

		p = gf_file_path_join(2, pth, "gftest");
		f = fopen(p, "w");
		if(f != NULL) {
			gf_log_function(engine, "%s seems usable for user path", pth);
			picked = i;
			r      = pth;
			fclose(f);
			remove(p);
			free(p);
			break;
		} else {
			free(p);
		}
	}

	/* cleanup */
	for(i = 0; i < arrlen(list); i++) {
		free(list[i]);
	}
	arrfree(list);

	return r;
}

int gf_util_file_size(FILE* f) {
#if 1
	int r;
	fseek(f, 0, SEEK_END);
	r = ftell(f);
	fseek(f, 0, SEEK_SET);
	return r;
#else
#ifdef _WIN32
	int	     fd = _fileno(f);
	struct _stat s;
	_fstat(fd, &s);
	return s.st_size;
#else
	int	    fd = fileno(f);
	struct stat s;
	fstat(fd, &s);
	return s.st_size;
#endif
#endif
}

gf_uint64_t gf_util_random(gf_engine_t* engine) {
	gf_uint64_t n = engine->seed;
	n ^= n << 7;
	n ^= n >> 9;
	return engine->seed = n;
}
