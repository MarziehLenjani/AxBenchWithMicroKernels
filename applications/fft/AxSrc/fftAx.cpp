#include <cstdio>
#include <iostream>
#include "fourier.hpp"
#include <fstream>
#include <time.h>
#include <math.h>
#include <string.h>
#include "../../shortvariable/shortVariable.hpp"
#include <cmath>

static int* indices;
static Complex* x;
static Complex* f;



//Max is:  2096128.000000   , min is -668415.93i7500
int main(int argc, char* argv[])
{
	int i ;

	int n 						= atoi(argv[1]);
	std::string inputFilename	= argv[2];
	std::string outputFilename 	= argv[3];
	double maxAbsoluteError=0;
//------------------Added Part

	fileNamingArg =atoi(argv[4]);
	
	NumberOfBits1                    = fileNamingArg;
//	NumberOfBits2                    = fileNamingArg;
	NumberOfBits3                    = fileNamingArg;
	NumberOfBits4                    = fileNamingArg;
	
	setTheMasks(fileNamingArg, &posMask, &negMask);

	StartOfSimulationReport();

	StepSize1=	16.0/pow(2,NumberOfBits1); //TODO:change 111
	StepSize2=	1.0/pow(2,NumberOfBits2); //TODO:change 111
	StepSize3=	1024.0/pow(2,NumberOfBits3); //TODO:change 111
	//StepSize3=	1024.0/pow(2,NumberOfBits3); //TODO:change 111
        StepSize4=	256.0/pow(2,NumberOfBits4); //TODO:change 111
//	StepSize4=	256.0/pow(2,NumberOfBits4); //TODO:change 111
        //StepSize3=      32768/pow(2,NumberOfBits3);	
 	//StepSize4=      32768/pow(2,NumberOfBits4);
        //StepSize3=      2048/pow(2,NumberOfBits3);
        //StepSize4=      2048/pow(2,NumberOfBits);
	
	
	#ifdef Sampling
	StepSize1=	 pow(2,atoi(argv[5]));
	StepSize2=       pow(2,atoi(argv[6]));
	StepSize3=       pow(2,atoi(argv[7]));
	StepSize4=       pow(2,atoi(argv[8]));
        NumberOfSamplingItr= SamplingItrUnit * fileNamingArg ;
	#endif
	#ifdef multiple_StepSize	
	relativeError=	 pow(2,atoi(argv[5])); //acceptable absoulute error(step size) for the smallest value 
	#endif
 //	#if defined(ActiveMasking) || defined(Quantization)
	outputFilename=outputFilename;
	outputFilename=outputFilename+"_";
	outputFilename=outputFilename + std::to_string (fileNamingArg);	
//	#endif
	outputFilename=outputFilename+".txt";
//-----------End of Aded part	
	
	// StepSize2=	2096128.0/pow(2,NumberOfBits2)



	float mantissa;
	// prepare the output file for writting the theta values
	std::ofstream outputFileHandler;
	outputFileHandler.open(outputFilename);
	outputFileHandler.precision(5);
	std::ifstream inputFileHandler (inputFilename, std::ifstream::in);
//	printf("number of bits is %d \n",NumberOfBits );

	// create the arrays
	x 		= (Complex*)malloc(n * sizeof (Complex));
	f 		= (Complex*)malloc(n * sizeof (Complex));
	indices = (int*)malloc(n * sizeof (int));
	
	//------------------------------i
/*
	numberOfRangeTableEntry=2;
	RangeTable[0].beginAddr=&x[0];
	RangeTable[0].endAddr=&x[n];
	RangeTable[0].stepSizArr[0]=0;
	RangeTable[0].stepSizArr[1]=0;
	
	RangeTable[1].beginAddr=&f[0];
	RangeTable[1].endAddr=&f[n];
	RangeTable[1].stepSizArr[0]=0;
	RangeTable[1].stepSizArr[1]=0;
*/
	//-------------------------------

	if(x == NULL || f == NULL || indices == NULL)
	{
		std::cout << "cannot allocate memory for the triangle coordinates!" << std::endl;
		return -1 ;
	}

	int K = n;

	for(i = 0;i < K ; i++)
	{
//		x[i].real = i;
		inputFileHandler >> x[i].real;
		float temp=x[i].real;
		SetMaxAndMin( x[i].real,&maxValue1,&minValue1,&NumberOfBits1,StepSize1, i);
		#ifdef ActiveMasking
		maskVariable(&x[i].real, posMask,negMask);
		#endif
		#ifdef Quantization
		
		Quantize(&x[i].real,StepSize1,NumberOfBits1,i);
		#endif
		if(std::abs(x[i].real-temp)>maxAbsoluteError)
		{
			maxAbsoluteError=std::abs(x[i].real-temp);
		}
		//mantissa = frexp(x[i].real, &tmpExp); 
		/*
		if(i==0||mantissa>maxMantissa)
		{
			maxMantissa=mantissa;
		}
		if(i==0||tmpExp>maxExp)
		{
			maxExp=tmpExp;
		}
		if(i==0||mantissa<minMantissa)
		{
			minMantissa=mantissa;
		}
		if(i==0||tmpExp<minExp)
		{
			minExp=tmpExp;
		}
		*/
		
		x[i].imag = 0 ;
		

		
	}
	radix2DitCooleyTykeyFft(K, indices, x, f) ;
	
	for(i = 0;i < K ; i++)
	{
		
		SetMaxAndMin( f[i].real,&maxValue3,&minValue3, &NumberOfBits3,StepSize3, i);
		SetMaxAndMin( f[i].imag,&maxValue4,&minValue4,&NumberOfBits4,StepSize4, i);
		/*
              if(NumberOfBits ==32)
                {

                        printf( "%f ", f[i].imag);
                }
*/		
		#ifdef ActiveMasking
		maskVariable(&f[i].real, posMask,negMask);
		maskVariable(&f[i].imag, posMask,negMask);
		
		#endif
		#ifdef Quantization
		
		Quantize(&f[i].real,StepSize3,NumberOfBits3,i);
		Quantize(&f[i].imag,StepSize4,NumberOfBits4,i);

		#endif
	
		outputFileHandler << f[i].real << ", " << f[i].imag << std::endl;
		/*
		mantissa = frexp(x[i].real, &tmpExp); 
		if(i==0||tmpExp>maxExp)
		{
			maxExp=tmpExp;
		}
		if(i==0||mantissa>maxMantissa)
		{
			maxMantissa=mantissa;
		}
		if(i==0||mantissa<minMantissa)
		{
			minMantissa=mantissa;
		}
		if(i==0||tmpExp<minExp)
		{
			minExp=tmpExp;
		}
		*/


	}
	/*
	printf("-------------Max Mantissa---------------\n");
	printf("The fraction is %f and the exponent is %d\n", maxMantissa,maxExp );
	printf("The fraction is %08X and the exponent is %08X\n", *(unsigned int*)& maxMantissa,maxExp );
	mantissa = frexp(maxMantissa, &tmpExp);
	printf("The fraction is %f and the exponent is %d\n", mantissa,tmpExp );
	printf("The fraction is %08X and the exponent is %08X\n", *(unsigned int*)& mantissa,tmpExp);

	maskVariable(&maxMantissa, posMask,negMask);
	printf("The masked value is %f \n",  maxMantissa);	
	printf("The masked value is %08X \n", *(unsigned int*)& maxMantissa );	
	
	printf("-------------Min Mantissa---------------\n");
	printf("The fraction is %f and the exponent is %d\n", minMantissa,minExp );	
	printf("The fraction is %08X and the exponent is %08X\n",*(unsigned int*)& minMantissa,minExp );
	mantissa = frexp(minMantissa, &tmpExp);
	printf("The fraction is %f and the exponent is %d\n", mantissa,tmpExp );
	printf("The fraction is %08X and the exponent is %08X\n", *(unsigned int*)& mantissa,tmpExp);
	maskVariable(&minMantissa, posMask,negMask);
	printf("The masked value is  %f\n", minMantissa );
	printf("The masked value  %08X\n", *(unsigned int*)& minMantissa );
	*/

//	printf("numberOfRangeTableEntry is %d \n",numberOfRangeTableEntry);


	EndOfSimulationReport(4*n);
	printf("  %f", maxAbsoluteError);
	outputFileHandler.close();
	
	return 0 ;
}
