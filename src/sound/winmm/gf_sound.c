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

	sound->context.wavefmt.wFormatTag      = WAVE_FORMAT_PCM;
	sound->context.wavefmt.nChannels       = 2;
	sound->context.wavefmt.nSamplesPerSec  = rate;
	sound->context.wavefmt.wBitsPerSample  = 16;
	sound->context.wavefmt.nBlockAlign     = sound->context.wavefmt.nChannels * sound->context.wavefmt.wBitsPerSample / 8;
	sound->context.wavefmt.nAvgBytesPerSec = sound->context.wavefmt.nSamplesPerSec * sound->context.wavefmt.nBlockAlign;

	res = waveOutOpen(&sound->context.waveout, WAVE_MAPPER, &sound->context.wavefmt, 0, 0, WAVE_ALLOWSYNC);
	if(res != MMSYSERR_NOERROR) {
		free(sound);
		return NULL;
	}

	gf_log_function(audio->engine, "WinMM available", "");
	return sound;
}

void gf_sound_winmm_thread(void* ptr) {
	gf_sound_t*    sound = (gf_sound_t*)ptr;
	WAVEHDR	       hdr;
	int	       interval = 60;
	int	       len	= sound->context.wavefmt.nAvgBytesPerSec * interval / 1000;
	unsigned char* data	= malloc(len);
	int	       diff	= 0;
	while(1) {
		int dw = GetTickCount();
		int dw2;
		if(diff >= len) {
			Sleep(interval);
			diff -= len;
			continue;
		}
		hdr.lpData	    = data;
		hdr.dwBufferLength  = len - diff;
		hdr.dwBytesRecorded = 0;
		hdr.dwUser	    = 0;
		hdr.dwFlags	    = 0;
		hdr.dwLoops	    = 0;

		memset(data, 0, len);
		gf_audio_callback(sound->audio, data, hdr.dwBufferLength / 2 / 2);

		waveOutPrepareHeader(sound->context.waveout, &hdr, sizeof(hdr));
		waveOutWrite(sound->context.waveout, &hdr, sizeof(hdr));
		waveOutUnprepareHeader(sound->context.waveout, &hdr, sizeof(hdr));
		diff = 0;
		dw2  = GetTickCount();
		if((dw2 - dw) < interval) {
			Sleep(interval - (dw2 - dw));
		} else {
			diff = ((dw2 - dw) - interval) * sound->sample_rate / 1000;
		}
		diff *= 4;
		if(sound->context.quit) break;
	}

	free(data);
}

int gf_sound_start(gf_sound_t* sound) {
	sound->context.quit   = 0;
	sound->context.thread = gf_thread_create(gf_sound_winmm_thread, sound);
	return 0;
}

void gf_sound_destroy(gf_sound_t* sound) {
	sound->context.quit = 1;
	gf_thread_join(sound->context.thread);
	gf_thread_destroy(sound->context.thread);

	waveOutPause(sound->context.waveout);
	waveOutReset(sound->context.waveout);
	waveOutClose(sound->context.waveout);

	free(sound);
}
