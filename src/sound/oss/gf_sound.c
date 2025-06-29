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
 * Code for OSS audio playback.
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
#include <fcntl.h>
#include <unistd.h>

gf_sound_t* gf_sound_create(gf_audio_t* audio, int rate) {
	int	       res;
	gf_sound_t*    sound = malloc(sizeof(*sound));
	int	       v;
	unsigned short s;
	memset(sound, 0, sizeof(*sound));

	s = 1;

	sound->audio	   = audio;
	sound->sample_rate = rate;

#ifdef __NetBSD__
	sound->context.fd = open("/dev/audio", O_RDWR);
#else
	sound->context.fd = open("/dev/dsp", O_RDWR);
#endif
	if(sound->context.fd < 0) {
		free(sound);
		return NULL;
	}

	/* TODO: What does this even do? */
	if(ioctl(sound->context.fd, SNDCTL_DSP_RESET, 0) < 0) {
		close(sound->context.fd);
		free(sound);
		return NULL;
	}

	v = rate;
	if(ioctl(sound->context.fd, SNDCTL_DSP_SPEED, &v) < 0) {
		close(sound->context.fd);
		free(sound);
		return NULL;
	}

	v = 1;
	if(ioctl(sound->context.fd, SNDCTL_DSP_STEREO, &v) < 0) {
		close(sound->context.fd);
		free(sound);
		return NULL;
	}

	v = AFMT_S16_LE;
	if(0 == *(unsigned char*)&s) v = AFMT_S16_BE;
	if(ioctl(sound->context.fd, SNDCTL_DSP_SETFMT, &v) < 0) {
		close(sound->context.fd);
		free(sound);
		return NULL;
	}

	/* TODO: What does this even do? */
	v = 0;
	if(ioctl(sound->context.fd, SNDCTL_DSP_SETTRIGGER, &v) < 0) {
		close(sound->context.fd);
		free(sound);
		return NULL;
	}

	v = PCM_ENABLE_OUTPUT;
	if(ioctl(sound->context.fd, SNDCTL_DSP_SETTRIGGER, &v) < 0) {
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

	gf_log_function(audio->engine, "OSS available", "");

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
