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
 * @endif
 */

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
#define gf_audio_decoder_t struct gf_audio_decoder
#define gf_audio_t struct gf_audio

gf_audio_decoder_t;
gf_audio_t;

/* External library */
#include <jar_xm.h>
#include <jar_mod.h>
#include <dr_mp3.h>
#include <dr_flac.h>
#include <dr_wav.h>

/* Engine */
#include <gf_type/core.h>
#include <gf_type/thread.h>
#include <gf_type/sound.h>

/* Standard */

/**
 * @struct gf_audio_decoder
 * @~english
 * @brief Audio decoder
 *
 * @var gf_audio_decoder::audio
 * @brief Audio interface
 *
 * @var gf_audio_decoder::key
 * @brief Decoder ID
 *
 * @var gf_audio_decoder::xm
 * @brief XM context
 *
 * @var gf_audio_decoder::mod
 * @brief MOD context
 *
 * @var gf_audio_decoder::mp3
 * @brief MP3 context
 *
 * @var gf_audio_decoder::flac
 * @brief FLAC context
 *
 * @var gf_audio_decoder::wav
 * @brief WAV context
 *
 * @var gf_audio_decoder::internal
 * @brief Used internally
 *
 * @var gf_audio_decoder::used
 * @brief `1` if used, `-1` if used but paused, `-2` if used but drain, otherwise `0`
 *
 * @var gf_audio_decoder::data
 * @brief Copy of data
 *
 * @var gf_audio_decoder::size
 * @brief Data size
 *
 * @var gf_audio_decoder::auto_destroy
 * @brief Auto-destroy
 */
GF_DECLARE_TYPE(audio_decoder, {
	gf_audio_id_t	   key;
	gf_audio_t*	   audio;
	jar_xm_context_t*  xm;
	jar_mod_context_t* mod;
	drmp3*		   mp3;
	drflac*		   flac;
	drwav*		   wav;
	int		   internal;
	int		   used;
	unsigned char*	   data;
	size_t		   size;
	int		   auto_destroy;
});

/**
 * @struct gf_audio
 * @~english
 * @brief Audio interface
 *
 * @var gf_audio::engine
 * @brief Engine instance
 *
 * @var gf_audio::device_config
 * @brief miniaudio device config
 *
 * @var gf_audio::device
 * @brief miniaudio device
 *
 * @var gf_audio::decoder
 * @brief Decoder
 *
 * @var gf_audio::mutex
 * @brief Mutex
 *
 * @var gf_audio::volume
 * @brief Global volume
 */
GF_DECLARE_TYPE(audio, {
	gf_engine_t*	    engine;
	gf_sound_t*	    device;
	gf_audio_decoder_t* decoder;
	gf_thread_mutex_t*  mutex;
	double		    volume;
});
#else
typedef void gf_audio_decoder_t;
typedef void gf_audio_t;
#endif

#endif
