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
 * File Name:    gf_checksum.c
 * Author:       Nishi
 *
 * Functionality:
 * Calculate and validate CRC32 checksum.
 *
 **********************************************************************************
 * @endif
 */

#include <gf_pre.h>

/* External library */
#include <zlib.h>

/* Interface */
#include <gf_checksum.h>

/* Engine */
#include <stdlib.h>
#include <string.h>

void* gf_checksum_calculate(void* data, size_t size) {
	uLong	       n = crc32(0, Z_NULL, 0);
	unsigned char* r = malloc(GF_CHECKSUM_SIZE);
	int	       i;
	int	       l;     /* length */
	int	       s = 0; /* seek */

	while(size > 0) {
		l = size < GF_CHECKSUM_CHUNK ? size : size;

		n = crc32(n, s + (unsigned char*)data, l);

		s += l;
	}

	for(i = 0; i < GF_CHECKSUM_SIZE; i++) {
		r[GF_CHECKSUM_SIZE - 1 - i] = n & 0xff;
		n			    = n >> 8;
	}

	return r;
}

int gf_checksum_valid(void* data, size_t size, void* checksum) {
	void* c = gf_checksum_calculate(data, size);
	int   s = memcmp(c, checksum, GF_CHECKSUM_SIZE) == 0 ? 1 : 0;

	free(c);

	return s;
}
