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
 * Code for SDL2 audio playback.
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

void SDLCALL gf_sound_sdl2_callback(void* ptr, Uint8* stream, int len);

gf_sound_t* gf_sound_create(gf_audio_t* audio, int rate) {
	int	      res;
	SDL_AudioSpec spec;
	gf_sound_t*   sound = malloc(sizeof(*sound));
	memset(sound, 0, sizeof(*sound));

	sound->audio	   = audio;
	sound->sample_rate = rate;

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
		free(sound);
		return NULL;
	}

	spec.freq     = rate;
	spec.format   = AUDIO_S16;
	spec.channels = 2;
	spec.samples  = 2048;
	spec.callback = gf_sound_sdl2_callback;
	spec.userdata = sound;

	if((sound->context.device = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0)) == 0) {
		free(sound);
		return NULL;
	}

	gf_log_function(audio->engine, "SDL2 available", "");

	return sound;
}

void SDLCALL gf_sound_sdl2_callback(void* ptr, Uint8* stream, int len) {
	gf_sound_t* sound = (gf_sound_t*)ptr;
	gf_audio_callback(sound->audio, stream, len / 2 / 2);
}

int gf_sound_start(gf_sound_t* sound) {
	SDL_PauseAudioDevice(sound->context.device, 0);
	return 0;
}

void gf_sound_destroy(gf_sound_t* sound) {
	SDL_PauseAudioDevice(sound->context.device, 1);
	SDL_ClearQueuedAudio(sound->context.device);
	SDL_CloseAudioDevice(sound->context.device);
	free(sound);
}
