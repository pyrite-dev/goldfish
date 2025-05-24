/**
 * @file gf_snddrv.h
 * @~english
 * @brief Sound driver headers
 * @note User should not include this, this header is used internally
 */

#ifndef __GF_SNDDRV_H__
#define __GF_SNDDRV_H__

#if defined(SND_WINDOWS)
#include <windows.h>
#include <dsound.h>
#elif defined(SND_MINIAUDIO)
#include <miniaudio.h>
#elif defined(SND_ALSA)
#include <alsa/asoundlib.h>
#elif defined(SND_SDL2)
#include <SDL.h>
#include <SDL_audio.h>
#elif defined(SND_OSS)
#include <soundcard.h>
#elif defined(SND_SUN)
#include <sys/audioio.h>
#elif defined(SND_NULL)
#endif

#endif
