#define GF_EXPOSE_NETWORK

#include <gf_core.h>
#include <gf_network.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

gf_engine_t* engine;

int main(int argc, char** argv){
	if(argc != 2){
		fprintf(stderr, "Usage: %s host\n", argv[0]);
		return 1;
	}
	gf_engine_begin();
	engine = gf_engine_create_ex("unused", 1, "data", argv, argc);
	if(engine != NULL){
		gf_network_t* net = gf_network_tcp(engine, argv[1], 80);
		if(net != NULL){
			char* buffer = malloc(1);
			size_t sz = 0;
			buffer[0] = 0;
		
			char req[1024];
			req[0] = 0;
			strcat(req, "GET / HTTP/1.0\r\nHost: ");
			strcat(req, argv[1]);
			strcat(req, "\r\nConnection: close\r\n\r\n");
			gf_network_buffer_t* buf = gf_network_wbuffer(net, strlen(req));
			memcpy(buf->data, req, buf->size);

			while(gf_network_step(net) == 0){
				if(net->state == GF_NETWORK_STATE_AFTER_WRITE || net->state == GF_NETWORK_STATE_AFTER_READ){
					buf = gf_network_rbuffer(net, 512);
				}else if(net->state == GF_NETWORK_STATE_READ_COMPLETE || net->state == GF_NETWORK_STATE_READ_PART){
					char* old = buffer;
					buffer = malloc(sz + buf->size + 1);
					memcpy(buffer, old, sz);
					memcpy(buffer + sz, buf->data, buf->size);
					buffer[sz + buf->size] = 0;
					sz += buf->size;
					free(old);
				}
			}

			printf("%s\n", buffer);
			free(buffer);
		}
		gf_engine_destroy(engine);
	}
	gf_engine_end();
}
