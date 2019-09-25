#ifndef __FOURIER_HPP__
#define __FOURIER_HPP__


#include "complex.hpp"
#include <iostream>
/*
extern unsigned int posMask;
extern unsigned int negMask;
extern float maxValue1;
extern float  minValue1;
extern float maxValue2;
extern float  minValue2;
extern float StepSize1;
extern int NumberOfBits;
extern float maxValue3;
extern float minValue3;
extern float maxValue4;
extern float minValue4;
*/

void radix2DitCooleyTykeyFft(int k, int* indices, Complex* x, Complex* f) ;
#endif
