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
 * File Name:    gf_math.c
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
#include <gf_math.h>

/* Engine */

/* Standard */
#include <math.h>

double gf_math_log2(double x) { return log(x) / log(2); }

double gf_math_abs(double x) {
	if(x < 0) return -x;
	return x;
}

double gf_math_round(double x) {
	if((x - (int)x) >= 0.5) {
		return 1 + (int)x;
	}
	return (int)x;
}

double gf_math_floor(double x) { return (int)x; }

double gf_math_ceil(double x) { return 1 + (int)x; }

void gf_math_normalize(gf_math_vector_t v) {
	double x = v[0];
	double y = v[1];
	double z = v[2];
	double length;

	x *= x;
	y *= y;
	z *= z;

	length = sqrt(x + y + z);
	if(length > 0) {
		length = (double)1 / length;
	} else {
		length = 0;
	}
	v[0] *= length;
	v[1] *= length;
	v[2] *= length;
}

void gf_math_normal(gf_math_vector_t r, gf_math_vector_t v0, gf_math_vector_t v1, gf_math_vector_t v2) {
	gf_math_vector_t tmp0;
	gf_math_vector_t tmp1;

	gf_math_subtract(tmp0, v1, v0);
	gf_math_subtract(tmp1, v2, v0);

	gf_math_multiply(r, tmp0, tmp1);

	gf_math_normalize(r);
}

void gf_math_subtract(gf_math_vector_t r, gf_math_vector_t v0, gf_math_vector_t v1) {
	r[0] = v0[0] - v1[0];
	r[1] = v0[1] - v1[1];
	r[2] = v0[2] - v1[2];
}

void gf_math_multiply(gf_math_vector_t r, gf_math_vector_t v0, gf_math_vector_t v1) {
	r[0] = v0[1] * v1[2] - v0[2] * v1[1];
	r[1] = v0[2] * v1[0] - v0[0] * v1[2];
	r[2] = v0[0] * v1[1] - v0[1] * v1[0];
}

double gf_math_cot(double x) { return (double)1 / tan(x); }

double gf_math_nearest_2pow(double x) {
	double l = gf_math_log2(x);
	double r = pow(2, (int)l);
	if(x == r) {
		return r;
	}
	return pow(2, 1 + (int)l);
}
