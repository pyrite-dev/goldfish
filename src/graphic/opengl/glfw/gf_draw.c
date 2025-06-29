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
 * File Name:    gf_draw.c
 * Author:       Nishi
 *
 * Functionality:
 * TODO: Write here
 *
 **********************************************************************************
 * @endif
 */

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
#include <gf_type/compat.h>

/* Standard */
#include <string.h>
#include <stdlib.h>

typedef struct keymap {
	int key;
	int value;
} keymap_t;

static keymap_t* keymaps = NULL;

static void whmput(int key, int value) {
	int actualkey = glfwGetKeyScancode(key);
	hmput(keymaps, actualkey, value);
}

void gf_draw_platform_begin(void) {
	int i;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

	hmdefault(keymaps, -1);
	whmput(GLFW_KEY_ESCAPE, GF_INPUT_KEY_ESCAPE);

	whmput(GLFW_KEY_ENTER, GF_INPUT_KEY_ENTER);

	whmput(GLFW_KEY_BACKSPACE, GF_INPUT_KEY_BACKSPACE);
	whmput(GLFW_KEY_SPACE, GF_INPUT_KEY_SPACE);

	whmput(GLFW_KEY_0, GF_INPUT_KEY_0);
	for(i = 0; i < 9; i++) whmput(GLFW_KEY_1 + i, GF_INPUT_KEY_1 + i);

	for(i = 0; i < 26; i++) whmput(GLFW_KEY_A + i, GF_INPUT_KEY_A + i);

	for(i = 0; i < 12; i++) {
		whmput(GLFW_KEY_F1 + i, GF_INPUT_KEY_F1 + i);
	}

	whmput(GLFW_KEY_UP, GF_INPUT_KEY_UP);
	whmput(GLFW_KEY_DOWN, GF_INPUT_KEY_DOWN);
	whmput(GLFW_KEY_LEFT, GF_INPUT_KEY_LEFT);
	whmput(GLFW_KEY_RIGHT, GF_INPUT_KEY_RIGHT);

	whmput(GLFW_KEY_LEFT_SHIFT, GF_INPUT_KEY_LEFT_SHIFT);
	whmput(GLFW_KEY_RIGHT_SHIFT, GF_INPUT_KEY_RIGHT_SHIFT);

	whmput(GLFW_KEY_LEFT_ALT, GF_INPUT_KEY_LEFT_ALT);
	whmput(GLFW_KEY_RIGHT_ALT, GF_INPUT_KEY_RIGHT_ALT);

	whmput(GLFW_KEY_LEFT_CONTROL, GF_INPUT_KEY_LEFT_CONTROL);
	whmput(GLFW_KEY_RIGHT_CONTROL, GF_INPUT_KEY_RIGHT_CONTROL);

	whmput(GLFW_KEY_LEFT_SUPER, GF_INPUT_KEY_LEFT_SUPER);
	whmput(GLFW_KEY_RIGHT_SUPER, GF_INPUT_KEY_RIGHT_SUPER);

	whmput(GLFW_KEY_TAB, GF_INPUT_KEY_TAB);
	whmput(GLFW_KEY_CAPS_LOCK, GF_INPUT_KEY_CAPSLOCK);

	whmput(GLFW_KEY_GRAVE_ACCENT, GF_INPUT_KEY_GRAVE);
	whmput(GLFW_KEY_MINUS, GF_INPUT_KEY_MINUS);
	whmput(GLFW_KEY_EQUAL, GF_INPUT_KEY_EQUALS);
	whmput(GLFW_KEY_LEFT_BRACKET, GF_INPUT_KEY_LEFT_BRACKET);
	whmput(GLFW_KEY_RIGHT_BRACKET, GF_INPUT_KEY_RIGHT_BRACKET);

	whmput(GLFW_KEY_BACKSLASH, GF_INPUT_KEY_BACKSLASH);
	whmput(GLFW_KEY_SEMICOLON, GF_INPUT_KEY_SEMICOLON);
	whmput(GLFW_KEY_APOSTROPHE, GF_INPUT_KEY_QUOTE);
	whmput(GLFW_KEY_COMMA, GF_INPUT_KEY_COMMA);
	whmput(GLFW_KEY_PERIOD, GF_INPUT_KEY_PERIOD);
	whmput(GLFW_KEY_SLASH, GF_INPUT_KEY_SLASH);

	whmput(GLFW_KEY_INSERT, GF_INPUT_KEY_INSERT);
	whmput(GLFW_KEY_DELETE, GF_INPUT_KEY_DELETE);
	whmput(GLFW_KEY_HOME, GF_INPUT_KEY_HOME);
	whmput(GLFW_KEY_END, GF_INPUT_KEY_END);
	whmput(GLFW_KEY_PAGE_UP, GF_INPUT_KEY_PAGE_UP);
	whmput(GLFW_KEY_PAGE_DOWN, GF_INPUT_KEY_PAGE_DOWN);

	whmput(GLFW_KEY_PRINT_SCREEN, GF_INPUT_KEY_PRINT_SCREEN);
	whmput(GLFW_KEY_SCROLL_LOCK, GF_INPUT_KEY_SCROLL_LOCK);
	whmput(GLFW_KEY_PAUSE, GF_INPUT_KEY_PAUSE_BREAK);
	whmput(GLFW_KEY_NUM_LOCK, GF_INPUT_KEY_NUM_LOCK);
}

void gf_draw_platform_end(void) {
	hmfree(keymaps);
	keymaps = NULL;
}

static void gf_glfw_size(GLFWwindow* window, int w, int h) {
	gf_draw_t* draw = (gf_draw_t*)glfwGetWindowUserPointer(window);
	draw->width	= w;
	draw->height	= h;
	glfwSetWindowSize(window, w, h);
	gf_draw_reshape(draw);
}

static void gf_glfw_cursor(GLFWwindow* window, double x, double y) {
	gf_draw_t* draw = (gf_draw_t*)glfwGetWindowUserPointer(window);
	if(draw->input != NULL) {
		draw->input->mouse_x = x;
		draw->input->mouse_y = y;
	}
}

static void gf_glfw_button(GLFWwindow* window, int button, int action, int mods) {
	gf_draw_t* draw = (gf_draw_t*)glfwGetWindowUserPointer(window);
	if(draw->input != NULL) {
		if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) draw->input->mouse_flag |= GF_INPUT_MOUSE_LEFT_MASK;
		if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) draw->input->mouse_flag ^= GF_INPUT_MOUSE_LEFT_MASK;

		if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) draw->input->mouse_flag |= GF_INPUT_MOUSE_RIGHT_MASK;
		if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) draw->input->mouse_flag ^= GF_INPUT_MOUSE_RIGHT_MASK;

		if(button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) draw->input->mouse_flag |= GF_INPUT_MOUSE_MIDDLE_MASK;
		if(button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) draw->input->mouse_flag ^= GF_INPUT_MOUSE_MIDDLE_MASK;
	}
}

static void gf_glfw_key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	gf_draw_t* draw = (gf_draw_t*)glfwGetWindowUserPointer(window);
	if(draw->input != NULL) {
		if(action == GLFW_PRESS || action == GLFW_REPEAT) {
			int key = hmget(keymaps, scancode);
			gf_input_key_press(draw->input, key);
		} else if(action == GLFW_RELEASE) {
			int key = hmget(keymaps, scancode);
			gf_input_key_release(draw->input, key);
		}
	}
}

int gf_draw_platform_has_extension(gf_draw_t* draw, const char* query) {
	glfwMakeContextCurrent(draw->platform->window);

	return glfwExtensionSupported(query) ? 1 : 0;
}

int gf_draw_platform_step(gf_draw_t* draw) {
	int ret = 0;
	int w, h;
	glfwMakeContextCurrent(draw->platform->window);
	draw->close = glfwWindowShouldClose(draw->platform->window) ? 1 : 0;
	if(draw->close) glfwSetWindowShouldClose(draw->platform->window, GLFW_FALSE);
	glfwPollEvents();
	if(ret == 0) {
		gf_draw_driver_before(draw);
		gf_draw_frame(draw);
		gf_draw_driver_after(draw);

		glfwSwapBuffers(draw->platform->window);
	}
	return ret;
}

gf_draw_platform_t* gf_draw_platform_create(gf_engine_t* engine, gf_draw_t* draw) {
	GLFWimage	    icons[1];
	gf_draw_platform_t* platform = malloc(sizeof(*platform));
	memset(platform, 0, sizeof(*platform));
	platform->engine = engine;

	draw->platform = platform;

	platform->window = glfwCreateWindow(draw->width, draw->height, draw->title, NULL, NULL);
	if(platform->window == NULL) {
		gf_log_function(engine, "Failed to create window", "");
		gf_draw_platform_destroy(platform);
		return NULL;
	}

	icons[0].pixels = malloc(engine->icon_width * engine->icon_height * 4);
	memcpy(icons[0].pixels, engine->icon, engine->icon_width * engine->icon_height * 4);
	icons[0].width	= engine->icon_width;
	icons[0].height = engine->icon_height;

	glfwSetInputMode(platform->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetWindowIcon(platform->window, 1, icons);

	free(icons[0].pixels);

	glfwSetWindowUserPointer(platform->window, draw);
	glfwSetCursorPosCallback(platform->window, gf_glfw_cursor);
	glfwSetWindowSizeCallback(platform->window, gf_glfw_size);
	glfwSetMouseButtonCallback(platform->window, gf_glfw_button);
	glfwSetIMEStatusCallback(platform->window, gf_glfw_ime);
	glfwSetKeyCallback(platform->window, gf_glfw_key);

	glfwMakeContextCurrent(platform->window);
#ifdef GF_DO_SWAP_INTERVAL
	glfwSwapInterval(1);
#endif
	return platform;
}

void gf_draw_platform_destroy(gf_draw_platform_t* platform) {
	if(platform->window != NULL) {
		glfwDestroyWindow(platform->window);
	}
	gf_log_function(platform->engine, "Destroyed platform-dependent part of drawing driver", "");
	free(platform);
}
