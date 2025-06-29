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
 * TODO: Write here
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
#include <gf_log.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

void gf_sound_callback(ma_device* device, void* output, const void* input, ma_uint32 frame) {
	gf_sound_t* sound = device->pUserData;

	gf_audio_callback(sound->audio, output, frame);

	(void)input;
}

gf_sound_t* gf_sound_create(gf_audio_t* audio, int rate) {
	ma_device_config config;
	gf_sound_t*	 sound = malloc(sizeof(*sound));
	memset(sound, 0, sizeof(*sound));

	sound->audio	   = audio;
	sound->sample_rate = rate;

	config			 = ma_device_config_init(ma_device_type_playback);
	config.playback.format	 = ma_format_s16;
	config.playback.channels = 2;
	config.sampleRate	 = rate;
	config.dataCallback	 = gf_sound_callback;
	config.pUserData	 = sound;

	if(ma_device_init(NULL, &config, &sound->context.device) != MA_SUCCESS) {
		free(sound);
		return NULL;
	}

	gf_log_function(audio->engine, "miniaudio available", "");

	return sound;
}

int gf_sound_start(gf_sound_t* sound) { return ma_device_start(&sound->context.device) == MA_SUCCESS ? 0 : -1; }

void gf_sound_destroy(gf_sound_t* sound) {
	ma_device_stop(&sound->context.device);
	ma_device_uninit(&sound->context.device);
	free(sound);
}
