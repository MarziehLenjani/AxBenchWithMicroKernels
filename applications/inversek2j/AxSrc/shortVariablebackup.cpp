#include "shortVariable.hpp"
#include <math.h>
#include <cstdlib>
int NumberOfBits;
//unsigned int posMask=0xFF800000;
unsigned int posMask=0x00000000;
unsigned int negMask=0x00000000;
float maxValue1=0;
float minValue1=10000;
float maxValue2=0;
float minValue2=10000;
float maxValue3=0;
float minValue3=10000;
float maxValue4=0;
float minValue4=10000;
double StepSize1;
double StepSize2;
double StepSize3;

double errorRate=0;
int maxExp=0;
float maxMantissa=0;
int minExp=10000;
float minMantissa=0;
unsigned ratio=1;



void Quantize(float* valueAdr,double StpSize, int numBits)
{
	//printf("before Quantized is %f ",*valueAdr);
	if(numBits!=0)
	{
		//*valueAdr=((long int)(*valueAdr/StpSize))*StpSize+(StpSize/2);
		double tmpStpSize=StpSize;
		double intPart;
		modf((*valueAdr)/StpSize,&intPart);
		
		#ifdef multiple_StepSize
		if (abs(intPart)>= pow(2,numBits-2))
		{
			tmpStpSize=(ratio/2.0)*StpSize;
		}else{
			if (abs(intPart)>= pow(2,numBits-1))
			{
				tmpStpSize=(ratio)*StpSize;
			}
		}
		intPart=((long int)(*valueAdr/tmpStpSize));		
		#endif
		#ifdef IgnoreOulierQuantization
		if (abs(intPart)> pow(2,numBits))
		{
			if(intPart>0)
			{
				intPart=pow(2,numBits);
			}
			if(intPart<0)
			{
				intPart= (-1)* pow(2,numBits);
			}
		}
		
		#endif
		*valueAdr=intPart*tmpStpSize;

	}
	
	//printf("after Quantized is %f ",*valueAdr);
}
void maskVariable(float* valueAdr, unsigned lposMsk,unsigned lnegMsk){
		if(*valueAdr>0)
		{
			unsigned int masked=(*(unsigned int*)valueAdr) & (lposMsk);
			*valueAdr=(*(float*)& masked);	
				
		}	
		/*//TODO:neg mask should only consider negativity of the exponent
		if(*valueAdr<0)
		{
			unsigned int masked=(*(unsigned int*)valueAdr) | (lnegMsk);
			*valueAdr=(*(float*)& masked);	
				
		}		
		*/
}
void SetMaxAndMin(float value, float* MaxAdr, float* MinAdr)
{
	if(value>* MaxAdr)
	{
		*MaxAdr=value;
	}
	if(value<* MinAdr)
	{
		*MinAdr=value;
	}
	int tmpExp;
	float mantissa = frexp(value, &tmpExp);
	if(tmpExp>maxExp)
	{
		maxExp=tmpExp;
	}
	if(tmpExp<minExp)
	{
		minExp=tmpExp;
	}
}
void setTheMasks( int numBit, unsigned int * pMask, unsigned int * nMask){
	if(numBit>0)
	{
		for (int ti=1; ti<=numBit;ti++)
		{
			*pMask=*pMask+ pow(2,32-ti);
		}
		//TODO fix Negmask: neg mask should only consider negativity of the exponent
		for (int ti=0; ti<(32-numBit);ti++)
		{
			*nMask=*nMask+ pow(2,ti);
		}
	}else{
		*pMask=0xFFFFFFFF;
		*nMask=0x00000000;		
	}
	
}
