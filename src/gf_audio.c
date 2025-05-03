#define GF_EXPOSE_AUDIO

#include <gf_pre.h>

/* External library */
#include <miniaudio.h>
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

void gf_audio_callback(ma_device* dev, void* output, const void* input, ma_uint32 frame) {
	ma_uint32   i;
	gf_audio_t* audio = dev->pUserData;
	ma_int16*   out	  = (ma_int16*)output;
	float*	    tmp	  = malloc(sizeof(*tmp) * frame * 2);
	double	    vol;

	for(i = 0; i < frame; i++) {
		tmp[2 * i + 0] = 0;
		tmp[2 * i + 1] = 0;
	}

	ma_mutex_lock(audio->mutex);
	for(i = 0; i < (ma_uint32)hmlen(audio->decoder); i++) {
		if(audio->decoder[i].used == 1 && audio->decoder[i].decoder != NULL) {
			ma_uint64 readframe;
			int	  j;
			ma_int16* r = malloc(sizeof(*r) * frame * 2);
			ma_decoder_read_pcm_frames(audio->decoder[i].decoder, r, frame, &readframe);
			for(j = 0; j < readframe; j++) {
				tmp[2 * j + 0] += (double)r[2 * j + 0] / 32768.0;
				tmp[2 * j + 1] += (double)r[2 * j + 1] / 32768.0;
			}
			free(r);
			if(frame > readframe) {
				audio->decoder[i].used = -2;
			}
		} else if(audio->decoder[i].used == 1 && audio->decoder[i].xm != NULL) {
			int    j;
			int    gotframe;
			float* r = malloc(sizeof(*r) * frame * 2);
			jar_xm_generate_samples(audio->decoder[i].xm, r, frame);
			gotframe = jar_xm_get_last(audio->decoder[i].xm) != -1 ? jar_xm_get_last(audio->decoder[i].xm) : (int)frame;
			for(j = 0; j < gotframe; j++) {
				tmp[2 * j + 0] += (double)r[2 * j + 0];
				tmp[2 * j + 1] += (double)r[2 * j + 1];
			}
			free(r);
			if(jar_xm_get_loop_count(audio->decoder[i].xm) > audio->decoder[i].internal) {
				audio->decoder[i].used = -2;
			}
		} else if(audio->decoder[i].used == 1 && audio->decoder[i].mod != NULL) {
			int	  j;
			int	  gotframe;
			ma_int16* r = malloc(sizeof(*r) * frame * 2);
			jar_mod_fillbuffer(audio->decoder[i].mod, r, frame, NULL);
			gotframe = audio->decoder[i].mod->last != -1 ? audio->decoder[i].mod->last : (int)frame;
			for(j = 0; j < gotframe; j++) {
				tmp[2 * j + 0] += (double)r[2 * j + 0] / 32768.0;
				tmp[2 * j + 1] += (double)r[2 * j + 1] / 32768.0;
			}
			free(r);
			if(audio->decoder[i].mod->loopcount > audio->decoder[i].internal) {
				audio->decoder[i].used = -2;
			}
		}
	}
	ma_mutex_unlock(audio->mutex);

	for(i = 0; i < frame; i++) {
		out[2 * i + 0] = tmp[2 * i + 0] * audio->volume * 32768;
		out[2 * i + 1] = tmp[2 * i + 1] * audio->volume * 32768;
	}
	free(tmp);
}

gf_audio_id_t gf_audio_load(gf_audio_t* audio, const void* data, size_t size) {
	gf_audio_decoder_t decoder;
	int		   xm_cond;
	int		   mod_cond;
	int		   ind;

	ma_mutex_lock(audio->mutex);

	decoder.used	       = 0;
	decoder.audio	       = audio;
	decoder.decoder	       = NULL;
	decoder.xm	       = NULL;
	decoder.mod	       = NULL;
	decoder.decoder_config = ma_decoder_config_init(audio->device_config.playback.format, audio->device_config.playback.channels, audio->device_config.sampleRate);
	decoder.key	       = 0;
	decoder.data	       = NULL;
	do {
		ind = hmgeti(audio->decoder, decoder.key);
		if(ind != -1) {
			decoder.key++;
		}
	} while(ind != -1);

	xm_cond	 = size > 37 && memcmp(data, "Extended Module: ", 17) == 0 && ((char*)data)[37] == 0x1a;
	mod_cond = size > 1080;

	if(mod_cond) {
		int j;
		int mod_sig_cond = 0;
		for(j = 0; j < sizeof(gf_audio_mod_sig) / sizeof(gf_audio_mod_sig[0]); j++) {
			mod_sig_cond = mod_sig_cond || (memcmp((char*)data + 1080, gf_audio_mod_sig[j], 4) == 0);
		}
		mod_cond = mod_cond && mod_sig_cond;
	}

	if(xm_cond) {
		if(jar_xm_create_context_safe(&decoder.xm, data, size, audio->device_config.sampleRate) == 0) {
			/* In XM loader .internal is used to store old loopcount */
			decoder.internal = jar_xm_get_loop_count(decoder.xm);
			decoder.used	 = -1;
			jar_xm_set_max_loop_count(decoder.xm, 1);
			hmputs(audio->decoder, decoder);
			ma_mutex_unlock(audio->mutex);
			return decoder.key;
		}
		decoder.xm = NULL;
	} else if(mod_cond) {
		decoder.mod = malloc(sizeof(*decoder.mod));
		jar_mod_init(decoder.mod);
		decoder.mod->modfile	 = malloc(size);
		decoder.mod->modfilesize = size;
		memcpy(decoder.mod->modfile, data, size);
		jar_mod_setcfg(decoder.mod, audio->device_config.sampleRate, 16, 1, 1, 0);
		if(jar_mod_load(decoder.mod, (void*)decoder.mod->modfile, size)) {
			/* In MOD loader .internal is used to store old loopcount */
			decoder.internal = decoder.mod->loopcount;
			decoder.used	 = -1;
			hmputs(audio->decoder, decoder);
			ma_mutex_unlock(audio->mutex);
			return decoder.key;
		}
		free(decoder.mod->modfile);
		free(decoder.mod);
		decoder.mod = NULL;
	}
	decoder.decoder = malloc(sizeof(*decoder.decoder));
	decoder.data	= malloc(size);
	decoder.size	= size;
	memcpy(decoder.data, data, size);
	if(ma_decoder_init_memory(decoder.data, decoder.size, &decoder.decoder_config, decoder.decoder) == MA_SUCCESS) {
		decoder.used = -1;
		hmputs(audio->decoder, decoder);
		ma_mutex_unlock(audio->mutex);
		return decoder.key;
	}
	free(decoder.data);
	free(decoder.decoder);
	decoder.decoder = NULL;
	ma_mutex_unlock(audio->mutex);
	return -1;
}

gf_audio_id_t gf_audio_load_file(gf_audio_t* audio, const char* path) {
	gf_file_t*     f = gf_file_open(audio->engine, path, "r");
	gf_audio_id_t  st;
	unsigned char* data;
	if(f == NULL) return -1;

	data = malloc(f->size);
	gf_file_read(f, data, f->size);
	st = gf_audio_load(audio, data, f->size);

	free(data);

	gf_file_close(f);

	return st;
}

gf_audio_t* gf_audio_create(gf_engine_t* engine) {
	gf_audio_t* audio = malloc(sizeof(*audio));

	memset(audio, 0, sizeof(*audio));
	audio->engine = engine;

	audio->volume = 1;

	audio->device_config		       = ma_device_config_init(ma_device_type_playback);
	audio->device_config.playback.format   = ma_format_s16;
	audio->device_config.playback.channels = 2;
	audio->device_config.sampleRate	       = 44100;
	audio->device_config.dataCallback      = gf_audio_callback;
	audio->device_config.pUserData	       = audio;
	audio->decoder			       = NULL;

	audio->device = malloc(sizeof(*audio->device));
	if(ma_device_init(NULL, &audio->device_config, audio->device) != MA_SUCCESS) {
		gf_log_function(engine, "Failed to open playback device", "");
		free(audio->device);
		audio->device = NULL;
		gf_audio_destroy(audio);
		return NULL;
	}

	audio->mutex = malloc(sizeof(*audio->mutex));
	if(ma_mutex_init(audio->mutex) != MA_SUCCESS) {
		gf_log_function(engine, "Failed to create mutex", "");
		free(audio->mutex);
		audio->mutex = NULL;
		gf_audio_destroy(audio);
		return NULL;
	}

	if(ma_device_start(audio->device) != MA_SUCCESS) {
		gf_log_function(engine, "Failed to start playback device", "");
		gf_audio_destroy(audio);
		return NULL;
	}

	gf_log_function(engine, "Audio interface started", "");

	return audio;
}

void gf_audio_decoder_destroy(gf_audio_decoder_t* decoder) {
	gf_audio_t* audio = decoder->audio;
	ma_mutex_lock(audio->mutex);
	if(decoder->decoder != NULL) {
		ma_decoder_uninit(decoder->decoder);
		free(decoder->decoder);
		decoder->decoder = NULL;
	}
	if(decoder->xm != NULL) {
		jar_xm_free_context(decoder->xm);
		decoder->xm = NULL;
	}
	if(decoder->mod != NULL) {
		jar_mod_unload(decoder->mod);
		free(decoder->mod);
		decoder->mod = NULL;
	}
	if(decoder->data != NULL) {
		free(decoder->data);
	}
	decoder->used = 0;
	hmdel(audio->decoder, decoder->key);
	ma_mutex_unlock(audio->mutex);
}

void gf_audio_destroy(gf_audio_t* audio) {
	if(audio->device != NULL) {
		ma_device_uninit(audio->device);
		free(audio->device);
	}
	if(audio->mutex != NULL) {
		ma_mutex_uninit(audio->mutex);
		free(audio->mutex);
	}
	if(audio->decoder != NULL) {
		while(hmlen(audio->decoder) > 0) {
			gf_audio_decoder_destroy(&audio->decoder[0]);
		}
		hmfree(audio->decoder);
	}
	gf_log_function(audio->engine, "Destroyed audio interface", "");
	free(audio);
}

void gf_audio_resume(gf_audio_t* audio, gf_audio_id_t id) {
	int ind;

	ma_mutex_lock(audio->mutex);
	ind = hmgeti(audio->decoder, id);
	if(ind == -1) {
		ma_mutex_unlock(audio->mutex);
		return;
	}

	audio->decoder[ind].used = 1;
	ma_mutex_unlock(audio->mutex);
}

void gf_audio_pause(gf_audio_t* audio, gf_audio_id_t id) {
	int ind;

	ma_mutex_lock(audio->mutex);
	ind = hmgeti(audio->decoder, id);
	if(ind == -1) {
		ma_mutex_unlock(audio->mutex);
		return;
	}

	audio->decoder[ind].used = -1;
	ma_mutex_unlock(audio->mutex);
}

void gf_audio_stop(gf_audio_t* audio, gf_audio_id_t id) {
	int ind = hmgeti(audio->decoder, id);
	if(ind == -1) return;

	gf_audio_decoder_destroy(&audio->decoder[ind]);
}

void gf_audio_set_volume(gf_audio_t* audio, double volume) { audio->volume = volume; }

int gf_audio_is_over(gf_audio_t* audio, gf_audio_id_t id) {
	int ind = hmgeti(audio->decoder, id);
	if(ind == -1) return 0;

	return audio->decoder[ind].used == -2 ? 1 : 0;
}
