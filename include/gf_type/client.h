/**
 * @file gf_type/client.h
 * @~english
 * @brief Type definitions related to client interface
 */

#ifndef __GF_TYPE_CLIENT_H__
#define __GF_TYPE_CLIENT_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_CLIENT
typedef struct gf_client gf_client_t;

/* External library */

/* Engine */
#include <gf_type/core.h>
#include <gf_type/draw.h>
#include <gf_type/input.h>
#include <gf_type/audio.h>

/* Standard */

/**
 * @struct gf_client_t
 * @~english
 * @brief Client interface
 *
 * @var gf_client_t::engine
 * @brief Engine instance
 *
 * @var gf_client_t::draw
 * @brief Drawing interface
 *
 * @var gf_client_t::input
 * @brief Input interface
 *
 * @var gf_client_t::audio
 * @brief Audio interface
 */
GF_DECLARE_TYPE(client, {
	gf_engine_t* engine;
	gf_draw_t*   draw;
	gf_input_t*  input;
	gf_audio_t*  audio;
});
#else
typedef void gf_client_t;
#endif

#endif
