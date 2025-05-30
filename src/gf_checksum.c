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
