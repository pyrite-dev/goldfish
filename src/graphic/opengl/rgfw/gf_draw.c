#define GF_EXPOSE_DRAW_PLATFORM
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_CORE
#define GF_EXPOSE_INPUT

#include <gf_pre.h>

/* External library */
#define RGFW_IMPLEMENTATION
#include <gf_opengl.h>
#include <stb_ds.h>

/* Interface */
#include <gf_draw_platform.h>

/* Engine */
#include <gf_draw_driver.h>
#include <gf_log.h>
#include <gf_draw.h>
#include <gf_input.h>

/* Standard */
#include <string.h>
#include <stdlib.h>

typedef struct keymap {
	int key;
	int value;
} keymap_t;

static keymap_t* keymaps = NULL;

static void whmput(int key, int value) { hmput(keymaps, key, value); }

void gf_draw_platform_begin(void) {
	int i;

	hmdefault(keymaps, -1);
	whmput(RGFW_escape, GF_INPUT_KEY_ESCAPE);

	whmput(RGFW_return, GF_INPUT_KEY_ENTER);

	whmput(RGFW_backSpace, GF_INPUT_KEY_BACKSPACE);
	whmput(RGFW_space, GF_INPUT_KEY_SPACE);

	for(i = 0; i < 10; i++) whmput(RGFW_0 + i, GF_INPUT_KEY_0 + i);

	for(i = 0; i < 26; i++) whmput(RGFW_a + i, GF_INPUT_KEY_A + i);

	for(i = 0; i < 12; i++) {
		whmput(RGFW_F1 + i, GF_INPUT_KEY_F1 + i);
	}

	whmput(RGFW_up, GF_INPUT_KEY_UP);
	whmput(RGFW_down, GF_INPUT_KEY_DOWN);
	whmput(RGFW_left, GF_INPUT_KEY_LEFT);
	whmput(RGFW_right, GF_INPUT_KEY_RIGHT);

	whmput(RGFW_shiftL, GF_INPUT_KEY_LEFT_SHIFT);
	whmput(RGFW_shiftR, GF_INPUT_KEY_RIGHT_SHIFT);

	whmput(RGFW_altL, GF_INPUT_KEY_LEFT_ALT);
	whmput(RGFW_altR, GF_INPUT_KEY_RIGHT_ALT);

	whmput(RGFW_controlL, GF_INPUT_KEY_LEFT_CONTROL);
	whmput(RGFW_controlR, GF_INPUT_KEY_RIGHT_CONTROL);

	whmput(RGFW_superL, GF_INPUT_KEY_LEFT_SUPER);
	whmput(RGFW_superR, GF_INPUT_KEY_RIGHT_SUPER);

	whmput(RGFW_tab, GF_INPUT_KEY_TAB);
	whmput(RGFW_capsLock, GF_INPUT_KEY_CAPSLOCK);

	whmput(RGFW_backtick, GF_INPUT_KEY_GRAVE);
	whmput(RGFW_minus, GF_INPUT_KEY_MINUS);
	whmput(RGFW_equals, GF_INPUT_KEY_EQUALS);
	whmput(RGFW_bracket, GF_INPUT_KEY_LEFT_BRACKET);
	whmput(RGFW_closeBracket, GF_INPUT_KEY_RIGHT_BRACKET);

	whmput(RGFW_backSlash, GF_INPUT_KEY_BACKSLASH);
	whmput(RGFW_semicolon, GF_INPUT_KEY_SEMICOLON);
	whmput(RGFW_apostrophe, GF_INPUT_KEY_QUOTE);
	whmput(RGFW_comma, GF_INPUT_KEY_COMMA);
	whmput(RGFW_period, GF_INPUT_KEY_PERIOD);
	whmput(RGFW_slash, GF_INPUT_KEY_SLASH);

	whmput(RGFW_insert, GF_INPUT_KEY_INSERT);
	whmput(RGFW_delete, GF_INPUT_KEY_DELETE);
	whmput(RGFW_home, GF_INPUT_KEY_HOME);
	whmput(RGFW_end, GF_INPUT_KEY_END);
	whmput(RGFW_pageUp, GF_INPUT_KEY_PAGE_UP);
	whmput(RGFW_pageDown, GF_INPUT_KEY_PAGE_DOWN);

	whmput(RGFW_printScreen, GF_INPUT_KEY_PRINT_SCREEN);
	whmput(RGFW_scrollLock, GF_INPUT_KEY_SCROLL_LOCK);
	whmput(RGFW_pause, GF_INPUT_KEY_PAUSE_BREAK);
	whmput(RGFW_numLock, GF_INPUT_KEY_NUM_LOCK);
}

void gf_draw_platform_end(void) {
	hmfree(keymaps);
	keymaps = NULL;
}

int gf_draw_platform_has_extension(gf_draw_t* draw, const char* query) {
	RGFW_window_makeCurrent(draw->platform->window);

	return RGFW_extensionSupported(query, strlen(query)) ? 1 : 0;
}

int gf_draw_platform_step(gf_draw_t* draw) {
	int ret = 0;
	int w, h;
	RGFW_window_makeCurrent(draw->platform->window);
	draw->close = RGFW_window_shouldClose(draw->platform->window);
	while(RGFW_window_checkEvent(draw->platform->window)) {
		if(draw->platform->window->event.type == RGFW_quit) {
			draw->close = 1;
			RGFW_window_setShouldClose(draw->platform->window, RGFW_FALSE);
			break;
		} else if(draw->platform->window->event.type == RGFW_mousePosChanged) {
			draw->input->mouse_x = draw->platform->window->event.point.x;
			draw->input->mouse_y = draw->platform->window->event.point.y;
		} else if(draw->platform->window->event.type == RGFW_windowMoved) {
			draw->x = draw->platform->window->r.x;
			draw->y = draw->platform->window->r.y;
		} else if(draw->platform->window->event.type == RGFW_windowResized) {
			draw->width  = draw->platform->window->r.w;
			draw->height = draw->platform->window->r.h;
		} else if(draw->platform->window->event.type == RGFW_mouseButtonPressed) {
			if(draw->platform->window->event.button == RGFW_mouseLeft) draw->input->mouse_flag |= GF_INPUT_MOUSE_LEFT_MASK;
			if(draw->platform->window->event.button == RGFW_mouseMiddle) draw->input->mouse_flag |= GF_INPUT_MOUSE_MIDDLE_MASK;
			if(draw->platform->window->event.button == RGFW_mouseRight) draw->input->mouse_flag |= GF_INPUT_MOUSE_RIGHT_MASK;
		} else if(draw->platform->window->event.type == RGFW_mouseButtonReleased) {
			if(draw->platform->window->event.button == RGFW_mouseLeft) draw->input->mouse_flag ^= GF_INPUT_MOUSE_LEFT_MASK;
			if(draw->platform->window->event.button == RGFW_mouseMiddle) draw->input->mouse_flag ^= GF_INPUT_MOUSE_MIDDLE_MASK;
			if(draw->platform->window->event.button == RGFW_mouseRight) draw->input->mouse_flag ^= GF_INPUT_MOUSE_RIGHT_MASK;
		} else if(draw->platform->window->event.type == RGFW_keyPressed) {
			int key = hmget(keymaps, draw->platform->window->event.key);
			gf_input_key_press(draw->input, key);
		} else if(draw->platform->window->event.type == RGFW_keyReleased) {
			int key = hmget(keymaps, draw->platform->window->event.key);
			if(!draw->platform->window->event.repeat) {
				gf_input_key_release(draw->input, key);
			}
		}
	}
	if(ret == 0) {
		gf_draw_driver_before(draw);
		gf_draw_frame(draw);
		gf_draw_driver_after(draw);

		RGFW_window_swapBuffers(draw->platform->window);
	}
	return ret;
}

gf_draw_platform_t* gf_draw_platform_create(gf_engine_t* engine, gf_draw_t* draw) {
	gf_draw_platform_t* platform = malloc(sizeof(*platform));
	memset(platform, 0, sizeof(*platform));
	platform->engine = engine;

	draw->platform = platform;

	platform->window = RGFW_createWindow(draw->title, RGFW_RECT(draw->x, draw->y, draw->width, draw->height), RGFW_windowHideMouse);
	if(platform->window == NULL) {
		gf_log_function(engine, "Failed to create window", "");
		gf_draw_platform_destroy(platform);
		return NULL;
	}
	platform->window->exitKey = RGFW_keyNULL;
	if(engine->icon != NULL) {
		RGFW_window_setIcon(platform->window, engine->icon, RGFW_AREA(engine->icon_width, engine->icon_height), 4);
	}
	platform->window->userPtr = draw;

	RGFW_window_makeCurrent(platform->window);
#ifdef GF_DO_SWAP_INTERVAL
	RGFW_window_swapInterval(platform->window, 1);
#endif
	return platform;
}

void gf_draw_platform_destroy(gf_draw_platform_t* platform) {
	if(platform->window != NULL) {
		RGFW_window_close(platform->window);
	}
	gf_log_function(platform->engine, "Destroyed platform-dependent part of drawing driver", "");
	free(platform);
}
