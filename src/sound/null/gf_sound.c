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
#include <gf_thread.h>

/* Standard */
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <time.h>
#endif

gf_sound_t* gf_sound_create(gf_audio_t* audio, int rate) {
	int	    res;
	gf_sound_t* sound = malloc(sizeof(*sound));
	memset(sound, 0, sizeof(*sound));

	sound->audio	   = audio;
	sound->sample_rate = rate;

	gf_log_function(audio->engine, "Null available", "");

	return sound;
}

void gf_sound_null_thread(void* ptr) {
	gf_sound_t*    sound = (gf_sound_t*)ptr;
	unsigned char* data;
	int	       interval = 60;
	int	       len	= sound->sample_rate * 2 * 2 * interval / 1000;
#ifndef _WIN32
	struct timespec ts;
#endif

	data = malloc(len);
	while(1) {
		gf_audio_callback(sound->audio, data, len / 2 / 2);
#ifdef _WIN32
		Sleep(interval);
#else
		ts.tv_sec  = 0;
		ts.tv_nsec = 1000000 * interval;
		nanosleep(&ts, NULL);
#endif
		if(sound->context.quit) break;
	}
	free(data);
}

int gf_sound_start(gf_sound_t* sound) {
	sound->context.quit   = 0;
	sound->context.thread = gf_thread_create(gf_sound_null_thread, sound);
	return 0;
}

void gf_sound_destroy(gf_sound_t* sound) {
	sound->context.quit = 1;
	gf_thread_join(sound->context.thread);
	gf_thread_destroy(sound->context.thread);
	free(sound);
}
