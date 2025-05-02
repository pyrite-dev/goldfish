#define GF_EXPOSE_INPUT

#include <gf_pre.h>

/* External library */

/* Interface */
#include <gf_input.h>

/* Engine */
#include <gf_log.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

gf_input_t* gf_input_create(gf_engine_t* engine) {
	gf_input_t* input = malloc(sizeof(*input));
	memset(input, 0, sizeof(*input));
	input->engine = engine;

	input->mouse_x	  = -1;
	input->mouse_y	  = -1;
	input->mouse_flag = 0;

	return input;
}

void gf_input_destroy(gf_input_t* input) {
	gf_log_function(input->engine, "Destroyed input interface", "");
	free(input);
}
