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

	return sound;
}

void gf_sound_alsa_thread(void* ptr) {
	gf_sound_t*    sound = (gf_sound_t*)ptr;
	int	       frame;
	unsigned char* data;

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
