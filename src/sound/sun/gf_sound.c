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
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

gf_sound_t* gf_sound_create(gf_audio_t* audio, int rate) {
	int	    res;
	gf_sound_t* sound = malloc(sizeof(*sound));
	memset(sound, 0, sizeof(*sound));
	audio_info_t info;

	sound->audio	   = audio;
	sound->sample_rate = rate;

	sound->context.fd = open("/dev/audio", O_RDWR);
	if(sound->context.fd < 0) {
		free(sound);
		return NULL;
	}

	if(ioctl(sound->context.fd, AUDIO_GETINFO, &info) < 0) {
		close(sound->context.fd);
		free(sound);
		return NULL;
	}

	info.play.sample_rate = rate;
	info.play.channels    = 2;
	info.play.precision   = 16;
	info.play.encoding    = AUDIO_ENCODING_SLINEAR;
	if(ioctl(sound->context.fd, AUDIO_SETINFO, &info) < 0) {
		close(sound->context.fd);
		free(sound);
		return NULL;
	}

	return sound;
}

void gf_sound_oss_thread(void* ptr) {
	gf_sound_t*    sound = (gf_sound_t*)ptr;
	unsigned char* data;
	int	       interval = 60;
	int	       len	= sound->sample_rate * 2 * 2 * interval / 1000;

	data = malloc(len);

	while(1) {
		memset(data, 0, len);
		gf_audio_callback(sound->audio, data, len / 2 / 2);

		write(sound->context.fd, data, len);

		if(sound->context.quit) break;
	}

	free(data);
}

int gf_sound_start(gf_sound_t* sound) {
	sound->context.quit   = 0;
	sound->context.thread = gf_thread_create(gf_sound_oss_thread, sound);
	return 0;
}

void gf_sound_destroy(gf_sound_t* sound) {
	sound->context.quit = 1;
	gf_thread_join(sound->context.thread);
	gf_thread_destroy(sound->context.thread);
	if(sound->context.fd >= 0) {
		close(sound->context.fd);
	}
	free(sound);
}
