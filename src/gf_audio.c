#define GF_EXPOSE_AUDIO
#define GF_EXPOSE_SOUND

#include <gf_pre.h>

/* External library */
#include <jar_xm.h>
#include <jar_mod.h>
#include <dr_mp3.h>
#include <dr_flac.h>
#include <dr_wav.h>
#include <stb_ds.h>

/* Interface */
#include <gf_audio.h>

/* Engine */
#include <gf_sound.h>
#include <gf_log.h>
#include <gf_file.h>
#include <gf_thread.h>
#include <gf_type/compat.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

const char* gf_audio_mod_sig[] = {"M!K!", "M.K.", "FLT4", "FLT8", "4CHN", "6CHN", "8CHN", "10CH", "12CH", "14CH", "16CH", "18CH", "20CH", "22CH", "24CH", "26CH", "28CH", "30CH", "32CH"};

void gf_audio_callback(gf_audio_t* audio, void* output, int frame) {
	int	    i;
	gf_int16_t* out = (gf_int16_t*)output;
	float*	    tmp = malloc(sizeof(*tmp) * frame * 2);

	for(i = 0; i < frame; i++) {
		tmp[2 * i + 0] = 0;
		tmp[2 * i + 1] = 0;
	}

	gf_thread_mutex_lock(audio->mutex);
	for(i = 0; i < hmlen(audio->decoder); i++) {
		if(audio->decoder[i].used == 1 && (audio->decoder[i].mp3 != NULL || audio->decoder[i].flac != NULL || audio->decoder[i].wav != NULL)) {
			int    j;
			int    gotframe;
			int    ch = 0;
			double sr = (double)audio->device->sample_rate;
			float* r;
			int    want;
			void*  ptr;
			drwav_uint64 (*read_frame)(void*, drwav_uint64, void*);

			if(audio->decoder[i].mp3 != NULL) {
				read_frame = (void*)drmp3_read_pcm_frames_f32;
				ptr	   = audio->decoder[i].mp3;
				ch	   = audio->decoder[i].mp3->channels;
				sr /= audio->decoder[i].mp3->sampleRate;
			} else if(audio->decoder[i].flac != NULL) {
				read_frame = (void*)drflac_read_pcm_frames_f32;
				ptr	   = audio->decoder[i].flac;
				ch	   = audio->decoder[i].flac->channels;
				sr /= audio->decoder[i].flac->sampleRate;
			} else if(audio->decoder[i].wav != NULL) {
				read_frame = (void*)drwav_read_pcm_frames_f32;
				ptr	   = audio->decoder[i].wav;
				ch	   = audio->decoder[i].wav->channels;
				sr /= audio->decoder[i].wav->sampleRate;
			}

			want	 = frame / sr;
			r	 = malloc(sizeof(*r) * want * ch);
			gotframe = read_frame(ptr, want, r);
			for(j = 0; j < gotframe * sr; j++) {
				int ind = j / sr;
				if(ch == 1) {
					tmp[2 * j + 0] += (double)r[ind + 0];
					tmp[2 * j + 1] += (double)r[ind + 0];
				} else {
					tmp[2 * j + 0] += (double)r[ch * ind + 0];
					tmp[2 * j + 1] += (double)r[ch * ind + 1];
				}
			}
			free(r);
			if(gotframe < want) {
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
			int	    j;
			int	    gotframe;
			gf_int16_t* r = malloc(sizeof(*r) * frame * 2);
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
	gf_thread_mutex_unlock(audio->mutex);

	for(i = 0; i < hmlen(audio->decoder); i++) {
		if(audio->decoder[i].auto_destroy && audio->decoder[i].used == -2) {
			gf_audio_decoder_destroy(&audio->decoder[i]);
			i = 0;
		}
	}

	for(i = 0; i < frame; i++) {
		out[2 * i + 0] = tmp[2 * i + 0] * audio->volume * 32767;
		out[2 * i + 1] = tmp[2 * i + 1] * audio->volume * 32767;
	}
	free(tmp);
}

gf_audio_id_t gf_audio_load(gf_audio_t* audio, const void* data, size_t size) {
	gf_audio_decoder_t decoder;
	int		   xm_cond;
	int		   mod_cond;
	int		   ind;

	gf_thread_mutex_lock(audio->mutex);

	decoder.used	     = 0;
	decoder.audio	     = audio;
	decoder.xm	     = NULL;
	decoder.mod	     = NULL;
	decoder.mp3	     = NULL;
	decoder.flac	     = NULL;
	decoder.wav	     = NULL;
	decoder.key	     = 0;
	decoder.data	     = NULL;
	decoder.auto_destroy = 0;
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
		if(jar_xm_create_context_safe(&decoder.xm, data, size, audio->device->sample_rate) == 0) {
			/* In XM loader .internal is used to store old loopcount */
			decoder.internal = jar_xm_get_loop_count(decoder.xm);
			decoder.used	 = -1;
			jar_xm_set_max_loop_count(decoder.xm, 1);
			hmputs(audio->decoder, decoder);
			gf_thread_mutex_unlock(audio->mutex);
			return decoder.key;
		}
		decoder.xm = NULL;
	} else if(mod_cond) {
		decoder.mod = malloc(sizeof(*decoder.mod));
		jar_mod_init(decoder.mod);
		decoder.mod->modfile	 = malloc(size);
		decoder.mod->modfilesize = size;
		memcpy(decoder.mod->modfile, data, size);
		jar_mod_setcfg(decoder.mod, audio->device->sample_rate, 16, 1, 1, 0);
		if(jar_mod_load(decoder.mod, (void*)decoder.mod->modfile, size)) {
			/* In MOD loader .internal is used to store old loopcount */
			decoder.internal = decoder.mod->loopcount;
			decoder.used	 = -1;
			hmputs(audio->decoder, decoder);
			gf_thread_mutex_unlock(audio->mutex);
			return decoder.key;
		}
		free(decoder.mod->modfile);
		free(decoder.mod);
		decoder.mod = NULL;
	}

	decoder.data = malloc(size);
	decoder.size = size;
	memcpy(decoder.data, data, size);

	decoder.mp3 = malloc(sizeof(*decoder.mp3));
	if((decoder.size > 2 && decoder.data[0] == 0xff && (decoder.data[1] == 0xfb || decoder.data[1] == 0xf3 || decoder.data[1] == 0xf2) || (decoder.size > 3 && memcmp(decoder.data, "ID3", 3) == 0)) && drmp3_init_memory(decoder.mp3, decoder.data, decoder.size, NULL)) {
		decoder.used = -1;
		hmputs(audio->decoder, decoder);
		gf_thread_mutex_unlock(audio->mutex);
		return decoder.key;
	}
	free(decoder.mp3);
	decoder.mp3 = NULL;

	if((decoder.size > 4 && memcmp(decoder.data, "fLaC", 4) == 0) && (decoder.flac = drflac_open_memory(decoder.data, decoder.size, NULL)) != NULL) {
		decoder.used = -1;
		hmputs(audio->decoder, decoder);
		gf_thread_mutex_unlock(audio->mutex);
		return decoder.key;
	}
	decoder.flac = NULL;

	decoder.wav = malloc(sizeof(*decoder.wav));
	if(drwav_init_memory(decoder.wav, decoder.data, decoder.size, NULL)) {
		decoder.used = -1;
		hmputs(audio->decoder, decoder);
		gf_thread_mutex_unlock(audio->mutex);
		return decoder.key;
	}
	free(decoder.wav);
	decoder.wav = NULL;

	free(decoder.data);
	gf_thread_mutex_unlock(audio->mutex);
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

	audio->decoder = NULL;

	if((audio->device = gf_sound_create(audio, 44100)) == NULL) {
		gf_log_function(engine, "Failed to open playback device", "");
		free(audio->device);
		audio->device = NULL;
		gf_audio_destroy(audio);
		return NULL;
	}

	if((audio->mutex = gf_thread_mutex_create()) == NULL) {
		gf_log_function(engine, "Failed to create mutex", "");
		audio->mutex = NULL;
		gf_audio_destroy(audio);
		return NULL;
	}

	if(gf_sound_start(audio->device) != 0) {
		gf_log_function(engine, "Failed to start playback device", "");
		gf_audio_destroy(audio);
		return NULL;
	}

	gf_log_function(engine, "Audio interface started", "");

	return audio;
}

void gf_audio_decoder_destroy(gf_audio_decoder_t* decoder) {
	gf_audio_t* audio = decoder->audio;
	gf_thread_mutex_lock(audio->mutex);
	if(decoder->mp3 != NULL) {
		drmp3_uninit(decoder->mp3);
		free(decoder->mp3);
		decoder->mp3 = NULL;
	}
	if(decoder->flac != NULL) {
		drflac_close(decoder->flac);
		decoder->flac = NULL;
	}
	if(decoder->wav != NULL) {
		drwav_uninit(decoder->wav);
		free(decoder->wav);
		decoder->wav = NULL;
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
	gf_thread_mutex_unlock(audio->mutex);
}

void gf_audio_destroy(gf_audio_t* audio) {
	if(audio->device != NULL) {
		gf_sound_destroy(audio->device);
	}
	if(audio->decoder != NULL) {
		while(hmlen(audio->decoder) > 0) {
			gf_audio_decoder_destroy(&audio->decoder[0]);
		}
		hmfree(audio->decoder);
	}
	if(audio->mutex != NULL) {
		gf_thread_mutex_destroy(audio->mutex);
	}
	gf_log_function(audio->engine, "Destroyed audio interface", "");
	free(audio);
}

void gf_audio_resume(gf_audio_t* audio, gf_audio_id_t id) {
	int ind;

	gf_thread_mutex_lock(audio->mutex);
	ind = hmgeti(audio->decoder, id);
	if(ind == -1) {
		gf_thread_mutex_unlock(audio->mutex);
		return;
	}

	audio->decoder[ind].used = 1;
	gf_thread_mutex_unlock(audio->mutex);
}

void gf_audio_pause(gf_audio_t* audio, gf_audio_id_t id) {
	int ind;

	gf_thread_mutex_lock(audio->mutex);
	ind = hmgeti(audio->decoder, id);
	if(ind == -1) {
		gf_thread_mutex_unlock(audio->mutex);
		return;
	}

	audio->decoder[ind].used = -1;
	gf_thread_mutex_unlock(audio->mutex);
}

void gf_audio_stop(gf_audio_t* audio, gf_audio_id_t id) {
	int ind = hmgeti(audio->decoder, id);
	if(ind == -1) return;

	gf_audio_decoder_destroy(&audio->decoder[ind]);
}

void gf_audio_auto_destroy(gf_audio_t* audio, gf_audio_id_t id) {
	int ind = hmgeti(audio->decoder, id);
	if(ind == -1) return;

	audio->decoder[ind].auto_destroy = 1;
}

void gf_audio_set_volume(gf_audio_t* audio, double volume) { audio->volume = volume; }

double gf_audio_get_volume(gf_audio_t* audio) { return audio->volume; }

int gf_audio_is_over(gf_audio_t* audio, gf_audio_id_t id) {
	int ind = hmgeti(audio->decoder, id);
	if(ind == -1) return 0;

	return audio->decoder[ind].used == -2 ? 1 : 0;
}
