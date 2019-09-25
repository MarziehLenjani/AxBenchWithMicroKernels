#include "complex.hpp"
#include <cmath>

#if defined(SWQNT) || defined(SWQNTInPlace)
extern float SWQntstepsize;
extern float mid;
extern int TmpSwQnt;
extern Complex* SwQx;
extern Complex* SwQf;
#endif


void fftSinCos(float x, float* s, float* c) {
#if defined( SWQNT)  || defined(SWQNTInPlace)
TmpSwQnt+=(x+mid)/SWQntstepsize;
#endif
    *s = sin(-2 * PI * x);
    *c = cos(-2 * PI * x);
}

float abs(const Complex* x) {
	return sqrt(x->real * x->real + x->imag * x->imag);
}

float arg(const Complex* x) {
	if (x->real > 0)
		return atan(x->imag / x->real);

	if (x->real < 0 && x->imag >= 0)
		return atan(x->imag / x->real) + PI;

	if (x->real < 0 && x->imag < 0)
		return atan(x->imag / x->real) - PI;

	if (x->real == 0 && x->imag > 0)
		return PI / 2;

	if (x->real == 0 && x->imag < 0)
		return -PI / 2;

	if (x->real == 0 && x->imag == 0)
		return 0;

	return 0;
}
