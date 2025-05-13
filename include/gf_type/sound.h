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
typedef struct gf_sound_t	  gf_sound_t;
typedef struct gf_sound_context_t gf_sound_context_t;

/* External library */
#include <gf_snddrv.h>

/* Engine */
#include <gf_type/audio.h>
#include <gf_type/thread.h>

/* Standard */

#if defined(SND_WINDOWS)
GF_DECLARE_TYPE(sound_context, {
	HANDLE dsound_dll;
	HRESULT(*dsound_create)(void*, LPDIRECTSOUND*, void*);
	LPDIRECTSOUND dsound;

	HWAVEOUT waveout;
	WAVEFORMATEX wavefmt;
	gf_thread_t* thread;
	int quit;
});
#elif defined(SND_MINIAUDIO)
GF_DECLARE_TYPE(sound_context, {
	ma_device device;
});
#elif defined(SND_ALSA)
GF_DECLARE_TYPE(sound_context, {
	snd_pcm_t* device;
	snd_pcm_hw_params_t* params;
	gf_thread_t* thread;
	int quit;
});
#elif defined(SND_SDL2)
GF_DECLARE_TYPE(sound_context, {
	SDL_AudioDeviceID device;
});
#else
/**
 * @struct gf_sound_context_t
 * @~english
 * @brief Platform-dependent sound driver context
 * @note Definition depends on the platform, read the header file for definition
 */
GF_DECLARE_TYPE(sound_context, {
});
#endif

/**
 * @struct gf_sound_t
 * @~english
 * @brief Sound driver
 *
 * @var gf_sound_t::audio
 * @brief Audio interface
 *
 * @var gf_sound_t::sample_rate
 * @brief Sample rate
 *
 * @var gf_sound_t::context
 * @brief Platform-dependent sound driver context
 */
GF_DECLARE_TYPE(sound, {
	gf_audio_t* audio;
	int sample_rate;
	gf_sound_context_t context;
});

#else
typedef void gf_sound_t;
typedef void gf_sound_context_t;
#endif

#endif
