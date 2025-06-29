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
 * Author:       IoI_xD
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
#include <gf_opengl.h>

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

void gf_draw_platform_begin(void) {}

void gf_draw_platform_end(void) {}

int gf_draw_platform_has_extension(gf_draw_t* draw, const char* query) {
	// const char* ext = NULL;
	// const char* ptr;
	// const int   len = strlen(query);

	// RGFW_window_makeCurrent(draw->platform->window);

	return 0;
}

int gf_draw_platform_step(gf_draw_t* draw) {
	int ret = 0;
	// int w, h;
	// RGFW_window_makeCurrent(draw->platform->window);
	// draw->close = RGFW_window_shouldClose(draw->platform->window);
	// while(RGFW_window_checkEvent(draw->platform->window)) {
	// 	if(draw->platform->window->event.type == RGFW_quit) {
	// 		draw->close = 1;
	// 		RGFW_window_setShouldClose(draw->platform->window, RGFW_FALSE);
	// 		break;
	// 	} else if(draw->platform->window->event.type == RGFW_mousePosChanged) {
	// 		draw->input->mouse_x = draw->platform->window->event.point.x;
	// 		draw->input->mouse_y = draw->platform->window->event.point.y;
	// 	} else if(draw->platform->window->event.type == RGFW_windowMoved) {
	// 		draw->x = draw->platform->window->r.x;
	// 		draw->y = draw->platform->window->r.y;
	// 	} else if(draw->platform->window->event.type == RGFW_windowResized) {
	// 		draw->width  = draw->platform->window->r.w;
	// 		draw->height = draw->platform->window->r.h;
	// 	} else if(draw->platform->window->event.type == RGFW_mouseButtonPressed) {
	// 		if(draw->platform->window->event.button == RGFW_mouseLeft) draw->input->mouse_flag |= GF_INPUT_MOUSE_LEFT_MASK;
	// 		if(draw->platform->window->event.button == RGFW_mouseMiddle) draw->input->mouse_flag |= GF_INPUT_MOUSE_MIDDLE_MASK;
	// 		if(draw->platform->window->event.button == RGFW_mouseRight) draw->input->mouse_flag |= GF_INPUT_MOUSE_RIGHT_MASK;
	// 	} else if(draw->platform->window->event.type == RGFW_mouseButtonReleased) {
	// 		if(draw->platform->window->event.button == RGFW_mouseLeft) draw->input->mouse_flag ^= GF_INPUT_MOUSE_LEFT_MASK;
	// 		if(draw->platform->window->event.button == RGFW_mouseMiddle) draw->input->mouse_flag ^= GF_INPUT_MOUSE_MIDDLE_MASK;
	// 		if(draw->platform->window->event.button == RGFW_mouseRight) draw->input->mouse_flag ^= GF_INPUT_MOUSE_RIGHT_MASK;
	// 	}
	// }
	// if(ret == 0) {
	// 	gf_draw_driver_before(draw);
	// 	gf_draw_frame(draw);
	// 	gf_draw_driver_after(draw);

	// 	RGFW_window_swapBuffers(draw->platform->window);
	// }
	return ret;
}

gf_draw_platform_t* gf_draw_platform_create(gf_engine_t* engine, gf_draw_t* draw) {
	// gf_draw_platform_t* platform = malloc(sizeof(*platform));
	// 	memset(platform, 0, sizeof(*platform));
	// 	platform->engine = engine;

	// 	platform->window = RGFW_createWindow(draw->title, RGFW_RECT(draw->x, draw->y, draw->width, draw->height), 0);
	// 	if(platform->window == NULL) {
	// 		gf_log_function(engine, "Failed to create window", "");
	// 		gf_draw_platform_destroy(platform);
	// 		return NULL;
	// 	}
	// 	if(engine->icon != NULL) {
	// 		RGFW_window_setIcon(platform->window, engine->icon, RGFW_AREA(engine->icon_width, engine->icon_height), 4);
	// 	}
	// 	platform->window->userPtr = draw;

	// 	RGFW_window_makeCurrent(platform->window);
	// #ifdef GF_DO_SWAP_INTERVAL
	// 	RGFW_window_swapInterval(platform->window, 1);
	// #endif
	// return platform;
	return NULL;
}

void gf_draw_platform_destroy(gf_draw_platform_t* platform) {
	// if(platform->window != NULL) {
	// 	RGFW_window_close(platform->window);
	// }
	// gf_log_function(platform->engine, "Destroyed platform-dependent part of drawing driver", "");
	// free(platform);
}
