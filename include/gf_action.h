/**
 * @file gf_action.h
 * @~english
 * @brief Action
 */

#ifndef __GF_ACTION_H__
#define __GF_ACTION_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/action.h>

/* Engine */
#include <gf_type/core.h>
#include <gf_type/input.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @~english
 * @brief Process key/mouse inputs
 * @param engine Engine instance
 * @param input Input interface
 */
GF_EXPORT void gf_action_process(gf_engine_t* engine, gf_input_t* input);

#ifdef __cplusplus
}
#endif

#endif
