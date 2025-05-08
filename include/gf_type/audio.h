/**
 * @file gf_type/audio.h
 * @~english
 * @brief Type definitions related to audio interface
 */

#ifndef __GF_TYPE_AUDIO_H__
#define __GF_TYPE_AUDIO_H__

#include <gf_pre.h>
#include <gf_macro.h>

typedef int gf_audio_id_t;

#ifdef GF_EXPOSE_AUDIO
typedef struct gf_audio_t	  gf_audio_t;
typedef struct gf_audio_decoder_t gf_audio_decoder_t;

/* External library */
#include <miniaudio.h>
#include <jar_xm.h>
#include <jar_mod.h>

/* Engine */
#include <gf_type/core.h>
#include <gf_type/thread.h>

/* Standard */

/**
 * @struct gf_audio_decoder_t
 * @~english
 * @brief Audio decoder
 *
 * @var gf_audio_decoder_t::audio
 * @brief Audio interface
 *
 * @var gf_audio_decoder_t::key
 * @brief Decoder ID
 *
 * @var gf_audio_decoder_t::decoder_config
 * @brief miniaudio decoder config
 *
 * @var gf_audio_decoder_t::decoder
 * @brief miniaudio decoder
 *
 * @var gf_audio_decoder_t::xm
 * @brief XM context
 *
 * @var gf_audio_decoder_t::mod
 * @brief MOD context
 *
 * @var gf_audio_decoder_t::internal
 * @brief Used internally
 *
 * @var gf_audio_decoder_t::used
 * @brief `1` if used, `-1` if used but paused, `-2` if used but drain, otherwise `0`
 *
 * @var gf_audio_decoder_t::data
 * @brief Copy of data
 *
 * @var gf_audio_decoder_t::size
 * @brief Data size
 */
GF_DECLARE_TYPE(audio_decoder, {
	gf_audio_id_t	   key;
	gf_audio_t*	   audio;
	ma_decoder_config  decoder_config;
	ma_decoder*	   decoder;
	jar_xm_context_t*  xm;
	jar_mod_context_t* mod;
	int		   internal;
	int		   used;
	unsigned char*	   data;
	size_t		   size;
});

/**
 * @struct gf_audio_t
 * @~english
 * @brief Audio interface
 *
 * @var gf_audio_t::engine
 * @brief Engine instance
 *
 * @var gf_audio_t::device_config
 * @brief miniaudio device config
 *
 * @var gf_audio_t::device
 * @brief miniaudio device
 *
 * @var gf_audio_t::decoder
 * @brief Decoder
 *
 * @var gf_audio_t::mutex
 * @brief Mutex
 *
 * @var gf_audio_t::volume
 * @brief Global volume
 */
GF_DECLARE_TYPE(audio, {
	gf_engine_t*	    engine;
	ma_device_config    device_config;
	ma_device*	    device;
	gf_audio_decoder_t* decoder;
	gf_thread_mutex_t*	    mutex;
	double volume;
});
#else
typedef void gf_audio_decoder_t;
typedef void gf_audio_t;
#endif

#endif
