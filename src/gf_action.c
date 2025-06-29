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
 * File Name:    gf_action.c
 * Author:       Nishi
 *
 * Functionality:
 * TODO: Write here
 *
 **********************************************************************************
 * @endif
 */

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
