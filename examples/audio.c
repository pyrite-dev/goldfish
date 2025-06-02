#include <gf_core.h>
#include <gf_audio.h>

#include <stdio.h>

int run = 1;
gf_engine_t* engine;

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <signal.h>

void handler(int sig){
	run = 0;
}
#endif

int main(int argc, char** argv){
	if(argc != 2){
		fprintf(stderr, "Usage: %s path\n", argv[0]);
		return 1;
	}
	gf_engine_begin();
	engine = gf_engine_create_ex("unused", 1, "data", argv, argc);
	if(engine != NULL){
		gf_audio_t* audio = gf_audio_create(engine);
		if(audio != NULL){
			gf_audio_id_t id = gf_audio_load_file(audio, argv[1]);

			if(id != -1){
				gf_audio_resume(audio, id);
				while(run && !gf_audio_is_over(audio, id)){
#ifdef _WIN32
					Sleep(10);
#else
					usleep(10 * 1000);
#endif
				}
			}

			gf_audio_destroy(audio);
		}
		gf_engine_destroy(engine);
	}
	gf_engine_end();
}
