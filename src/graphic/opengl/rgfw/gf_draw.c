#define GF_EXPOSE_DRAW_PLATFORM
#define GF_EXPOSE_DRAW
#define GF_EXPOSE_CORE
#define GF_EXPOSE_INPUT

#include <gf_pre.h>

/* External library */
#define RGFW_IMPLEMENTATION
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
