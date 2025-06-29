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
 * File Name:    gf_sound.c
 * Author:       Nishi
 *
 * Functionality:
 * Code for ALSA audio playback.
 *
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_SOUND
#define GF_EXPOSE_AUDIO

#include <gf_pre.h>

/* External library */
#include <gf_snddrv.h>

/* Interface */
#include <gf_sound.h>

/* Engine */
#include <gf_audio.h>
#include <gf_thread.h>
#include <gf_log.h>
#include <gf_type/compat.h>

/* Standard */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

gf_sound_t* gf_sound_create(gf_audio_t* audio, int rate) {
	int	    res;
	gf_sound_t* sound = malloc(sizeof(*sound));
	memset(sound, 0, sizeof(*sound));

	sound->audio	   = audio;
	sound->sample_rate = rate;

	if(snd_pcm_open(&sound->context.device, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0) {
		gf_sound_destroy(sound);
		return NULL;
	}

	snd_pcm_hw_params_malloc(&sound->context.params);
	snd_pcm_hw_params_any(sound->context.device, sound->context.params);

	if(snd_pcm_hw_params_set_access(sound->context.device, sound->context.params, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
		gf_sound_destroy(sound);
		return NULL;
	}

	if(snd_pcm_hw_params_set_format(sound->context.device, sound->context.params, SND_PCM_FORMAT_S16) < 0) {
		gf_sound_destroy(sound);
		return NULL;
	}

	if(snd_pcm_hw_params_set_channels(sound->context.device, sound->context.params, 2) < 0) {
		gf_sound_destroy(sound);
		return NULL;
	}

	if(snd_pcm_hw_params_set_rate_near(sound->context.device, sound->context.params, &rate, 0) < 0) {
		gf_sound_destroy(sound);
		return NULL;
	}

	if(snd_pcm_hw_params_set_rate(sound->context.device, sound->context.params, rate, 0) < 0) {
		gf_sound_destroy(sound);
		return NULL;
	}

	if(snd_pcm_hw_params(sound->context.device, sound->context.params) < 0) {
		gf_sound_destroy(sound);
		return NULL;
	}

	gf_log_function(audio->engine, "ALSA available", "");

	return sound;
}

void gf_sound_alsa_thread(void* ptr) {
	gf_sound_t*	  sound = (gf_sound_t*)ptr;
	snd_pcm_uframes_t frame;
	unsigned char*	  data;

	snd_pcm_hw_params_get_period_size(sound->context.params, &frame, 0);

	data = malloc(frame * 2 * 2);

	while(1) {
		memset(data, 0, frame * 2 * 2);
		gf_audio_callback(sound->audio, data, frame);

		if(snd_pcm_writei(sound->context.device, data, frame) == -EPIPE) {
			snd_pcm_prepare(sound->context.device);
		}

		if(sound->context.quit) break;
	}

	free(data);
}

int gf_sound_start(gf_sound_t* sound) {
	sound->context.quit   = 0;
	sound->context.thread = gf_thread_create(gf_sound_alsa_thread, sound);
	return 0;
}

void gf_sound_destroy(gf_sound_t* sound) {
	sound->context.quit = 1;
	gf_thread_join(sound->context.thread);
	gf_thread_destroy(sound->context.thread);
	if(sound->context.device != NULL) {
		snd_pcm_drain(sound->context.device);
		snd_pcm_close(sound->context.device);
		snd_pcm_hw_params_free(sound->context.params);
	}
	free(sound);
}
