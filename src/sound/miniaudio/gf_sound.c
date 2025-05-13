#define GF_EXPOSE_SOUND

#include <gf_pre.h>

/* External library */
#include <gf_snddrv.h>

/* Interface */
#include <gf_sound.h>

/* Engine */
#include <gf_audio.h>

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

	return sound;
}

int gf_sound_start(gf_sound_t* sound) { return ma_device_start(&sound->context.device) == MA_SUCCESS ? 0 : -1; }

void gf_sound_destroy(gf_sound_t* sound) {
	ma_device_stop(&sound->context.device);
	ma_device_uninit(&sound->context.device);
	free(sound);
}
