/*
 * inversek2j.cpp
 * 
 *  Created on: Sep. 10 2013
 *			Author: Amir Yazdanbakhsh <yazdanbakhsh@wisc.edu>
 */

#include <iostream>
#include <cstdlib>
#include "kinematics.hpp"
#include <fstream> 

#include <time.h>
#include <iomanip>
#include <string> 
#include <cmath>

#define PI 3.141592653589

#include <boost/math/special_functions/fpclassify.hpp>
#include "../../shortvariable/shortVariable.hpp"

int main(int argc, const char* argv[])
{

	int n;
	std::string inputFilename	= argv[1];
	std::string outputFilename 	= argv[2];

	double maxAbsoluteError=0;
//	#if defined(ActiveMasking) || defined(Quantization)
	fileNamingArg			= atoi(argv[3]);
	NumberOfBits1 = fileNamingArg;
	//NumberOfBits2 = fileNamingArg; //not used
	NumberOfBits3 = fileNamingArg;
	//NumberOfBits4 = fileNamingArg;//not used
//	#endif

	StepSize1=	1.570796/pow(2,NumberOfBits1);
	StepSize2=      1.570796/pow(2,NumberOfBits2);//not used
	StepSize3=      1.570796/pow(2,NumberOfBits3);
	StepSize4=      1.570796/pow(2,NumberOfBits4);//not used
	#ifdef multiple_StepSize	
	//ratio= atof(argv[4]);
	ratio= 4;
	StepSize1=((1.0/ratio) *1.570796)/pow(2,NumberOfBits-2);
	#endif
	#ifdef Sampling
        StepSize1=       pow(2,atoi(argv[4]));
        StepSize2=       pow(2,atoi(argv[5]));
        StepSize3=       pow(2,atoi(argv[6]));
        StepSize4=       pow(2,atoi(argv[7]));
	NumberOfSamplingItr= SamplingItrUnit * fileNamingArg ;
	#endif
	
	
	//#if defined(ActiveMasking) || defined(Quantization)
	outputFilename=outputFilename;
	outputFilename=outputFilename+"_";
	outputFilename=outputFilename + std::to_string (fileNamingArg);	
	//#endif
	outputFilename=outputFilename+".txt";
	setTheMasks(fileNamingArg, &posMask, &negMask);
 
	// prepare the output file for writting the theta values
	std::ofstream outputFileHandler;
	outputFileHandler.open(outputFilename);

	// prepare the input file for reading the theta data
	std::ifstream inputFileHandler (inputFilename, std::ifstream::in);

	// first line defins the number of enteries
	inputFileHandler >> n;

	StartOfSimulationReport();
	float* t1t2xy = (float*)malloc(n * 2 * 2 * sizeof(float));

	if(t1t2xy == NULL)
	{
		std::cerr << "# Cannot allocate memory for the coordinates an angles!" << std::endl;
		return -1 ;
	}

	srand (time(NULL));

	int curr_index1 = 0;		
	for(int i = 0 ; i < n * 2 * 2 ; i += 2 * 2)
	{
		float theta1, theta2;
		inputFileHandler >> theta1 >> theta2;

			t1t2xy[i] = theta1;
		t1t2xy[i + 1] = theta2;
		#ifdef ActiveMasking
		maskVariable(&t1t2xy[i], posMask,negMask);
		maskVariable(&t1t2xy[i + 1], posMask,negMask);
		#endif
			
		SetMaxAndMin( t1t2xy[i],&maxValue1,&minValue1,&NumberOfBits1,StepSize1, i);
		SetMaxAndMin( t1t2xy[i + 1],&maxValue1,&minValue1,&NumberOfBits1,StepSize1, i+1);
		#ifdef Quantization
		Quantize(&t1t2xy[i],StepSize1,NumberOfBits1,i);
		Quantize(&t1t2xy[i + 1],StepSize1,NumberOfBits1,i+1);

			#endif
		if(std::abs(t1t2xy[i]-theta1)>maxAbsoluteError)
			{
				maxAbsoluteError=std::abs(t1t2xy[i]-theta1);
			}
		if(std::abs(t1t2xy[i+1]-theta2)>maxAbsoluteError)
			{
				maxAbsoluteError=std::abs(t1t2xy[i+1]-theta2);
			}
		forwardk2j(t1t2xy[i + 0], t1t2xy[i + 1], t1t2xy + (i + 2), t1t2xy + (i + 3));
		#ifdef ActiveMasking
		maskVariable(t1t2xy + (i + 2), posMask,negMask);
		maskVariable(t1t2xy + (i + 3), posMask,negMask);
		#endif
		SetMaxAndMin( t1t2xy[i+2],&maxValue1,&minValue1,&NumberOfBits1,StepSize1, i+2);
		SetMaxAndMin( t1t2xy[i + 3],&maxValue1,&minValue1,&NumberOfBits1,StepSize1, i+3);
		#ifdef Quantization
		Quantize(&t1t2xy[i+2],StepSize1,NumberOfBits1,i+2);
		Quantize(&t1t2xy[i +3],StepSize1,NumberOfBits1,i+3);
		#endif
	}

	for(int i = 0 ; i < n * 2 * 2 ; i += 2 * 2)
	{
		inversek2j(t1t2xy[i + 2], t1t2xy[i + 3], t1t2xy + (i + 0), t1t2xy + (i + 1));
		#ifdef ActiveMasking
		maskVariable(&t1t2xy[i], posMask,negMask);
		maskVariable(&t1t2xy[i + 1], posMask,negMask);
		maskVariable(t1t2xy + (i + 2), posMask,negMask);
		maskVariable(t1t2xy + (i + 3), posMask,negMask);
		#endif
		SetMaxAndMin( t1t2xy[i],&maxValue3,&minValue3, &NumberOfBits3,StepSize3, i);
		SetMaxAndMin( t1t2xy[i + 1],&maxValue3,&minValue3, &NumberOfBits3,StepSize3, i+1);
		SetMaxAndMin( t1t2xy[i+2],&maxValue3,&minValue3, &NumberOfBits3,StepSize3, i+2);
		SetMaxAndMin( t1t2xy[i + 3],&maxValue3,&minValue3, &NumberOfBits3,StepSize3, i+3);
		#ifdef Quantization
		Quantize(&t1t2xy[i],StepSize3,NumberOfBits3,i);
		Quantize(&t1t2xy[i +1],StepSize3,NumberOfBits3,i+1);
		Quantize(&t1t2xy[i+2],StepSize3,NumberOfBits3,i+2);
		Quantize(&t1t2xy[i + +3],StepSize3,NumberOfBits3,i+3);

		#endif
	}

	for(int i = 0 ; i < n * 2 * 2 ; i += 2 * 2)
	{
		outputFileHandler <<  t1t2xy[i+0] <<"\t"<< t1t2xy[i+1] << "\n";
	}

	inputFileHandler.close();
	outputFileHandler.close();

	free(t1t2xy) ; 
	t1t2xy = NULL ;
	//printf("Max1 is:  %f   , min1 is %f  \n", maxValue1,minValue1);
	//printf("MaxEXP is:  %d   , minEXP is %d  \n", maxExp,minExp);
	//printf("StepSize is %f \n",StepSize1 );
	//printf("Max2 is:  %f   , min2 is %f  \n", maxValue2,minValue2);

	//printf("Max3 is:  %f   , min3 is %f  \n", maxValue3,minValue3);
	//printf("Max4 is:  %f   , min3 is %f  \n", maxValue4,minValue4);
	EndOfSimulationReport(n*8);
	printf("  %f", maxAbsoluteError);
	return 0 ;
}
