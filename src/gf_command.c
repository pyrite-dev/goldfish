#define GF_EXPOSE_CORE

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>

/* Interface */
#include <gf_command.h>

/* Engine */
#include <gf_log.h>
#include <gf_prop.h>
#include <gf_file.h>

/* Standard */
#include <string.h>
#include <stdlib.h>

void gf_command_file(gf_engine_t* engine, const char* path) {
	gf_file_t* f;
	if((f = gf_file_open(engine, path, "r")) != NULL) {
		char*  buf    = malloc(f->size + 1);
		int    incr   = 0;
		char** aelist = NULL;
		int    i;
		buf[f->size] = 0;
		gf_file_read(f, buf, f->size);

		for(i = 0;; i++) {
			if(buf[i] == 0 || buf[i] == '\n') {
				char  oldc = buf[i];
				char* line = buf + incr;
				buf[i]	   = 0;

				if(strlen(line) > 0) {
					arrput(aelist, line);
				}

				incr = i + 1;
				if(oldc == 0) break;
			} else if(buf[i] == '\r') {
				buf[i] = 0;
			}
		}

		if(aelist != NULL) {
			gf_log_function(engine, "%s: Executing", path);
			gf_command_run(engine, aelist, arrlen(aelist));
			arrfree(aelist);
		}

		free(buf);
		gf_file_close(f);
	} else {
		gf_log_function(engine, "%s: Not found", path);
	}
}

void gf_command_run(gf_engine_t* engine, char** list, int listc) {
	int i;
	for(i = 0; i < listc; i++) {
		int    j;
		char*  str  = malloc(strlen(list[i]) + 1);
		char** arg  = NULL;
		int    incr = 0;
		int    dq   = 0;
		if(list[i][0] == '#') continue;

		strcpy(str, list[i]);

		gf_log_function(engine, "Command: %s", str);

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
					gf_prop_set_integer(&engine->config, "width", atoi(arg[1]));
				}
			} else if(strcmp(arg[0], "height") == 0) {
				if(arrlen(arg) < 2) {
					gf_log_function(engine, "%s: Insufficient arguments", arg[0]);
				} else {
					gf_prop_set_integer(&engine->config, "height", atoi(arg[1]));
				}
			} else if(strcmp(arg[0], "texture") == 0) {
				if(arrlen(arg) < 2) {
					gf_log_function(engine, "%s: Insufficient arguments", arg[0]);
				} else if(strcmp(arg[1], "nearest") != 0 && strcmp(arg[1], "linear") != 0) {
					gf_log_function(engine, "%s: %s: Bad argument", arg[0], arg[1]);
				} else {
					gf_prop_set_text(&engine->config, "texture", arg[1]);
				}
			} else if(strcmp(arg[0], "exec") == 0) {
				if(arrlen(arg) < 2) {
					gf_log_function(engine, "%s: Insufficient arguments", arg[0]);
				} else {
					gf_command_file(engine, arg[1]);
				}
			} else {
				gf_log_function(engine, "%s: Unknown command", arg[0]);
			}
			arrfree(arg);
		}

		free(str);
	}
}
