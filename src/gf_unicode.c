#include <gf_pre.h>

/* External library */

/* Interface */
#include <gf_unicode.h>

/* Engine */
#include <gf_type/compat.h>

/* Standard */

#define CAST_I32(x) ((gf_int32_t)(x))
/* maybe should use gf_uint8_t but no one uses a computer where sizeof(unsigned char) is 8... */
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
