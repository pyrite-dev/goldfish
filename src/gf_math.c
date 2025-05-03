#include <gf_pre.h>

/* External library */

/* Interface */
#include <gf_math.h>

/* Engine */

/* Standard */
#include <math.h>

double gf_math_log2(double x) { return log(x) / log(2); }

double gf_math_round(double x) {
	if((x - (int)x) >= 0.5) {
		return 1 + (int)x;
	}
	return (int)x;
}

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
