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
extern double StepSize4;
extern int NumberOfBits1;
extern int NumberOfBits2;
extern int NumberOfBits3;
extern int NumberOfBits4;
extern int fileNamingArg;
extern double errorRate;
extern  int maxExp;
extern float maxMantissa;
extern int minExp;
extern float minMantissa;
extern unsigned ratio;

extern long int NumberOfOutliers1;
extern long int NumberOfOutliers2;
extern long int NumberOfOutliers3;
extern long int NumberOfOutliers4;

extern long int TotalNumberOfOutliers;
//-------activate 
//#define Quantization
//#define IgnoreOulierQuantization //-------actiivate Quantiziation as well
//#define Sampling      //-------activate Quantization as well
//#define SamplingItrUnit 100
//#define multiple_StepSize //-------activate Quantization as well
//------------------
#define ActiveMasking
#define ByteVsExponent

extern long int  NumberOfSamplingItr;

void maskVariable(float* valueAdr, unsigned lposMsk,unsigned lnegMsk);
void SetMaxAndMin(float value, float* MaxAdr, float* MinAdr, int * numBits ,double StpSize, long int  iterationNumber );
void Quantize(float* valueAdr,double StpSize, int numBits, int iterationNumber);
void setTheMasks( int numBit, unsigned int * pMask, unsigned int * nMask);
void StartOfSimulationReport();
void EndOfSimulationReport(long int n);
#endif
