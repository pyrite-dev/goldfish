/**
 * @file gf_input.h
 * @~english
 * @brief Input
 */

#ifndef __GF_INPUT_H__
#define __GF_INPUT_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/input.h>

/* Engine */
#include <gf_type/core.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Create input interface
 * @param engine Engine instance
 * @return Input interface
 */
GF_EXPORT gf_input_t* gf_input_create(gf_engine_t* engine);

/**
 * @~english
 * @brief Destroy input interface
 * @param input Input interface
 */
GF_EXPORT void gf_input_destroy(gf_input_t* input);

/**
 * @~english
 * @brief Handle key press event
 * @param input Input interface
 * @param key Key
 */
GF_EXPORT void gf_input_key_press(gf_input_t* input, int key);

/**
 * @~english
 * @brief Handle key release event
 * @param input Input interface
 * @param key Key
 */
GF_EXPORT void gf_input_key_release(gf_input_t* input, int key);

/**
 * @~english
 * @brief Check if key is down
 * @param input Input interface
 * @param key Key
 * @return `1` if key is down, `0` otherwise
 */
GF_EXPORT int gf_input_key_down(gf_input_t* input, int key);

#ifdef __cplusplus
}
#endif

#endif
