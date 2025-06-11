#define GF_EXPOSE_INPUT

#include <gf_pre.h>

/* External library */
#include <stb_ds.h>

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

	input->key_state = NULL;
	input->key_queue = NULL;

	return input;
}

void gf_input_destroy(gf_input_t* input) {
	gf_log_function(input->engine, "Destroyed input interface", "");
	free(input);
}

int gf_input_key_down(gf_input_t* input, int key) {
	int i;
	int has = 0;
	for(i = 0; i < arrlen(input->key_state); i++) {
		if(input->key_state[i] == key) {
			return 1;
		}
	}
	return 0;
}

int gf_input_key_pressed(gf_input_t* input, int key) {
	if(arrlen(input->key_queue) < 1) return 0;

	return input->key_queue[0] == key ? 1 : 0;
}

char gf_input_key_name(gf_input_t* input, int key) {
	if(GF_INPUT_KEY_A <= key && key <= GF_INPUT_KEY_Z) {
		return 'a' + (key - GF_INPUT_KEY_A);
	} else if(GF_INPUT_KEY_0 <= key && key <= GF_INPUT_KEY_9) {
		return '0' + (key - GF_INPUT_KEY_0);
	}else if(key == GF_INPUT_KEY_SPACE){
		return ' ';
	}

	return 0;
}

char gf_input_key_char(gf_input_t* input) {
	if(arrlen(input->key_queue) < 1) return 0;

	return gf_input_key_name(input, input->key_queue[0]);
}

void gf_input_key_press(gf_input_t* input, int key) {
	int i;
	int has = 0;
	for(i = 0; i < arrlen(input->key_state); i++) {
		if(input->key_state[i] == key) {
			has = 1;
			break;
		}
	}
	if(!has) {
		arrput(input->key_state, key);
	}
	arrput(input->key_queue, key);
}

void gf_input_key_release(gf_input_t* input, int key) {
	int i;
	for(i = 0; i < arrlen(input->key_state); i++) {
		if(input->key_state[i] == key) {
			arrdel(input->key_state, i);
			break;
		}
	}
}
