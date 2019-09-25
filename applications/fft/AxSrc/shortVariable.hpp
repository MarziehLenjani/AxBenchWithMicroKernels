#ifndef __SHORTVARIABLE_HPP__
#define __SHORTVARIABLE__

#include <math.h>
extern unsigned int posMask;
extern unsigned int negMask;
extern float maxValue1;
extern float  minValue1;
extern float maxValue2;
extern float  minValue2;
extern float StepSize1;
extern float StepSize2;
extern float StepSize3;
extern int NumberOfBits;
extern float maxValue3;
extern float minValue3;
extern float maxValue4;
extern float minValue4;
extern float errorRate;
extern  int maxExp;
extern float maxMantissa;
extern int minExp;
extern float minMantissa;
extern unsigned ratio;
extern int numberOfRangeTableEntry;
extern float relativeError
#define MaxNumberOfVarInStruct 10


typedef struct {
   long  beginAddr;
   long endAddr;
   long touched Addr=0;
   float stepSizArr[MaxNumberOfVarInStruct];
   int numberOfbits;
} RangeTableEntry;

extern RangeTableEntry RangeTable [1000];
//-------activate 
//#define Quantization
//#define IgnoreOulierQuantization //-------activate Quantization as well
//#define multiple_StepSize //-------activate Quantization as well
//------------------
#define ActiveMasking

void maskVariable(float* valueAdr, unsigned lposMsk,unsigned lnegMsk);
void SetMaxAndMin(float value, float* MaxAdr, float* MinAdr);
void Quantize(float* valueAdr,float StpSize,int numBits);
void setTheMasks( int numBit, unsigned int * pMask, unsigned int * nMask);


#endif
