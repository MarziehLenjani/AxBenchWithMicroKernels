#include "shortVariable.hpp"
#include <math.h>
#include <cstdlib>
#include <iostream>
int NumberOfBits1=0;
int NumberOfBits2=0;
int NumberOfBits3=0;
int NumberOfBits4=0;
int fileNamingArg;
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
double StepSize4;

double errorRate=0;
int maxExp=0;
float maxMantissa=0;
int minExp=10000;
float minMantissa=0;
unsigned ratio=1;


long int NumberOfOutliers1=0;
long int NumberOfOutliers2=0;
long int NumberOfOutliers3=0;
long int NumberOfOutliers4=0;

long int TotalNumberOfOutliers=0;

long int  NumberOfSamplingItr=-1;


void Quantize(float* valueAdr,double StpSize, int numBits, int iterationNumber)
{
	float beforeQuant=*valueAdr;
	//printf("before Quantized is %f ",*valueAdr);
        double tmpStpSize=StpSize;
        double intPart;
	if(numBits!=0 && iterationNumber> NumberOfSamplingItr) 
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
		
		if (abs(intPart)> pow(2,numBits))
		{
			TotalNumberOfOutliers++;
			#ifdef IgnoreOulierQuantization
			if(intPart>0)
			{
				intPart=pow(2,numBits);
			}
			if(intPart<0)
			{
				intPart= (-1)* pow(2,numBits);
			}
			#endif
		}
		
	
		*valueAdr=float(intPart*tmpStpSize);
//		std::cout<<"  " <<intPart<<" "<<tmpStpSize<<" "<< *valueAdr<<std::endl;

	}
	if(!isnan(beforeQuant) and isnan(*valueAdr))
	{
		std::cout<<" value is: "<<beforeQuant<<" intpart is  " <<intPart<<" tmpSize if "<<tmpStpSize<<" final value is "<< *valueAdr<<std::endl;
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
void SetMaxAndMin(float value, float* MaxAdr, float* MinAdr, int * numBits ,double StpSize, long int  iterationNumber )
{
	#ifdef Sampling
	if(iterationNumber< NumberOfSamplingItr || fileNamingArg==0)  
	{
	#endif
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
	#ifdef Sampling
	}
	#endif
	#ifdef Sampling
        if (fileNamingArg!=0 &&  iterationNumber>= NumberOfSamplingItr)	
	{ 
		//std::cout<<"iterationNumber is "<<iterationNumber<<std::endl;
		double diff=(*MaxAdr) - (*MinAdr);
		if(diff==0)
		{
			*numBits=1;
		}else{
			double quient= abs(diff/StpSize);
			*numBits=int(log2(quient))+1;
			if((*numBits)<=0  || (*numBits>32))
			{		
				printf("invalidNumberOfBits: %d   ",*numBits);
				std::cout<<"Max is "<<*MaxAdr<<" Min is"<<*MinAdr<<" StepSize:"<<StpSize<<" quient is "<<quient<<" diff is"<< diff<<std::endl;
				std::cout<<"iterationNumber is "<<iterationNumber<<std::endl;
				*numBits=0;
				exit(1);
			}
		}
	}
		
	#endif
}
void setTheMasks( int numBit, unsigned int * pMask, unsigned int * nMask){
	if(numBit>0)
	{
	#ifdef ByteVsExponent
		if(numBit==1)
		{ 
			for (int ti=1; ti<=24;ti++)
			{	
				*pMask=*pMask+ pow(2,32-ti);
			}
		}
		if(numBit==2)
		{
                        for (int ti=1; ti<=32;ti++)
                        {
				if(ti!=9)
				{
                               		 *pMask=*pMask+ pow(2,32-ti);
				}
                        }
		}
	#else
                for (int ti=1; ti<=numBit;ti++)
                {
                        *pMask=*pMask+ pow(2,32-ti);
                }		
	#endif
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
void EndOfSimulationReport(long  int n)
{
        if(fileNamingArg==0)
        {
	        printf("\n Max1 is:  %f   , min1 is %f  \n", maxValue1,minValue1);
        	printf("Max2 is:  %f   , min2 is %f  \n", maxValue2,minValue2);

        	printf("Max3 is:  %f   , min3 is %f  \n", maxValue3,minValue3);
       	        printf("Max4 is:  %f   , min3 is %f  \n", maxValue4,minValue4);
		printf ("Method fileNamingArg  NumberOfBits1 NumberOfBits2 NumberOfBits3 NumberOfBits4  TotalNumberOfOutliers  outlierRate MAxAbsErrInput AverageRelativeError MaxAbsErrorOutput MaxRelaAbsErr\n");
        }
	
       #ifdef ActiveMasking
            std::string technique("OLSB");
       
        #endif

        #ifdef Quantization
            std::string technique("Quantization");
        #endif
		#ifdef Original
			std::string technique("Original");
		#endif
	 printf("%s %2d  %2d  %2d  %2d  %2d  %20ld    %f",technique.c_str(),fileNamingArg, NumberOfBits1,NumberOfBits2,NumberOfBits3,NumberOfBits4, TotalNumberOfOutliers, (TotalNumberOfOutliers*1.0)/(n));

     

}
void StartOfSimulationReport()
{
       #ifdef ActiveMasking
        if(fileNamingArg==0)
        {
                printf("--------------------Masking is defined");
        }
        #endif

        #ifdef Quantization

        if(fileNamingArg==0)
        {
                printf("--------------------Quantization is defined");
        }
	#endif

}
