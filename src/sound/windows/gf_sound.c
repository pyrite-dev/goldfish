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

	sound->context.dsound_dll = LoadLibrary("dsound.dll");
	do {
		if(sound->context.dsound_dll != NULL) {
			DSBUFFERDESC primary_descripton;
			memset(&primary_descripton, 0, sizeof(primary_descripton));

			primary_descripton.dwSize  = sizeof(primary_descripton);
			primary_descripton.dwFlags = DSBCAPS_PRIMARYBUFFER;

			sound->context.dsound_create = (void*)GetProcAddress(sound->context.dsound_dll, "DirectSoundCreate");

			if(sound->context.dsound_create(NULL, &sound->context.dsound, NULL) != DS_OK) {
				FreeLibrary(sound->context.dsound_dll);
				sound->context.dsound = NULL;
				break;
			}

			if(sound->context.dsound->lpVtbl->SetCooperativeLevel(sound->context.dsound, GetDesktopWindow(), DSSCL_PRIORITY) != DS_OK) {
				sound->context.dsound->lpVtbl->Release(sound->context.dsound);
				FreeLibrary(sound->context.dsound_dll);
				sound->context.dsound = NULL;
				break;
			}

			gf_log_function(audio->engine, "DirectSound available", "");

			return sound;
		}
	} while(0);

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
	int	       dw	= GetTickCount();
	while(1) {
		int dw2;
		hdr.lpData	    = data;
		hdr.dwBufferLength  = len;
		hdr.dwBytesRecorded = 0;
		hdr.dwUser	    = 0;
		hdr.dwFlags	    = 0;
		hdr.dwLoops	    = 0;

		memset(data, 0, len);
		gf_audio_callback(sound->audio, data, hdr.dwBufferLength / 2 / 2);

		waveOutPrepareHeader(sound->context.waveout, &hdr, sizeof(hdr));
		waveOutWrite(sound->context.waveout, &hdr, sizeof(hdr));
		waveOutUnprepareHeader(sound->context.waveout, &hdr, sizeof(hdr));
		dw2 = GetTickCount();
		if((dw2 - dw) < interval) {
			Sleep(interval - (dw2 - dw));
		}
		dw = GetTickCount();
		if(sound->context.quit) break;
	}

	free(data);
}

void gf_sound_dsound_thread(void* ptr) {
	gf_sound_t*	    sound    = (gf_sound_t*)ptr;
	int		    interval = 60;
	int		    len	     = sound->context.wavefmt.nAvgBytesPerSec * interval / 1000;
	LPDIRECTSOUNDBUFFER buffer   = NULL;
	DSBUFFERDESC	    buffer_description;
	gf_uint8_t*	    ptr1;
	gf_uint8_t*	    ptr2;
	DWORD		    len1;
	DWORD		    len2;
	gf_uint8_t*	    buf = malloc(len);
	int		    dw;

	memset(&buffer_description, 0, sizeof(buffer_description));
	buffer_description.dwSize	 = sizeof(buffer_description);
	buffer_description.dwBufferBytes = len;
	buffer_description.lpwfxFormat	 = &sound->context.wavefmt;
	buffer_description.dwFlags	 = DSBCAPS_GLOBALFOCUS | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_LOCSOFTWARE;
	if(sound->context.dsound->lpVtbl->CreateSoundBuffer(sound->context.dsound, &buffer_description, &buffer, NULL) != DS_OK) {
		/* TODO: Handle this properly */
		return;
	}

	buffer->lpVtbl->Lock(buffer, 0, len, (void**)&ptr1, &len1, (void**)&ptr2, &len2, DSBLOCK_ENTIREBUFFER);
	if(ptr1 != NULL) memset(ptr1, 0, len1);
	if(ptr2 != NULL) memset(ptr2, 0, len2);
	buffer->lpVtbl->Unlock(buffer, ptr1, len1, ptr2, len2);

	buffer->lpVtbl->Play(buffer, 0, 0, DSBPLAY_LOOPING);
	dw = GetTickCount();
	while(1) {
		int   n;
		int   i;
		DWORD cursor;
		DWORD dummy;
		int   dw2;

		buffer->lpVtbl->GetCurrentPosition(buffer, &dummy, &cursor);

		buffer->lpVtbl->Lock(buffer, cursor, len, (void**)&ptr1, &len1, (void**)&ptr2, &len2, 0);

		gf_audio_callback(sound->audio, buf, (len1 + len2) / 2 / 2);
		if(ptr1 != NULL) memcpy(ptr1, buf, len1);
		if(ptr2 != NULL) memcpy(ptr2, buf + len1, len2);

		buffer->lpVtbl->Unlock(buffer, ptr1, len1, ptr2, len2);

		dw2 = GetTickCount();
		if((dw2 - dw) < interval) {
			Sleep(interval - (dw2 - dw));
		}
		dw = GetTickCount();
		if(sound->context.quit) break;
	}
	free(buf);
	buffer->lpVtbl->Release(buffer);
}

int gf_sound_start(gf_sound_t* sound) {
	sound->context.quit = 0;
	if(sound->context.dsound != NULL) {
		sound->context.thread = gf_thread_create(gf_sound_dsound_thread, sound);
	} else {
		sound->context.thread = gf_thread_create(gf_sound_winmm_thread, sound);
	}
	return 0;
}

void gf_sound_destroy(gf_sound_t* sound) {
	sound->context.quit = 1;
	gf_thread_join(sound->context.thread);
	gf_thread_destroy(sound->context.thread);
	if(sound->context.dsound != NULL) {
		sound->context.dsound->lpVtbl->Release(sound->context.dsound);
		FreeLibrary(sound->context.dsound_dll);
	} else {
		waveOutPause(sound->context.waveout);
		waveOutReset(sound->context.waveout);
		waveOutClose(sound->context.waveout);
	}
	free(sound);
}
