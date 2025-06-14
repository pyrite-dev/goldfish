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

const char* gf_input_key_names[] = {
	/* GF_INPUT_KEY_ESCAPE */ "escape",
	/* GF_INPUT_KEY_TAB */ "tab",
	/* GF_INPUT_KEY_CAPSLOCK */ "capslock",

	/* GF_INPUT_KEY_BACKSPACE */ "backspace",
	/* GF_INPUT_KEY_ENTER */ "enter",
	/* GF_INPUT_KEY_FN */ "fn",

	/* GF_INPUT_KEY_UP */ "up",
	/* GF_INPUT_KEY_DOWN */ "down",
	/* GF_INPUT_KEY_LEFT */ "left",
	/* GF_INPUT_KEY_RIGHT */ "right",

	/* GF_INPUT_KEY_INSERT */ "insert",
	/* GF_INPUT_KEY_DELETE */ "delete",
	/* GF_INPUT_KEY_HOME */ "home",
	/* GF_INPUT_KEY_END */ "end",
	/* GF_INPUT_KEY_PAGE_UP */ "pageup",
	/* GF_INPUT_KEY_PAGE_DOWN */ "pagedown",

	/* GF_INPUT_KEY_PRINT_SCREEN */ "printscr",
	/* GF_INPUT_KEY_SCROLL_LOCK */ "scrlock",
	/* GF_INPUT_KEY_PAUSE_BREAK */ "pausebreak",

	/* GF_INPUT_KEY_SPACE */ "space",

	/* GF_INPUT_KEY_LEFT_SHIFT */ "lshift",
	/* GF_INPUT_KEY_RIGHT_SHIFT */ "rshift",

	/* GF_INPUT_KEY_LEFT_CONTROL */ "lctrl",
	/* GF_INPUT_KEY_RIGHT_CONTROL */ "rctrl",

	/* GF_INPUT_KEY_LEFT_ALT */ "lalt",
	/* GF_INPUT_KEY_RIGHT_ALT */ "ralt",

	/* GF_INPUT_KEY_LEFT_SUPER */ "lsup",
	/* GF_INPUT_KEY_RIGHT_SUPER */ "rsup",

	/* GF_INPUT_KEY_F1 */ "f1",
	/* GF_INPUT_KEY_F2 */ "f2",
	/* GF_INPUT_KEY_F3 */ "f3",
	/* GF_INPUT_KEY_F4 */ "f4",
	/* GF_INPUT_KEY_F5 */ "f5",
	/* GF_INPUT_KEY_F6 */ "f6",
	/* GF_INPUT_KEY_F7 */ "f7",
	/* GF_INPUT_KEY_F8 */ "f8",
	/* GF_INPUT_KEY_F9 */ "f9",
	/* GF_INPUT_KEY_F10 */ "f10",
	/* GF_INPUT_KEY_F11 */ "f11",
	/* GF_INPUT_KEY_F12 */ "f12",

	/* GF_INPUT_KEY_GRAVE */ "`",
	/* GF_INPUT_KEY_0 */ "0",
	/* GF_INPUT_KEY_1 */ "1",
	/* GF_INPUT_KEY_2 */ "2",
	/* GF_INPUT_KEY_3 */ "3",
	/* GF_INPUT_KEY_4 */ "4",
	/* GF_INPUT_KEY_5 */ "5",
	/* GF_INPUT_KEY_6 */ "6",
	/* GF_INPUT_KEY_7 */ "7",
	/* GF_INPUT_KEY_8 */ "8",
	/* GF_INPUT_KEY_9 */ "9",

	/* GF_INPUT_KEY_A */ "a",
	/* GF_INPUT_KEY_B */ "b",
	/* GF_INPUT_KEY_C */ "c",
	/* GF_INPUT_KEY_D */ "d",
	/* GF_INPUT_KEY_E */ "e",
	/* GF_INPUT_KEY_F */ "f",
	/* GF_INPUT_KEY_G */ "g",
	/* GF_INPUT_KEY_H */ "h",
	/* GF_INPUT_KEY_I */ "i",
	/* GF_INPUT_KEY_J */ "j",
	/* GF_INPUT_KEY_K */ "k",
	/* GF_INPUT_KEY_L */ "l",
	/* GF_INPUT_KEY_M */ "m",
	/* GF_INPUT_KEY_N */ "n",
	/* GF_INPUT_KEY_O */ "o",
	/* GF_INPUT_KEY_P */ "p",
	/* GF_INPUT_KEY_Q */ "q",
	/* GF_INPUT_KEY_R */ "r",
	/* GF_INPUT_KEY_S */ "s",
	/* GF_INPUT_KEY_T */ "t",
	/* GF_INPUT_KEY_U */ "u",
	/* GF_INPUT_KEY_V */ "v",
	/* GF_INPUT_KEY_W */ "w",
	/* GF_INPUT_KEY_X */ "x",
	/* GF_INPUT_KEY_Y */ "y",
	/* GF_INPUT_KEY_Z */ "z",

	/* GF_INPUT_KEY_MINUS */ "-",
	/* GF_INPUT_KEY_EQUALS */ "=",
	/* GF_INPUT_KEY_LEFT_BRACKET */ "[",
	/* GF_INPUT_KEY_RIGHT_BRACKET */ "]",
	/* GF_INPUT_KEY_BACKSLASH */ "\\",
	/* GF_INPUT_KEY_SEMICOLON */ ";",
	/* GF_INPUT_KEY_QUOTE */ "'",
	/* GF_INPUT_KEY_COMMA */ ",",
	/* GF_INPUT_KEY_PERIOD */ ".",
	/* GF_INPUT_KEY_SLASH */ "/",

	/* GF_INPUT_KEY_NUM_LOCK */ "numlock",
	/* GF_INPUT_KEY_NUMPAD_SLASH */ "numpad_slash",
	/* GF_INPUT_KEY_NUMPAD_ASTERISK */ "numpad_asterisk",
	/* GF_INPUT_KEY_NUMPAD_MINUS */ "numpad_minus",
	/* GF_INPUT_KEY_NUMPAD_PLUS */ "numpad_plus",
	/* GF_INPUT_KEY_NUMPAD_ENTER */ "numpad_enter",
	/* GF_INPUT_KEY_NUMPAD_PERIOD */ "numpad_period",
	/* GF_INPUT_KEY_NUMPAD_0 */ "numpad_0",
	/* GF_INPUT_KEY_NUMPAD_1 */ "numpad_1",
	/* GF_INPUT_KEY_NUMPAD_2 */ "numpad_2",
	/* GF_INPUT_KEY_NUMPAD_3 */ "numpad_3",
	/* GF_INPUT_KEY_NUMPAD_4 */ "numpad_4",
	/* GF_INPUT_KEY_NUMPAD_5 */ "numpad_5",
	/* GF_INPUT_KEY_NUMPAD_6 */ "numpad_6",
	/* GF_INPUT_KEY_NUMPAD_7 */ "numpad_7",
	/* GF_INPUT_KEY_NUMPAD_8 */ "numpad_8",
	/* GF_INPUT_KEY_NUMPAD_9 */ "numpad_9",
};

gf_input_t* gf_input_create(gf_engine_t* engine) {
	gf_input_t* input = malloc(sizeof(*input));
	memset(input, 0, sizeof(*input));
	input->engine = engine;

	input->mouse_x	  = -1;
	input->mouse_y	  = -1;
	input->mouse_flag = 0;

	input->key_state  = NULL;
	input->key_queue  = NULL;
	input->key_rqueue = NULL;

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

int gf_input_key_released(gf_input_t* input, int key) {
	if(arrlen(input->key_rqueue) < 1) return 0;

	return input->key_rqueue[0] == key ? 1 : 0;
}

int gf_input_get_pressed_key(gf_input_t* input) {
	if(arrlen(input->key_queue) < 1) return -1;

	return input->key_queue[0];
}

int gf_input_get_released_key(gf_input_t* input) {
	if(arrlen(input->key_rqueue) < 1) return -1;

	return input->key_rqueue[0];
}

char gf_input_key_char_of(gf_input_t* input, int key) {
	if(GF_INPUT_KEY_A <= key && key <= GF_INPUT_KEY_Z) {
		return 'a' + (key - GF_INPUT_KEY_A);
	} else if(GF_INPUT_KEY_0 <= key && key <= GF_INPUT_KEY_9) {
		return '0' + (key - GF_INPUT_KEY_0);
	} else if(key == GF_INPUT_KEY_SPACE) {
		return ' ';
	}

	return 0;
}

char gf_input_key_char(gf_input_t* input) {
	if(arrlen(input->key_queue) < 1) return 0;

	return gf_input_key_char_of(input, input->key_queue[0]);
}

const char* gf_input_key_name(int key) {
	if (key > GF_INPUT_KEY_LAST)
		return NULL;
	return gf_input_key_names[key];
}

int gf_input_key_from_name(const char* name) {
	for (int i = 0; i <= GF_INPUT_KEY_LAST; i++) {
		if (strcmp(name, gf_input_key_names[i]) == 0)
			return i;
	}
	return -1;
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
	arrput(input->key_rqueue, key);
}

void gf_input_bind_key(gf_input_t* input, int key, const char* command) {
	if (input->keymap[key] != NULL) {
		free(input->keymap[key]);
	}
	if (command == NULL) {
		input->keymap[key] = NULL;
		return;
	}
	size_t n = strlen(command)+1;
	char* c = malloc(n * sizeof(char));
	strcpy(c, command);
	input->keymap[key] = c;
}

int gf_input_next_bound_key(gf_input_t* input, int last_key) {
	for (int i = last_key+1; i <= GF_INPUT_KEY_LAST; i++) {
		if (input->keymap[i] != NULL) {
			return i;
		}
	}
	return -1;
}

const char* gf_input_key_binding(gf_input_t* input, int key) {
	return input->keymap[key];
}
