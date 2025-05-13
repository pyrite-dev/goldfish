#define GF_EXPOSE_SOUND

#include <gf_pre.h>

/* External library */
#include <gf_snddrv.h>

/* Interface */
#include <gf_sound.h>

/* Engine */
#include <gf_audio.h>
#include <gf_thread.h>
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
