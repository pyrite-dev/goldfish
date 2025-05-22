#define GF_EXPOSE_CORE

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>

/* Interface */
#include <gf_command.h>

/* Engine */
#include <gf_log.h>

/* Standard */
#include <string.h>
#include <stdlib.h>

void gf_command_run(gf_engine_t* engine, char** list, int listc) {
	int i;
	for(i = 0; i < listc; i++) {
		int    j;
		char*  str  = malloc(strlen(list[i]) + 1);
		char** arg  = NULL;
		int    incr = 0;
		int    dq   = 0;
		strcpy(str, list[i]);

		for(j = 0;; j++) {
			if(str[j] == '"') {
				dq = dq ? 0 : 1;
			}
			if((!dq && (str[j] == ' ' || str[j] == '\t')) || str[j] == 0) {
				char  oldc = str[j];
				char* p;
				str[j] = 0;

				p = str + incr;
				if(p[0] == '"') p++;
				if(p[strlen(p) - 1] == '"') p[strlen(p) - 1] = 0;

				arrput(arg, p);

				incr = j + 1;
				j++;
				if(oldc == 0) break;
				for(; str[j] != 0 && (str[j] == ' ' || str[j] == '\t'); j++);
				j--;
			}
		}

		if(arg != NULL) {
			if(strcmp(arg[0], "width") == 0) {
				if(arrlen(arg) < 2) {
					gf_log_function(engine, "%s: Insufficient arguments", arg[0]);
				} else {
					engine->width = atoi(arg[1]);
				}
			} else if(strcmp(arg[0], "height") == 0) {
				if(arrlen(arg) < 2) {
					gf_log_function(engine, "%s: Insufficient arguments", arg[0]);
				} else {
					engine->height = atoi(arg[1]);
				}
			} else {
				gf_log_function(engine, "%s: Unknown command", arg[0]);
			}
			arrfree(arg);
		}

		free(str);
	}
}
