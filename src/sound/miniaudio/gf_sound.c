#define GF_EXPOSE_SOUND

#include <gf_pre.h>

/* External library */
#include <miniaudio.c>

/* Interface */
#include <gf_sound.h>

/* Engine */

/* Standard */

gf_sound_t* gf_sound_create(gf_audio_t* audio, int rate) {}

int gf_sound_start(gf_sound_t* sound) {}

void gf_sound_destroy(gf_sound_t* sound) {}
