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
	SDL_Scancode key;
	int	     value;
} keymap_t;

static keymap_t* keymaps = NULL;

static void whmput(SDL_Scancode key, int value) { hmput(keymaps, key, value); }

void gf_draw_platform_begin(void) {
	int i;

	hmdefault(keymaps, -1);
	whmput(SDL_SCANCODE_ESCAPE, GF_INPUT_KEY_ESCAPE);

	whmput(SDL_SCANCODE_RETURN, GF_INPUT_KEY_ENTER);

	whmput(SDL_SCANCODE_BACKSPACE, GF_INPUT_KEY_BACKSPACE);
	whmput(SDL_SCANCODE_SPACE, GF_INPUT_KEY_SPACE);

	whmput(SDL_SCANCODE_0, GF_INPUT_KEY_0);
	for(i = 0; i < 9; i++) whmput(SDL_SCANCODE_1 + i, GF_INPUT_KEY_1 + i);

	for(i = 0; i < 26; i++) whmput(SDL_SCANCODE_A + i, GF_INPUT_KEY_A + i);

	for(i = 0; i < 12; i++) {
		whmput(SDL_SCANCODE_F1 + i, GF_INPUT_KEY_F1 + i);
	}

	whmput(SDL_SCANCODE_UP, GF_INPUT_KEY_UP);
	whmput(SDL_SCANCODE_DOWN, GF_INPUT_KEY_DOWN);
	whmput(SDL_SCANCODE_LEFT, GF_INPUT_KEY_LEFT);
	whmput(SDL_SCANCODE_RIGHT, GF_INPUT_KEY_RIGHT);

	whmput(SDL_SCANCODE_LSHIFT, GF_INPUT_KEY_LEFT_SHIFT);
	whmput(SDL_SCANCODE_RSHIFT, GF_INPUT_KEY_RIGHT_SHIFT);

	whmput(SDL_SCANCODE_LALT, GF_INPUT_KEY_LEFT_ALT);
	whmput(SDL_SCANCODE_RALT, GF_INPUT_KEY_RIGHT_ALT);

	whmput(SDL_SCANCODE_LCTRL, GF_INPUT_KEY_LEFT_CONTROL);
	whmput(SDL_SCANCODE_RCTRL, GF_INPUT_KEY_RIGHT_CONTROL);

	whmput(SDL_SCANCODE_LGUI, GF_INPUT_KEY_LEFT_SUPER);
	whmput(SDL_SCANCODE_RGUI, GF_INPUT_KEY_RIGHT_SUPER);

	whmput(SDL_SCANCODE_TAB, GF_INPUT_KEY_TAB);
	whmput(SDL_SCANCODE_CAPSLOCK, GF_INPUT_KEY_CAPSLOCK);

	whmput(SDL_SCANCODE_GRAVE, GF_INPUT_KEY_GRAVE);
	whmput(SDL_SCANCODE_MINUS, GF_INPUT_KEY_MINUS);
	whmput(SDL_SCANCODE_EQUALS, GF_INPUT_KEY_EQUALS);
	whmput(SDL_SCANCODE_LEFTBRACKET, GF_INPUT_KEY_LEFT_BRACKET);
	whmput(SDL_SCANCODE_RIGHTBRACKET, GF_INPUT_KEY_RIGHT_BRACKET);

	whmput(SDL_SCANCODE_BACKSLASH, GF_INPUT_KEY_BACKSLASH);
	whmput(SDL_SCANCODE_SEMICOLON, GF_INPUT_KEY_SEMICOLON);
	whmput(SDL_SCANCODE_APOSTROPHE, GF_INPUT_KEY_QUOTE);
	whmput(SDL_SCANCODE_COMMA, GF_INPUT_KEY_COMMA);
	whmput(SDL_SCANCODE_PERIOD, GF_INPUT_KEY_PERIOD);
	whmput(SDL_SCANCODE_SLASH, GF_INPUT_KEY_SLASH);

	whmput(SDL_SCANCODE_INSERT, GF_INPUT_KEY_INSERT);
	whmput(SDL_SCANCODE_DELETE, GF_INPUT_KEY_DELETE);
	whmput(SDL_SCANCODE_HOME, GF_INPUT_KEY_HOME);
	whmput(SDL_SCANCODE_END, GF_INPUT_KEY_END);
	whmput(SDL_SCANCODE_PAGEUP, GF_INPUT_KEY_PAGE_UP);
	whmput(SDL_SCANCODE_PAGEDOWN, GF_INPUT_KEY_PAGE_DOWN);

	whmput(SDL_SCANCODE_PRINTSCREEN, GF_INPUT_KEY_PRINT_SCREEN);
	whmput(SDL_SCANCODE_SCROLLLOCK, GF_INPUT_KEY_SCROLL_LOCK);
	whmput(SDL_SCANCODE_PAUSE, GF_INPUT_KEY_PAUSE_BREAK);
	whmput(SDL_SCANCODE_NUMLOCKCLEAR, GF_INPUT_KEY_NUM_LOCK);
}

void gf_draw_platform_end(void) {
	hmfree(keymaps);
	keymaps = NULL;
}

int gf_draw_platform_has_extension(gf_draw_t* draw, const char* query) {
	SDL_GL_MakeCurrent(draw->platform->window, draw->platform->context);

	return SDL_GL_ExtensionSupported(query) ? 1 : 0;
}

int gf_draw_platform_step(gf_draw_t* draw) {
	int	  ret = 0;
	int	  w, h;
	SDL_Event event;
	SDL_GL_MakeCurrent(draw->platform->window, draw->platform->context);
	draw->close = 0;
	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT) {
			draw->close = 1;
			break;
		} else if(event.type == SDL_MOUSEMOTION) {
			draw->input->mouse_x = event.motion.x;
			draw->input->mouse_y = event.motion.y;
		} else if(event.type == SDL_WINDOWEVENT) {
			if(event.window.event == SDL_WINDOWEVENT_MOVED) {
				draw->x = event.window.data1;
				draw->y = event.window.data2;
			} else if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || event.window.event == SDL_WINDOWEVENT_RESIZED) {
				int w;
				int h;
				SDL_GetWindowSize(draw->platform->window, &w, &h);
				draw->width  = w;
				draw->height = h;
				gf_draw_reshape(draw);
			}
		} else if(event.type == SDL_MOUSEBUTTONDOWN) {
			if(event.button.button == SDL_BUTTON_LEFT) draw->input->mouse_flag |= GF_INPUT_MOUSE_LEFT_MASK;
			if(event.button.button == SDL_BUTTON_MIDDLE) draw->input->mouse_flag |= GF_INPUT_MOUSE_MIDDLE_MASK;
			if(event.button.button == SDL_BUTTON_RIGHT) draw->input->mouse_flag |= GF_INPUT_MOUSE_RIGHT_MASK;
		} else if(event.type == SDL_MOUSEBUTTONUP) {
			if(event.button.button == SDL_BUTTON_LEFT) draw->input->mouse_flag ^= GF_INPUT_MOUSE_LEFT_MASK;
			if(event.button.button == SDL_BUTTON_MIDDLE) draw->input->mouse_flag ^= GF_INPUT_MOUSE_MIDDLE_MASK;
			if(event.button.button == SDL_BUTTON_RIGHT) draw->input->mouse_flag ^= GF_INPUT_MOUSE_RIGHT_MASK;
		} else if(event.type == SDL_KEYDOWN) {
			int key = hmget(keymaps, event.key.keysym.scancode);
			gf_input_key_press(draw->input, key);
		} else if(event.type == SDL_KEYUP) {
			int key = hmget(keymaps, event.key.keysym.scancode);
			if(!event.key.repeat) {
				gf_input_key_release(draw->input, key);
			}
		}
	}
	if(ret == 0) {
		gf_draw_driver_before(draw);
		gf_draw_frame(draw);
		gf_draw_driver_after(draw);

		SDL_GL_SwapWindow(draw->platform->window);
	}
	return ret;
}

gf_draw_platform_t* gf_draw_platform_create(gf_engine_t* engine, gf_draw_t* draw) {
	gf_draw_platform_t* platform = malloc(sizeof(*platform));
	memset(platform, 0, sizeof(*platform));
	platform->engine = engine;

	draw->platform = platform;

	SDL_Init(SDL_INIT_VIDEO);

	platform->window = SDL_CreateWindow(draw->title, draw->x, draw->y, draw->width, draw->height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if(platform->window == NULL) {
		gf_log_function(engine, "Failed to create window", "");
		gf_draw_platform_destroy(platform);
		return NULL;
	}
	SDL_ShowCursor(0);
	if(engine->icon != NULL) {
		SDL_Surface* s;
		gf_uint32_t  rmask;
		gf_uint32_t  gmask;
		gf_uint32_t  bmask;
		gf_uint32_t  amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
#endif
		s = SDL_CreateRGBSurfaceFrom(engine->icon, engine->icon_width, engine->icon_height, 32, 4 * engine->icon_width, rmask, gmask, bmask, amask);
		SDL_SetWindowIcon(platform->window, s);
		SDL_FreeSurface(s);
	}
	platform->context = SDL_GL_CreateContext(platform->window);

	SDL_GL_MakeCurrent(platform->window, platform->context);
#ifdef GF_DO_SWAP_INTERVAL
	SDL_GL_SetSwapInterval(1);
#endif
	return platform;
}

void gf_draw_platform_destroy(gf_draw_platform_t* platform) {
	if(platform->window != NULL) {
		SDL_GL_DeleteContext(platform->context);
		SDL_DestroyWindow(platform->window);
	}
	gf_log_function(platform->engine, "Destroyed platform-dependent part of drawing driver", "");
	free(platform);
}
