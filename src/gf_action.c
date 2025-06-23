#include <gf_pre.h>

/* External library */
#include <stb_ds.h>

/* Interface */
#include <gf_action.h>

/* Engine */
#include <gf_input.h>
#include <gf_command.h>

/* Standard */

void gf_action_process(gf_engine_t* engine, gf_input_t* input) {
	/**
	 * +command should be triggered when key is down
	 * -command should be triggered when key goes up
	 *  command should be triggered when key gets pressed
	 */
	int pkey = gf_input_get_pressed_key(input);
	int rkey = gf_input_get_released_key(input);

	/* Handle press command */
	if(pkey != -1) {
		const char* cmd = gf_input_key_binding(input, pkey);
		if(cmd != NULL) {
			char*  cmd2 = malloc(strlen(cmd) + 1);
			char** list;
			strcpy(cmd2, cmd);

			list = NULL;
			arrpush(list, cmd2);
			gf_command_run(engine, list, 1);
			arrfree(list);

			free(cmd2);
		}
	}

	/* Handle release command */
	if(rkey != -1) {
		const char* cmd = gf_input_key_binding(input, rkey);
		if(cmd != NULL && cmd[0] == '+') {
			char*  cmd2 = malloc(strlen(cmd) + 1);
			char** list;
			strcpy(cmd2, cmd);
			cmd2[0] = '-';

			list = NULL;
			arrpush(list, (char*)cmd);
			gf_command_run(engine, list, 1);
			arrfree(list);

			free(cmd2);
		}
	}
}
