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
 * @endif
 */

/**
 * @file gf_type/sound.h
 * @~english
 * @brief Type definitions related to sound backend
 */

#ifndef __GF_TYPE_SOUND_H__
#define __GF_TYPE_SOUND_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_SOUND
#define gf_sound_context_t struct gf_sound_context
#define gf_sound_t struct gf_sound

gf_sound_context_t;
gf_sound_t;

/* External library */
#include <gf_snddrv.h>

/* Engine */
#include <gf_type/audio.h>
#include <gf_type/thread.h>

/* Standard */

#if defined(GF_SND_WINDOWS)
GF_DECLARE_TYPE(sound_context, {
	HANDLE dsound_dll;
	HRESULT(WINAPI * dsound_create)(void*, LPDIRECTSOUND*, void*);
	LPDIRECTSOUND dsound;

	HWAVEOUT     waveout;
	WAVEFORMATEX wavefmt;
	gf_thread_t* thread;
	int	     quit;
});
#elif defined(GF_SND_WINMM)
GF_DECLARE_TYPE(sound_context, {
	HWAVEOUT     waveout;
	WAVEFORMATEX wavefmt;
	gf_thread_t* thread;
	int	     quit;
});
#elif defined(GF_SND_MINIAUDIO)
GF_DECLARE_TYPE(sound_context, { ma_device device; });
#elif defined(GF_SND_ALSA)
GF_DECLARE_TYPE(sound_context, {
	snd_pcm_t*	     device;
	snd_pcm_hw_params_t* params;
	gf_thread_t*	     thread;
	int		     quit;
});
#elif defined(GF_SND_SDL2)
GF_DECLARE_TYPE(sound_context, { SDL_AudioDeviceID device; });
#elif defined(GF_SND_OSS) || defined(GF_SND_SUN)
GF_DECLARE_TYPE(sound_context, {
	int	     fd;
	gf_thread_t* thread;
	int	     quit;
});
#elif defined(GF_SND_NULL)
GF_DECLARE_TYPE(sound_context, {
	gf_thread_t* thread;
	int	     quit;
});
#else
/**
 * @struct gf_sound_context
 * @~english
 * @brief Platform-dependent sound driver context
 * @note Definition depends on the platform, read the header file for definition
 */
GF_DECLARE_TYPE(sound_context, {});
#endif

/**
 * @struct gf_sound
 * @~english
 * @brief Sound driver
 *
 * @var gf_sound::audio
 * @brief Audio interface
 *
 * @var gf_sound::sample_rate
 * @brief Sample rate
 *
 * @var gf_sound::context
 * @brief Platform-dependent sound driver context
 */
GF_DECLARE_TYPE(sound, {
	gf_audio_t*	   audio;
	int		   sample_rate;
	gf_sound_context_t context;
});

#else
typedef void gf_sound_t;
typedef void gf_sound_context_t;
#endif

#endif
