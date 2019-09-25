
#include "fourier.hpp"
#include <cmath>
#include <fstream>
#include <map>


void calcFftIndices(int K, int* indices)
{
	int i, j ;
	int N ;

	N = (int)log2f(K) ;

	indices[0] = 0 ;
	indices[1 << 0] = 1 << (N - (0 + 1)) ;
	for (i = 1; i < N; ++i)
	{
		for(j = (1 << i) ; j < (1 << (i + 1)); ++j)
		{
			indices[j] = indices[j - (1 << i)] + (1 << (N - (i + 1))) ;
		}
	}
}

void radix2DitCooleyTykeyFft(int K, int* indices, Complex* x, Complex* f)
{

	calcFftIndices(K, indices) ;

	int step ;
	float arg ;
	int eI ;
	int oI ;

	float fftSin;
	float fftCos;

	Complex t;
	int i ;
	int N ;
	int j ;
	int k ;

	double dataIn[1];
	double dataOut[2];

	for(i = 0, N = 1 << (i + 1); N <= K ; i++, N = 1 << (i + 1))
	{
		//float maxValue1=0;
		//float minValue1=10000;
		//float maxValue2=0;
		//float minValue2=10000;

		for(j = 0 ; j < K ; j += N)
		{
			step = N >> 1 ;
			for(k = 0; k < step ; k++)
			{
				arg = (float)k / N ;
				eI = j + k ; 
				oI = j + step + k ;

				dataIn[0] = arg;

//#pragma parrot(input, "fft", [1]dataIn)

				fftSinCos(arg, &fftSin, &fftCos);

				dataOut[0] = fftSin;
				dataOut[1] = fftCos;

//#pragma parrot(output, "fft", [2]<0.0; 2.0>dataOut)

				fftSin = dataOut[0];
				fftCos = dataOut[1];


				// Non-approximate
				t =  x[indices[eI]] ;
				x[indices[eI]].real = t.real + (x[indices[oI]].real * fftCos - x[indices[oI]].imag * fftSin);
				//printf(" x.real is % f \n" , x[indices[eI]].real);
				x[indices[eI]].imag = t.imag + (x[indices[oI]].imag * fftCos + x[indices[oI]].real * fftSin);
				x[indices[oI]].real = t.real - (x[indices[oI]].real * fftCos - x[indices[oI]].imag * fftSin);
				x[indices[oI]].imag = t.imag - (x[indices[oI]].imag * fftCos + x[indices[oI]].real * fftSin);
				// 
				// #ifdef ActiveMasking
				// maskVariable(&x[indices[eI]].real, posMask,negMask);
				 
				//printf("posMask is:  %08x   , negMask is %08x  \n", posMask,negMask);
				// #endif
				// #ifdef Quantization
				// SetMaxAndMin( x[indices[eI]].real,&maxValue1,&minValue1);
				// Quantize(&x[i].real,StepSize1,NumberOfBits);

				//printf("Max is:  %f   , min is %f  \n", maxValue1,minValue1);
				// #endif
                
				// #ifdef ActiveMasking
				// maskVariable(&x[indices[eI]].imag, posMask,negMask);
				// #endif
				// #ifdef Quantization
				// SetMaxAndMin( x[indices[eI]].imag,&maxValue2,&minValue2);
				// Quantize(&x[indices[eI]].imag,StepSize1,NumberOfBits);
				// #endif
                
				// #ifdef ActiveMasking
				// maskVariable(&x[indices[oI]].real, posMask,negMask);
				// #endif
				// #ifdef Quantization
				// SetMaxAndMin( x[indices[oI]].real,&maxValue1,&minValue1);
				// Quantize(&x[indices[oI]].real,StepSize1,NumberOfBits);
				// #endif
                
				// #ifdef ActiveMasking
				// maskVariable(&x[indices[oI]].imag, posMask,negMask);
				// #endif
				// #ifdef Quantization
				// SetMaxAndMin( x[indices[oI]].imag,&maxValue2,&minValue2);
				// Quantize(&x[indices[oI]].imag,StepSize1,NumberOfBits);
				// #endif

				// 
				
			}


		}
		//printf("Max1 is:  %f   , min1 is %f  \n", maxValue1,minValue1);
		//printf("Max2 is:  %f   , min2 is %f  \n", maxValue2,minValue2);

	}

	for (int i = 0 ; i < K ; i++)
	{
		f[i] = x[indices[i]] ;
		//SetMaxAndMin( f[i].real,&maxValue3,&minValue3);
		//SetMaxAndMin( f[i].imag,&maxValue4,&minValue4);


	}
}
