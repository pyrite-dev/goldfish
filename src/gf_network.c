#define GF_EXPOSE_NETWORK

#include <gf_pre.h>

/* External library */
#include <gf_netdrv.h>
#include <minisocket.h>
#include <stb_ds.h>

/* Interface */
#include <gf_network.h>

/* Engine */
#include <gf_type/compat.h>
#include <gf_log.h>
#include <gf_util.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

gf_uint32_t gf_network_id(const char* str) {
	gf_uint32_t id = *(gf_uint32_t*)&str[0];
	return htonl(id);
}

static void gf_network_key(gf_network_t* net) {
	gf_uint64_t r;
	int	    count = 0;
	int	    i;
	for(i = 0; i < sizeof(net->random_seed); i++) {
		if(count == 0) {
			count = sizeof(r);
		}

		net->random_seed[i] = r & 0xff;

		count--;
		r = r >> 8;
	}
}

static gf_network_t* gf_network_secure(gf_engine_t* engine, ms_interface_t* net) {
	int	      len    = strlen("ClientHello") + 1 + 1 + 1 + 64 + 2;
	int	      first  = 1;
	char**	      list   = NULL;
	char*	      buffer = malloc(1);
	int	      brk    = 0;
	ms_buffer_t*  buf    = ms_wbuffer(net, len);
	gf_network_t* r	     = malloc(sizeof(*r));
	memset(r, 0, sizeof(*r));
	r->engine = engine;
	r->net	  = net;

	gf_network_key(r);

	buffer[0] = 0;

	gf_log_function(engine, "GFSL handshaking", "");

	gf_log_function(engine, "ClientHello", "");
	memcpy(buf->data, "ClientHello 1 abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd\r\n", len);
	while(1) {
		int st = ms_step(net);
		if(st != 0 || net->state >= MS_STATE_FAILED) {
			brk = 1;
		} else if((net->state == MS_STATE_AFTER_WRITE && first) || net->state == MS_STATE_AFTER_READ) {
			first = 0;
			buf   = ms_rbuffer(net, 1);
		} else if(net->state == MS_STATE_READ_COMPLETE) {
			unsigned char c = *(unsigned char*)buf->data;
			if(c == '\n') {
				int i;
				int incr = 0;
				int len	 = strlen(buffer);
				for(i = 0; i <= len; i++) {
					if(buffer[i] == ' ' || i == len) {
						char* arg = buffer + incr;
						buffer[i] = 0;
						arrput(list, arg);
						incr = i + 1;
						if(i == len) break;
					}
				}

				if(list != NULL && arrlen(list) > 0) {
					if(strcmp(list[0], "ServerHello") == 0 && arrlen(list) == 3 && strcmp(list[1], "1") == 0 && strlen(list[2]) == 64) {
					} else {
						brk = 1;
					}
					arrfree(list);
					list = NULL;
				} else {
					brk = 1;
				}
				free(buffer);
				buffer	  = malloc(1);
				buffer[0] = 0;
			} else if(c != '\r') {
				char* old = buffer;
				buffer	  = malloc(strlen(old) + 2);
				strcpy(buffer, old);
				buffer[strlen(old)]	= c;
				buffer[strlen(old) + 1] = 0;
				free(old);
			}
		}
		if(brk) break;
	}
	free(buffer);
	if(list != NULL) arrfree(list);
	if(brk) {
		gf_log_function(engine, "Handshake failed", "");
		ms_destroy(net);
		free(r);
		r = NULL;
	}

	return r;
}

gf_network_t* gf_network_secure_tcp(gf_engine_t* engine, const char* host, int port) {
	ms_interface_t* net;
	gf_log_function(engine, "Connecting to %s:%d", host, port);

	net = ms_tcp(host, port);
	if(net == NULL) {
		gf_log_function(engine, "Connection failure", "");
		return NULL;
	}

	while(1) {
		int st = ms_step(net);
		if(net->state == MS_STATE_CONNECTED) break;
		if(net->state >= MS_STATE_FAILED || st != 0) {
			gf_log_function(engine, "Connection failure", "");
			ms_destroy(net);
			return NULL;
		}
	}

	gf_log_function(engine, "Connected", "");

	return gf_network_secure(engine, net);
}
