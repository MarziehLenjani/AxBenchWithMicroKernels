#ifndef __SHORTVARIABLE_HPP__
#define __SHORTVARIABLE__

#include <math.h>
extern unsigned int posMask;
extern unsigned int negMask;
extern float maxValue1;
extern float minValue1;
extern float maxValue2;
extern float minValue2;
extern float maxValue3;
extern float minValue3;
extern float maxValue4;
extern float minValue4;
extern double StepSize1;
extern double StepSize2;
extern double StepSize3;
extern int NumberOfBits;
extern double errorRate;
extern  int maxExp;
extern float maxMantissa;
extern int minExp;
extern float minMantissa;
extern unsigned ratio;
//-------activate 
//#define Quantization
//#define IgnoreOulierQuantization //-------activate Quantization as well
//#define multiple_StepSize //-------activate Quantization as well
//------------------
#define ActiveMasking

void maskVariable(float* valueAdr, unsigned lposMsk,unsigned lnegMsk);
void SetMaxAndMin(float value, float* MaxAdr, float* MinAdr);
void Quantize(float* valueAdr,double StpSize,int numBits);
void setTheMasks( int numBit, unsigned int * pMask, unsigned int * nMask);


#endif
