/**
 * @file gf_sound.h
 * @~english
 * @brief Sound driver
 */

#ifndef __GF_SOUND_H__
#define __GF_SOUND_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */
#include <gf_type/sound.h>

/* Engine */
#include <gf_type/audio.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

GF_EXPORT gf_sound_t* gf_sound_create(gf_audio_t* audio, int rate);
GF_EXPORT int gf_sound_start(gf_sound_t* sound);
GF_EXPORT void gf_sound_destroy(gf_sound_t* sound);

#ifdef __cplusplus
}
#endif

#endif
