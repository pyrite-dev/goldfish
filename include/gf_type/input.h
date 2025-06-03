/**
 * @file gf_type/input.h
 * @~english
 * @brief Type definitions related to input interface
 */

#ifndef __GF_TYPE_INPUT_H__
#define __GF_TYPE_INPUT_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_INPUT
#define gf_input_t struct gf_input

gf_input_t;

/* External library */

/* Engine */
#include <gf_type/draw.h>
#include <gf_type/core.h>

/* Standard */

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
 * @struct gf_input_t
 * @~english
 * @brief Input interface
 *
 * @var gf_input_t::engine
 * @brief Engine instance
 *
 * @var gf_input_t::mouse_x
 * @brief X coord of mouse
 *
 * @var gf_input_t::mouse_y
 * @brief Y coord of mouse
 *
 * @var gf_input_t::mouse_flag
 * @brief Mouse flag
 */
GF_DECLARE_TYPE(input, {
	gf_engine_t* engine;
	int	     mouse_x;
	int	     mouse_y;
	int	     mouse_flag;
});
#else
typedef void gf_input_t;
#endif

#endif
