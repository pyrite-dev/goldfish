/**
 * @file gf_type/input.h
 * @~english
 * @brief Type definitions related to input interface
 */

#ifndef __GF_TYPE_INPUT_H__
#define __GF_TYPE_INPUT_H__

#include <gf_pre.h>
#include <gf_macro.h>

/**
 * @~english
 * @brief Mask for left mouse button
 */
#define GF_INPUT_MOUSE_LEFT_MASK (1 << 0)

/**
 * @~english
 * @brief Mask for middle mouse button
 */
#define GF_INPUT_MOUSE_MIDDLE_MASK (1 << 1)

/**
 * @~english
 * @brief Mask for right mouse button
 */
#define GF_INPUT_MOUSE_RIGHT_MASK (1 << 2)

/**
 * @~english
 * @brief Key list
 */
enum GF_INPUT_KEYS {
	GF_INPUT_KEY_ESCAPE = 0,
	GF_INPUT_KEY_TAB,
	GF_INPUT_KEY_CAPSLOCK,

	GF_INPUT_KEY_BACKSPACE,
	GF_INPUT_KEY_ENTER,
	GF_INPUT_KEY_FN,

	GF_INPUT_KEY_UP,
	GF_INPUT_KEY_DOWN,
	GF_INPUT_KEY_LEFT,
	GF_INPUT_KEY_RIGHT,

	GF_INPUT_KEY_INSERT,
	GF_INPUT_KEY_DELETE,
	GF_INPUT_KEY_HOME,
	GF_INPUT_KEY_END,
	GF_INPUT_KEY_PAGE_UP,
	GF_INPUT_KEY_PAGE_DOWN,

	GF_INPUT_KEY_PRINT_SCREEN,
	GF_INPUT_KEY_SCROLL_LOCK,
	GF_INPUT_KEY_PAUSE_BREAK,

	GF_INPUT_KEY_SPACE,

	GF_INPUT_KEY_LEFT_SHIFT,
	GF_INPUT_KEY_RIGHT_SHIFT,

	GF_INPUT_KEY_LEFT_CONTROL,
	GF_INPUT_KEY_RIGHT_CONTROL,

	GF_INPUT_KEY_LEFT_ALT,
	GF_INPUT_KEY_RIGHT_ALT,

	GF_INPUT_KEY_LEFT_SUPER,
	GF_INPUT_KEY_RIGHT_SUPER,

	GF_INPUT_KEY_F1,
	GF_INPUT_KEY_F2,
	GF_INPUT_KEY_F3,
	GF_INPUT_KEY_F4,
	GF_INPUT_KEY_F5,
	GF_INPUT_KEY_F6,
	GF_INPUT_KEY_F7,
	GF_INPUT_KEY_F8,
	GF_INPUT_KEY_F9,
	GF_INPUT_KEY_F10,
	GF_INPUT_KEY_F11,
	GF_INPUT_KEY_F12,

	GF_INPUT_KEY_GRAVE,
	GF_INPUT_KEY_0,
	GF_INPUT_KEY_1,
	GF_INPUT_KEY_2,
	GF_INPUT_KEY_3,
	GF_INPUT_KEY_4,
	GF_INPUT_KEY_5,
	GF_INPUT_KEY_6,
	GF_INPUT_KEY_7,
	GF_INPUT_KEY_8,
	GF_INPUT_KEY_9,

	GF_INPUT_KEY_A,
	GF_INPUT_KEY_B,
	GF_INPUT_KEY_C,
	GF_INPUT_KEY_D,
	GF_INPUT_KEY_E,
	GF_INPUT_KEY_F,
	GF_INPUT_KEY_G,
	GF_INPUT_KEY_H,
	GF_INPUT_KEY_I,
	GF_INPUT_KEY_J,
	GF_INPUT_KEY_K,
	GF_INPUT_KEY_L,
	GF_INPUT_KEY_M,
	GF_INPUT_KEY_N,
	GF_INPUT_KEY_O,
	GF_INPUT_KEY_P,
	GF_INPUT_KEY_Q,
	GF_INPUT_KEY_R,
	GF_INPUT_KEY_S,
	GF_INPUT_KEY_T,
	GF_INPUT_KEY_U,
	GF_INPUT_KEY_V,
	GF_INPUT_KEY_W,
	GF_INPUT_KEY_X,
	GF_INPUT_KEY_Y,
	GF_INPUT_KEY_Z,

	GF_INPUT_KEY_MINUS,
	GF_INPUT_KEY_EQUALS,
	GF_INPUT_KEY_LEFT_BRACKET,
	GF_INPUT_KEY_RIGHT_BRACKET,
	GF_INPUT_KEY_BACKSLASH,
	GF_INPUT_KEY_SEMICOLON,
	GF_INPUT_KEY_QUOTE,
	GF_INPUT_KEY_COMMA,
	GF_INPUT_KEY_PERIOD,
	GF_INPUT_KEY_SLASH,

	GF_INPUT_KEY_NUM_LOCK,
	GF_INPUT_KEY_NUMPAD_SLASH,
	GF_INPUT_KEY_NUMPAD_ASTERISK,
	GF_INPUT_KEY_NUMPAD_MINUS,
	GF_INPUT_KEY_NUMPAD_PLUS,
	GF_INPUT_KEY_NUMPAD_ENTER,
	GF_INPUT_KEY_NUMPAD_PERIOD,
	GF_INPUT_KEY_NUMPAD_0,
	GF_INPUT_KEY_NUMPAD_1,
	GF_INPUT_KEY_NUMPAD_2,
	GF_INPUT_KEY_NUMPAD_3,
	GF_INPUT_KEY_NUMPAD_4,
	GF_INPUT_KEY_NUMPAD_5,
	GF_INPUT_KEY_NUMPAD_6,
	GF_INPUT_KEY_NUMPAD_7,
	GF_INPUT_KEY_NUMPAD_8,
	GF_INPUT_KEY_NUMPAD_9,
};

#define GF_INPUT_KEY_LAST GF_INPUT_KEY_NUMPAD_9

#ifdef GF_EXPOSE_INPUT
#define gf_input_t struct gf_input

gf_input_t;

/* External library */

/* Engine */
#include <gf_type/draw.h>
#include <gf_type/core.h>

/* Standard */

/**
 * @struct gf_input
 * @~english
 * @brief Input interface
 *
 * @var gf_input::engine
 * @brief Engine instance
 *
 * @var gf_input::mouse_x
 * @brief X coord of mouse
 *
 * @var gf_input::mouse_y
 * @brief Y coord of mouse
 *
 * @var gf_input::mouse_flag
 * @brief Mouse flag
 *
 * @var gf_input::key_queue
 * @brief Key queue
 *
 * @var gf_input::key_rqueue
 * @brief Released key queue
 *
 * @var gf_input::key_state
 * @brief Key state
 */
GF_DECLARE_TYPE(input, {
	gf_engine_t* engine;
	int	     mouse_x;
	int	     mouse_y;
	int	     mouse_flag;
	int*	     key_queue;
	int*	     key_rqueue;
	int*	     key_state;
	char*	     keymap[GF_INPUT_KEY_LAST + 1];
});
#else
typedef void gf_input_t;
#endif

#endif
