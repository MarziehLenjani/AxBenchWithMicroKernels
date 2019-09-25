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


#define PI 3.141592653589

#include <boost/math/special_functions/fpclassify.hpp>

#if defined(SWQNT) || defined(SWQNTInPlace)
 float SWQntstepsize=0.0125;
 float mid=5.5;
 int TmpSwQnt=0;
 float* SWQ_t1t2xy;
#endif


int main(int argc, const char* argv[])
{

	int n;
	std::string inputFilename	= argv[1];

	//std::string outputFilename 	= argv[2];

	// prepare the output file for writting the theta values
	//std::ofstream outputFileHandler;
	//outputFileHandler.open(outputFilename.c_str());

	// prepare the input file for reading the theta data
	std::ifstream inputFileHandler (inputFilename.c_str(), std::ifstream::in);

	// first line defins the number of enteries
	inputFileHandler >> n;

	float ratio=1;
	#ifdef SWQNT
		SWQ_t1t2xy=(float*)malloc(n * 2 * 2 * sizeof(float));
	#endif
	float* t1t2xy = (float*)malloc(n * 2 * 2 * sizeof(float));
		//----------------------------------------
	std::string rangeFilename 	= argv[2];
	std::ofstream rangeFileHandler;
	rangeFileHandler.open(rangeFilename);
	rangeFileHandler <<1<< std::endl;
	rangeFileHandler <<(long)(&t1t2xy[0])<< " " << (long)(&t1t2xy[n * 2 * 2 ]) << std::endl;
	rangeFileHandler.flush();
	rangeFileHandler.close();
	//-------------------------------------------
	int originalVariableLength =atoi(argv[3]);
	int reducedVariableLength =atoi(argv[4]);
	int FileReadCycle =atoi(argv[5]);
	ratio= float(originalVariableLength)/float(reducedVariableLength);
	if(t1t2xy == NULL)
	{
		std::cerr << "# Cannot allocate memory for the coordinates an angles!" << std::endl;
		return -1 ;
	}

	srand (time(NULL));

	int curr_index1 = 0;	
	//warmup
	//exit(0);
	for(int i = 0 ; i < n * 2 * 2 ; i += 2 * 2)
	{
		float theta1, theta2;
		if(i<((n * 2 * 2)/ratio) &&( (FileReadCycle==0)||( i<FileReadCycle)))
		{
			inputFileHandler >> theta1 >> theta2;
		}

		t1t2xy[i] = theta1;
		t1t2xy[i + 1] = theta2;
		#ifdef SWQNTInPlace
			TmpSwQnt+=(theta1+mid)/SWQntstepsize;
			TmpSwQnt+=(theta2+mid)/SWQntstepsize;
		#endif

		forwardk2j(t1t2xy[i + 0], t1t2xy[i + 1], t1t2xy + (i + 2), t1t2xy + (i + 3));
	}

	for(int i = 0 ; i < n * 2 * 2 ; i += 2 * 2)
	{
		inversek2j(t1t2xy[i + 2], t1t2xy[i + 3], t1t2xy + (i + 0), t1t2xy + (i + 1));
	}
/*
	for(int i = 0 ; i < n * 2 * 2 ; i += 2 * 2)
	{
		outputFileHandler <<  t1t2xy[i+0] << "\t" << t1t2xy[i+1] << "\n";
	}
	*/

	inputFileHandler.close();
	//utputFileHandler.close();

	free(t1t2xy) ; 
	t1t2xy = NULL ;

	return 0 ;
}
