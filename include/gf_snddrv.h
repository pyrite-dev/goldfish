/**
 * @file gf_snddrv.h
 * @~english
 * @brief Sound driver headers
 * @note User should not include this, this header is used internally
 */

#ifndef __GF_SNDDRV_H__
#define __GF_SNDDRV_H__

#if defined(GF_SND_WINDOWS)
#include <windows.h>
#include <dsound.h>
#elif defined(GF_SND_WINMM)
#include <windows.h>
#elif defined(GF_SND_MINIAUDIO)
#include <miniaudio.h>
#elif defined(GF_SND_ALSA)
#include <alsa/asoundlib.h>
#elif defined(GF_SND_SDL2)
#include <SDL.h>
#include <SDL_audio.h>
#elif defined(GF_SND_OSS)
#include <soundcard.h>
#elif defined(GF_SND_SUN)
#include <sys/audioio.h>
#elif defined(GF_SND_NULL)
#endif

#endif
