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

/**
 * @~english
 * @brief Create sound driver
 * @brief Sample rate
 * @return Sound driver
 */
GF_EXPORT gf_sound_t* gf_sound_create(gf_audio_t* audio, int rate);

/**
 * @~english
 * @brief Start sound driver
 * @param sound sound Sound driver
 * @return `0` if successful, otherwise if failure
 */
GF_EXPORT int gf_sound_start(gf_sound_t* sound);

/**
 * @~english
 * @brief Destroy sound driver
 * @param sound sound Sound driver
 */
GF_EXPORT void gf_sound_destroy(gf_sound_t* sound);

#ifdef __cplusplus
}
#endif

#endif
