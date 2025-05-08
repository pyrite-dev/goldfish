// #define GF_EXPOSE_AUDIO

#include <gf_pre.h>

/* External library */
// #include <miniaudio.h>
#include <jar_xm.h>
#include <jar_mod.h>
#include <stb_ds.h>

/* Interface */
#include <gf_audio.h>

/* Engine */
#include <gf_log.h>
#include <gf_file.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

const char* gf_audio_mod_sig[] = {"M!K!", "M.K.", "FLT4", "FLT8", "4CHN", "6CHN", "8CHN", "10CH", "12CH", "14CH", "16CH", "18CH", "20CH", "22CH", "24CH", "26CH", "28CH", "30CH", "32CH"};

gf_audio_id_t gf_audio_load(gf_audio_t* audio, const void* data, size_t size) {
	return -1;
}

gf_audio_id_t gf_audio_load_file(gf_audio_t* audio, const char* path) {
	return -1;
}

gf_audio_t* gf_audio_create(gf_engine_t* engine) {
	return NULL;
}

void gf_audio_decoder_destroy(gf_audio_decoder_t* decoder) {
}

void gf_audio_destroy(gf_audio_t* audio) {
}

void gf_audio_resume(gf_audio_t* audio, gf_audio_id_t id) {
}

void gf_audio_pause(gf_audio_t* audio, gf_audio_id_t id) {
}

void gf_audio_stop(gf_audio_t* audio, gf_audio_id_t id) {
}

void gf_audio_set_volume(gf_audio_t* audio, double volume) {}

int gf_audio_is_over(gf_audio_t* audio, gf_audio_id_t id) {
}
