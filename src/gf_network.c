/**
 * @if SHOW_LICENSE
 **********************************************************************************
 *
 * Copyright (c) 2025 Pyrite Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. All advertising materials mentioning features or use of this software must
 *    display the following acknowledgement:
 *    This product includes software developed by the Pyrite Development Team.
 *
 * 4. Neither the name of Pyrite Development Team nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 **********************************************************************************
 * File Name:    gf_network.c
 * Author:       Nishi
 *
 * Functionality:
 * Handle GoldFish Secure Layer.
 *
 **********************************************************************************
 * @endif
 */

#define GF_EXPOSE_NETWORK

#include <gf_pre.h>

/* External library */
#include <gf_netdrv.h>
#include <minisocket.h>
#include <stb_ds.h>
#include <compact25519.h>
#include <aes.h>

/* Interface */
#include <gf_network.h>

/* Engine */
#include <gf_type/compat.h>
#include <gf_log.h>
#include <gf_util.h>

/* Standard */
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
			r     = gf_util_random(net->engine);
		}

		net->random_seed[i] = r & 0xff;

		count--;
		r = r >> 8;
	}
	compact_x25519_keygen(net->private_key, net->public_key, net->random_seed);
}

static void gf_network_key_hex(char* out, gf_uint8_t key[X25519_KEY_SIZE]) {
	int	    i;
	const char* hex = "0123456789ABCDEF";
	for(i = 0; i < X25519_KEY_SIZE; i++) {
		out[2 * i + 0] = hex[(key[i] >> 4) & 0xf];
		out[2 * i + 1] = hex[(key[i] >> 0) & 0xf];
	}
}

static int gf_network_hex(char c) {
	if('0' <= c && c <= '9') {
		return c - '0';
	} else if('A' <= c && c <= 'F') {
		return c - 'A' + 10;
	} else if('a' <= c && c <= 'f') {
		return c - 'a' + 10;
	}
	return 0;
}

static void gf_network_close(int fd) {
#ifdef _WIN32
	closesocket(fd);
#else
	close(fd);
#endif
}

static int gf_network_socket(gf_engine_t* engine, const char* type, const char* host, int port) {
	int		   fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in addr;

	gf_log_function(engine, "Binding to %s:%d", host, port);

#ifdef _WIN32
	if(fd == INVALID_SOCKET) fd = -1;
#endif
	if(fd == -1) {
		gf_log_function(engine, "Failed to create socket", "");
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family	     = AF_INET;
	addr.sin_port	     = htons(port);
	addr.sin_addr.s_addr = inet_addr(host);
	if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		gf_log_function(engine, "Failed to bind", "");
		gf_network_close(fd);
		return -1;
	}
	if(listen(fd, 64) == -1) {
		gf_log_function(engine, "Failed to listen", "");
		gf_network_close(fd);
		return -1;
	}
	return fd;
}

typedef struct state {
	int	     len;
	int	     first;
	char**	     list;
	char*	     buffer;
	int	     brk;
	ms_buffer_t* buf;
	time_t	     began_at;
	int	     good;
	int	     server;
} state_t;

static gf_network_t* gf_network_secure(gf_engine_t* engine, ms_interface_t* net, int server) {
	state_t*      state = malloc(sizeof(*state));
	gf_network_t* r	    = malloc(sizeof(*r));
	memset(r, 0, sizeof(*r));
	r->engine    = engine;
	r->net	     = net;
	r->fd	     = -1;
	r->connected = 0;

	state->len    = strlen("ClientHello") + 1 + 1 + 1 + 64 + 2;
	state->first  = 1;
	state->list   = NULL;
	state->buffer = malloc(1);
	state->brk    = 0;
	state->good   = 0;
	state->server = server;

	r->state = state;

	gf_network_key(r);

	state->buffer[0] = 0;

	gf_log_function(engine, "GFSL handshaking", "");

	state->began_at = time(NULL);
	if(!server) {
		gf_log_function(engine, "Sent ClientHello", "");
		state->buf = ms_wbuffer(net, state->len);
		memcpy(state->buf->data, "ClientHello 1 ", state->len - 64 - 2);
		gf_network_key_hex(((char*)state->buf->data) + (state->len - 64 - 2), r->public_key);
		memcpy(((char*)state->buf->data) + (state->len - 2), "\r\n", 2);
	} else {
		state->first = 0;
		state->buf   = ms_rbuffer(net, 1);
	}

	return r;
}

static int gf_network_secure_step(gf_network_t* n) {
	ms_interface_t* net   = n->net;
	state_t*	state = n->state;
	int		ret   = 0;
	do {
		int st = ms_step(net);
		if((time(NULL) - state->began_at) >= 3) {
			gf_log_function(n->engine, "Timeout", "");
			state->brk = 1;
		}
		if(st != 0 || net->state >= MS_STATE_FAILED) {
			state->brk = 1;
		} else if(net->state == MS_STATE_AFTER_WRITE && state->good) {
			ret = 1;
			break;
		} else if((net->state == MS_STATE_AFTER_WRITE && state->first) || (net->state == MS_STATE_AFTER_READ && !state->good)) {
			state->first = 0;
			state->buf   = ms_rbuffer(net, 1);
		} else if(net->state == MS_STATE_READ_COMPLETE) {
			unsigned char c = *(unsigned char*)state->buf->data;
			if(c == '\n') {
				int i;
				int incr = 0;
				int len	 = strlen(state->buffer);
				for(i = 0; i <= len; i++) {
					if(state->buffer[i] == ' ' || i == len) {
						char* arg	 = state->buffer + incr;
						state->buffer[i] = 0;
						if(strlen(arg) > 0) {
							arrput(state->list, arg);
						}
						incr = i + 1;
						if(i == len) break;
					}
				}

				if(state->list != NULL && arrlen(state->list) > 0) {
					if(strcmp(state->list[0], state->server ? "ClientHello" : "ServerHello") == 0 && arrlen(state->list) == 3 && strcmp(state->list[1], "1") == 0 && strlen(state->list[2]) == 64) {
						gf_uint8_t their_public[X25519_KEY_SIZE];
						gf_uint8_t iv[AES_BLOCKLEN];
						memset(iv, 0, sizeof(iv));
						gf_log_function(n->engine, "Got %s", state->list[0]);

						for(i = 0; i < X25519_KEY_SIZE; i++) {
							their_public[i] = (gf_network_hex(state->list[2][i * 2 + 0]) << 4) | gf_network_hex(state->list[2][i * 2 + 1]);
						}
						if(state->server) {
							int	     len2 = strlen("ServerHello") + 1 + 1 + 1 + 64 + 2;
							ms_buffer_t* wbuf = ms_wbuffer(net, len2);
							memcpy(wbuf->data, "ServerHello 1 ", len2 - 64 - 2);
							gf_network_key_hex(((char*)wbuf->data) + (len2 - 64 - 2), n->public_key);
							memcpy(((char*)wbuf->data) + (len2 - 2), "\r\n", 2);

						} else {
							int	     len2 = strlen("ClientAccept") + 2;
							ms_buffer_t* wbuf = ms_wbuffer(net, len2);
							memcpy(wbuf->data, "ClientAccept\r\n", len2);
							state->good = 1;
							gf_log_function(n->engine, "Sent ClientAccept", "");
						}
						compact_x25519_shared(n->shared_secret, n->private_key, their_public);
						AES_init_ctx_iv(&n->aes, n->shared_secret, &iv[0]);
						/* DO NOT enable this unless you know what you are doing!!! */
#if 0
						{
							char b[64 + 1];
							b[64] = 0;
							gf_network_key_hex(b, n->shared_secret);
							gf_log_function(n->engine, "Round key: %s", b);
						}
#endif
					} else if(state->server && strcmp(state->list[0], "ClientAccept") == 0 && arrlen(state->list) == 1) {
						gf_log_function(n->engine, "Got %s", state->list[0]);
						ret = 1;
						break;
					} else {
						printf("%d\n", arrlen(state->list));
						state->brk = 1;
					}
					arrfree(state->list);
					state->list = NULL;
				} else {
					state->brk = 1;
				}
				free(state->buffer);
				state->buffer	 = malloc(1);
				state->buffer[0] = 0;
			} else if(c != '\r') {
				char* old     = state->buffer;
				state->buffer = malloc(strlen(old) + 2);
				strcpy(state->buffer, old);
				state->buffer[strlen(old)]     = c;
				state->buffer[strlen(old) + 1] = 0;
				free(old);
			}
		}
		if(state->brk) {
			ret = -1;
			break;
		}
	} while(0);

	if(ret != 0) {
		free(state->buffer);
		if(state->list != NULL) arrfree(state->list);
		if(state->brk) {
			gf_log_function(n->engine, "Handshake failed", "");
		} else {
			gf_log_function(n->engine, "Handshake success", "");
			n->connected = 1;
		}
		free(state);
		n->state = NULL;
	}

	return ret;
}

gf_network_t* gf_network_secure_tcp(gf_engine_t* engine, const char* host, int port) {
	ms_interface_t* net;
	gf_network_t*	r;
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

	r = gf_network_secure(engine, net, 0);
	if(r != NULL) {
		int s;
		while((s = gf_network_secure_step(r)) == 0);
	}
	return r;
}

gf_network_t* gf_network_secure_tcp_server(gf_engine_t* engine, const char* host, int port) {
	gf_network_t* net = malloc(sizeof(*net));
	memset(net, 0, sizeof(*net));
	net->engine = engine;
	net->fd	    = -1;

	net->fd = gf_network_socket(engine, "tcp", host, port);
	if(net->fd == -1) {
		free(net);
		return NULL;
	}

	return net;
}

int gf_network_secure_server_step(gf_network_t* net) {
	fd_set	       fds;
	int	       s;
	struct timeval tv;
	FD_ZERO(&fds);
	FD_SET(net->fd, &fds);

	tv.tv_sec  = 0;
	tv.tv_usec = 0;

	s = select(FD_SETSIZE, &fds, NULL, NULL, &tv);
	if(s == -1) {
		return -1;
	}
	if(s > 0) {
		struct sockaddr_in addr;
		int		   len = sizeof(addr);
		int		   fd  = accept(net->fd, (struct sockaddr*)&addr, &len);
		gf_network_t*	   cn;
		ms_interface_t*	   nint;

		gf_log_function(net->engine, "Connection accepted", "");

		ms_non_block(fd);
		nint = ms_user(fd);
		cn   = gf_network_secure(net->engine, nint, 1);
		arrput(net->clients, cn);
	} else {
		int i;
		for(i = 0; i < arrlen(net->clients); i++) {
			if(net->clients[i]->connected) {
				int s = ms_step(net->clients[i]->net);
				if(s != 0) {
					ms_destroy(net->clients[i]->net);
					arrdel(net->clients, i);
					i--;
					continue;
				}
			} else {
				int s = gf_network_secure_step(net->clients[i]);
				if(s == -1) {
					gf_network_destroy(net->clients[i]);
					arrdel(net->clients, i);
					i--;
					continue;
				}
			}
		}
	}
	return 0;
}

void gf_network_destroy(gf_network_t* net) {
	if(net->net != NULL) ms_destroy(net->net);
	if(net->fd != -1) gf_network_close(net->fd);
	if(net->state != NULL) {
		state_t* state = net->state;
		free(state->buffer);
		if(state->list != NULL) arrfree(state->list);
		free(state);
	}
	if(net->clients != NULL) {
		int i;
		for(i = 0; i < arrlen(net->clients); i++) {
			gf_network_destroy(net->clients[i]);
		}
		arrfree(net->clients);
	}
	gf_log_function(net->engine, "Destroyed network interface", "");
	free(net);
}
