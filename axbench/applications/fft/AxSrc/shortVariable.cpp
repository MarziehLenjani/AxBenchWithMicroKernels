#include "shortVariable.hpp"
#include <math.h>
#include <cstdlib>
int NumberOfBits;
unsigned int posMask=0xFF800000;
unsigned int negMask=0x00000000;
float maxValue1=0;
float minValue1=10000;
float maxValue2=0;
float minValue2=10000;
float StepSize1;
float StepSize2;
float StepSize3;
float maxValue3=0;
float minValue3=10000;
float maxValue4=0;
float minValue4=10000;
float errorRate=0;
int maxExp=0;
float maxMantissa=0;
int minExp=10000;
float minMantissa=0;
unsigned ratio=4;
int numberOfRangeTableEntry;
float relativeError;


/*
void Quantize(float* valueAdr,float StpSize, int numBits)
{
	//printf("before Quantized is %f ",*valueAdr);
	if(numBits!=0)
	{
		//*valueAdr=((long int)(*valueAdr/StpSize))*StpSize+(StpSize/2);
		float tmpStpSize=StpSize;
		int intPart=((long int)(*valueAdr/StpSize));
		
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
*/
void Quantize(float* valueAdr,int varNumberinStruct, int numBits)
{
	//printf("before Quantized is %f ",*valueAdr);
	if(numBits!=0)
	{
		//*valueAdr=((long int)(*valueAdr/StpSize))*StpSize+(StpSize/2);
		float tmpStpSize=StepSize1;
		int intPart=((long int)(*valueAdr/tmpStpSize));
		float tmpRelativeErro=(*valueAdr)/tmpStpSize;
		#ifdef multiple_StepSize		
		for (int i=0; i<numberOfRangeTableEntry;i++)
		{
			if(valueAdr>=RangeTable[i].beginAddr && valueAdr<RangeTable[i].endAddr)
			{
				tmpStpSize=RangeTable[i].stepSizArr[varNumberinStruct];
				int intPart=((long int)(*valueAdr/tmpStpSize));
				if (abs(intPart)>= pow(2,numBits-1)  ) //just For numbers with negative values
				{
					if(RangeTable[i].stepSizArr[varNumberinStruct]!=tmpStpSize && RangeTable[i].stepSizArr[varNumberinStruct]!=0){
						numberOfRangeTableEntry++;	
						RangeTable[numberOfRangeTableEntry].endAddr=RangeTable[i].endAddr;
						RangeTable[numberOfRangeTableEntry].beginAddr=valueAdr;
						RangeTable[i].endAddr=valueAdr;
					}
					for(int j=0;j<MaxNumberOfVarInStruct;j++)
					{
						RangeTable[numberOfRangeTableEntry].stepSizArr[j]=RangeTable[i].stepSizArr[j];
					}
					while(abs(intPart)>= pow(2,numBits-1))
					{
						tmpStpSize=RangeTable[numberOfRangeTableEntry].stepSizArr[varNumberinStruct]*2;
						RangeTable[numberOfRangeTableEntry].stepSizArr[varNumberinStruct]=tmpStpSize;
						intPart=((long int)(*valueAdr/tmpStpSize));
					}
				}else if(tmpRelativeErro>relativeError)
				{
					if(RangeTable[i].stepSizArr[varNumberinStruct]!=tmpStpSize  && RangeTable[i].stepSizArr[varNumberinStruct]!=0){
						numberOfRangeTableEntry++;				
					}
					RangeTable[numberOfRangeTableEntry].endAddr=RangeTable[i].endAddr;
					RangeTable[numberOfRangeTableEntry].beginAddr=valueAdr;
					RangeTable[i].endAddr=valueAdr;
					for(int j=0;j<MaxNumberOfVarInStruct;j++)
					{
						RangeTable[numberOfRangeTableEntry].stepSizArr[j]=RangeTable[i].stepSizArr[j];
					}
					while(tmpRelativeErro>relativeError)
					{
						tmpStpSize=RangeTable[numberOfRangeTableEntry].stepSizArr[varNumberinStruct]/2;
						RangeTable[numberOfRangeTableEntry].stepSizArr[varNumberinStruct]=tmpStpSize;
						intPart=((long int)(*valueAdr/tmpStpSize));
						tmpRelativeErro=(*valueAdr)/tmpStpSize;
					}					
				}
				intPart=((long int)(*valueAdr/tmpStpSize));		
			}
		}
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
		if(*valueAdr<0)
		{
			unsigned int masked=(*(unsigned int*)valueAdr) | (lnegMsk);
			*valueAdr=(*(float*)& masked);	
				
		}		
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
			*pMask=*pMask+ pow(2,23-ti);
		}
		for (int ti=0; ti<(23-numBit);ti++)
		{
			*nMask=*nMask+ pow(2,ti);
		}
	}else{
		*pMask=0xFFFFFFFF;
		*nMask=0x00000000;		
	}
	
}