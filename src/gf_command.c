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
 * File Name:    gf_command.c
 * Author:       Nishi
 *
 * Functionality:
 * Handle console commands.
 *
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_CORE
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_CLIENT
#define GF_EXPOSE_COMMAND

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>
#include <stb_image_write.h>

/* Interface */
#include <gf_command.h>

/* Engine */
#include <gf_type/compat.h>
#include <gf_type/command.h>
#include <gf_log.h>
#include <gf_prop.h>
#include <gf_file.h>
#include <gf_input.h>
#include <gf_gui.h>
#include <gf_graphic.h>

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
		gf_file_read(f, buf, f->size);
		buf[f->size] = 0;

		for(i = 0;; i++) {
			if(buf[i] == 0 || buf[i] == '\n') {
				char  oldc = buf[i];
				char* line = buf + incr;
				int   j;

				buf[i] = 0;

				for(j = 0; line[j] != 0; j++) {
					if(line[j] == '\r') {
						line[j] = 0;
						break;
					}
				}

				if(strlen(line) > 0) {
					arrput(aelist, line);
				}

				incr = i + 1;
				if(oldc == 0) break;
			}
		}

		gf_log_function(engine, "%s: Executing", path);
		if(aelist != NULL) {
			gf_command_run(engine, aelist, arrlen(aelist));
			arrfree(aelist);
		}

		free(buf);
		gf_file_close(f);
	} else {
		gf_log_function(engine, "%s: Not found", path);
	}
}

char* gf_command_join_args(const char** args, int start, int end) {
	int len = 0;
	int i;
	int x;
	char* new;
	/* First "1 + " for '\0', remaining for ' ' */
	for(i = start; i < end; i++) len += 1 + strlen(args[i]);

	new = malloc(len * sizeof(char));
	for(i = start, x = 0; i < end; i++) {
		if(i > start) {
			new[x] = ' ';
			x++;
		}

		strcpy(&new[x], (char*)args[i]);
		x += strlen(args[i]);
	}

	return new;
}

gf_bool_t gf_command_exec_builtin(gf_engine_t* engine, char** arg) {
	if(strcmp(arg[0], "width") == 0) {
		if(arrlen(arg) < 2) {
			gf_log_function(engine, "%s: Insufficient arguments", arg[0]);
			return gf_true;
		}

		gf_prop_set_integer(&engine->config, "width", atoi(arg[1]));
	} else if(strcmp(arg[0], "height") == 0) {
		if(arrlen(arg) < 2) {
			gf_log_function(engine, "%s: Insufficient arguments", arg[0]);
			return gf_true;
		}

		gf_prop_set_integer(&engine->config, "height", atoi(arg[1]));
	} else if(strcmp(arg[0], "texture") == 0) {
		if(arrlen(arg) < 2) {
			gf_log_function(engine, "%s: Insufficient arguments", arg[0]);
		} else if(strcmp(arg[1], "nearest") != 0 && strcmp(arg[1], "linear") != 0) {
			gf_log_function(engine, "%s: %s: Bad argument", arg[0], arg[1]);
		} else {
			gf_prop_set_text(&engine->config, "texture-filter", arg[1]);
		}
	} else if(strcmp(arg[0], "bad3d") == 0 || strcmp(arg[0], "w3d") == 0) {
		if(arrlen(arg) < 2) {
			gf_log_function(engine, "%s: Insufficient arguments", arg[0]);
		} else {
			gf_prop_set_integer(&engine->config, "bad3d", atoi(arg[1]));
		}
	} else if(strcmp(arg[0], "exec") == 0) {
		if(arrlen(arg) < 2) {
			gf_log_function(engine, "%s: Insufficient arguments", arg[0]);
			return gf_true;
		}

		gf_command_file(engine, arg[1]);
	} else if(strcmp(arg[0], "intro") == 0) {
		if(engine == NULL || engine->client == NULL || engine->client->draw == NULL) {
			return gf_true;
		}

		engine->client->draw->intro.finished = 0;
		engine->client->draw->intro.frame    = 0;
	} else if(strcmp(arg[0], "screenshot") == 0) {
		unsigned char* fb;
		if(engine == NULL || engine->client == NULL || engine->client->draw == NULL) {
			return gf_true;
		}

		fb = gf_graphic_get_screen(engine->client->draw, 0, 0, engine->client->draw->width, engine->client->draw->height);
		/* TODO: fix this when i add a function to get game base directory */
		stbi_write_png("output.png", engine->client->draw->width, engine->client->draw->height, 4, fb, 0);
		free(fb);
	} else if(strcmp(arg[0], "echo") == 0) {
		if(engine == NULL || arrlen(arg) < 2) {
			return gf_true;
		}

		gf_log(engine, "%s\n", arg[1]);
	} else if(strcmp(arg[0], "console") == 0) {
		int h;
		if(engine == NULL || engine->client == NULL || engine->client->draw == NULL || engine->client->draw->gui == NULL) {
			return gf_true;
		}

		h = gf_prop_get_integer(gf_gui_get_prop(engine->client->draw->gui, engine->client->draw->console), "hide");
		h = h == 0 ? 1 : 0;
		gf_prop_set_integer(gf_gui_get_prop(engine->client->draw->gui, engine->client->draw->console), "hide", h);
	} else if(strcmp(arg[0], "bind") == 0) {
		int key;
		if(arrlen(arg) < 2) {
			gf_log_function(engine, "%s: Insufficient arguments", arg[0]);
			return gf_true;
		}

		if(engine == NULL || engine->client == NULL || engine->client->input == NULL) {
			gf_log_function(engine, "%s: bind cannot be called from the server", arg[0]);
			return gf_true;
		}

		key = gf_input_key_from_name(arg[1]);
		if(key == -1) {
			gf_log_function(engine, "cannot bind unknown key \"%s\"", arg[1]);
			return gf_true;
		}

		if(arrlen(arg) == 2) {
			gf_input_bind_key(engine->client->input, key, NULL);
		} else {
			char* remargs = gf_command_join_args((const char**)arg, 2, arrlen(arg));
			gf_input_bind_key(engine->client->input, key, remargs);
			free(remargs);
		}
	} else if(strcmp(arg[0], "alias") == 0) {
		char*		   alias;
		gf_command_alias_t prevcmd;
		char*		   remargs;
		if(arrlen(arg) < 3) {
			gf_log_function(engine, "%s: Insufficient arguments", arg[0]);
			return gf_true;
		}

		if(engine == NULL) {
			return gf_true;
		}

		alias = malloc(strlen(arg[1]) + 1);
		strcpy(alias, arg[1]);

		/* Free previous key/command combo */
		if((prevcmd = shgets(engine->command_aliases, arg[1])).key != NULL) {
			free(prevcmd.key);
			free(prevcmd.value);
		}
		shdel(engine->command_aliases, arg[1]);

		remargs = gf_command_join_args((const char**)arg, 2, arrlen(arg));
		shput(engine->command_aliases, alias, remargs);
	} else if(strcmp(arg[0], "key_listboundkeys") == 0) {
		int key;
		if(engine == NULL || engine->client == NULL || engine->client->input == NULL) {
			gf_log_function(engine, "%s: bind cannot be called from the server", arg[0]);
			return gf_true;
		}

		key = -1;
		while((key = gf_input_next_bound_key(engine->client->input, key)) != -1) {
			const char* key_name = gf_input_key_name(key);
			const char* key_cmd  = gf_input_key_binding(engine->client->input, key);

			/* I don't think there's a print function yet. */
			/* TODO: replace this */
			gf_log(engine, "[%s]: %s", key_name, key_cmd);
		}
	} else {
		return gf_false;
	}
	return gf_true;
}

void gf_command_run(gf_engine_t* engine, char** list, int listc) {
	int i;
	for(i = 0; i < listc; i++) {
		int	    j;
		char*	    str	 = malloc(strlen(list[i]) + 1);
		char**	    arg	 = NULL;
		int	    incr = 0;
		int	    dq	 = 0;
		const char* alias_cmd;
		int	    found;
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

		if(arg == NULL) {
			free(str);
			return;
		}

		/* Handle aliases */
		alias_cmd = shget(engine->command_aliases, arg[0]);
		if(alias_cmd != NULL) {
			char*  cmd;
			char** list2;
			free(str);

			cmd   = malloc(strlen(alias_cmd) + 1);
			list2 = NULL;
			strcpy(cmd, alias_cmd);

			arrpush(list2, (char*)cmd);
			gf_command_run(engine, list2, 1);
			arrfree(list2);

			return;
		}

		found = gf_command_exec_builtin(engine, arg);
		if(!found) {
			gf_log_function(engine, "%s: Unknown command", arg[0]);
		}
		arrfree(arg);

		free(str);
	}
}
