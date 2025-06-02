#define GF_EXPOSE_CORE

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>

/* Interface */
#include <gf_util.h>

/* Engine */

/* Standard */
#include <string.h>
#include <stdlib.h>
#ifndef _WIN32
#include <pwd.h>
#include <unistd.h>
#endif

char* gf_util_strdup(const char* str) {
	char* s = malloc(strlen(str) + 1);
	strcpy(s, str);
	return s;
}

static void add_search(char*** l, char* p) {
	char* r = gf_util_strdup(p);

	arrput(*l, r);
}

static void add_user_search(char*** l, char* n) {
#ifdef _WIN32
	/* not good way */
	char* u = getenv("USERPROFILE");
	if(u != NULL) {
		char* p = malloc(strlen(u) + 1 + strlen(n) + 1);
		strcpy(p, u);
		strcat(p, "/");
		strcat(p, n);
		add_search(l, p);
		free(p);
	}
#else
	struct passwd* pwd = getpwuid(getuid());
	if(pwd != NULL) {
		char* p = malloc(strlen(pwd->pw_dir) + 1 + 7 + 1 + strlen(n) + 1);
		strcpy(p, pwd->pw_dir);
		strcat(p, "/.config/");
		strcat(p, n);
		add_search(l, p);
		free(p);
	}
#endif
}

char* gf_util_get_search(gf_engine_t* engine) {
	char** arr = NULL;
	char*  r;
	int    i;
	int    sz = 0;

	add_search(&arr, ".");
	add_search(&arr, "./dist");
	add_user_search(&arr, (engine == NULL || engine->name == NULL) ? "game_name_here" : engine->name);

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
