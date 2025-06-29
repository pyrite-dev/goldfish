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
 * File Name:    gf_unicode.c
 * Author:       Nishi
 * 
 * Functionality:
 * TODO: Write here
 * 
 **********************************************************************************
 * @endif
 */

#include <gf_pre.h>

/* External library */

/* Interface */
#include <gf_unicode.h>

/* Engine */
#include <gf_type/compat.h>

/* Standard */

#define CAST_I32(x) ((gf_int32_t)(x))
/* maybe should use gf_uint8_t but no one uses a computer where sizeof(unsigned char) is not 8... */
static int gf_utf8_count(unsigned char c) {
	if(0 <= c && c < 0x80) {
		return 1;
	}
	if(0xc2 <= c && c < 0xe0) {
		return 2;
	}
	if(0xe0 <= c && c < 0xf0) {
		return 3;
	}
	if(0xf0 <= c && c < 0xf8) {
		return 4;
	}
	return 0;
}

static int gf_utf8_later(unsigned char c) { return 0x80 <= c && c < 0xc0; }

int gf_unicode_8_to_32(const char* input, gf_int32_t* output) {
	const unsigned char* inbuf = (const unsigned char*)input;
	int		     b	   = gf_utf8_count(inbuf[0]);
	if(b == 0) return 0;

	if(b == 1) *output = inbuf[0];
	if(b == 2) {
		if(!gf_utf8_later(inbuf[1])) return 0;
		if((inbuf[0] & 0x1e) == 0) return 0;

		*output = CAST_I32(inbuf[0] & 0x1f) << 6;
		*output |= CAST_I32(inbuf[1] & 0x3f);
	}
	if(b == 3) {
		if(!gf_utf8_later(inbuf[1]) || !gf_utf8_later(inbuf[2])) return 0;
		if((inbuf[0] & 0x0f) == 0 && (inbuf[1] & 0x20) == 0) return 0;

		*output = CAST_I32(inbuf[0] & 0x0f) << 12;
		*output |= CAST_I32(inbuf[1] & 0x3f) << 6;
		*output |= CAST_I32(inbuf[2] & 0x3f);
	}
	if(b == 4) {
		if(!gf_utf8_later(inbuf[1]) || !gf_utf8_later(inbuf[2]) || !gf_utf8_later(inbuf[3])) return 0;
		if((inbuf[0] & 0x07) == 0 && (inbuf[1] & 0x30) == 0) return 0;

		*output = CAST_I32(inbuf[0] & 0x07) << 18;
		*output |= CAST_I32(inbuf[1] & 0x3f) << 12;
		*output |= CAST_I32(inbuf[2] & 0x3f) << 6;
		*output |= CAST_I32(inbuf[3] & 0x3f);
	}

	return b;
}
