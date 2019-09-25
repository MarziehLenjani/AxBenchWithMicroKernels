/*
 * kmeans.c
 * 
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */


#include <stdio.h>
#include <string>
#include "rgbimage.h"
#include "segmentation.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "../../shortvariable/shortVariable.hpp"
int main (int argc, const char* argv[]) {

	RgbImage srcImage;
	Clusters clusters;

	initRgbImage(&srcImage);

	std::string inImageName  = argv[1];
	std::string outImageName = argv[2];
//	#if defined(ActiveMasking) || defined(Quantization)
		fileNamingArg=atoi(argv[3]);
		NumberOfBits1=fileNamingArg;
		NumberOfBits2=fileNamingArg;
		//NumberOfBits3=fileNamingArg;
		//NumberOfBits4=fileNamingArg;

//	#endif
	StepSize1= 1/pow(2,NumberOfBits1);
	StepSize2= 1/pow(2,NumberOfBits2);
	//StepSize3= 1/pow(2,NumberOfBits3);
	//StepSize4= 1/pow(2,NumberOfBits4);

        #ifdef Sampling
        StepSize1=       pow(2,atoi(argv[4]));
        StepSize2=       pow(2,atoi(argv[5]));
        //StepSize3=       pow(2,atoi(argv[6]));//not used
        //StepSize4=       pow(2,atoi(argv[7]));//not used
	NumberOfSamplingItr= SamplingItrUnit * fileNamingArg ;
        #endif
//	#if defined(ActiveMasking) || defined(Quantization)
		outImageName=outImageName;
		outImageName=outImageName+"_";
		outImageName=outImageName + std::to_string (fileNamingArg);
//	#endif
	outImageName=outImageName+".txt";
	setTheMasks(fileNamingArg, &posMask, &negMask);
	loadRgbImage(inImageName.c_str(), &srcImage, 256);
	StartOfSimulationReport();
	initClusters(&clusters, 6, 1);
	segmentImage(&srcImage, &clusters, 1);
	
	saveRgbImage(&srcImage, outImageName.c_str(), 255);
//	std::cout << "maxValue2: "<<maxValue2<<" minvalue2"<<minValue2<<"\n";

	freeRgbImage(&srcImage);
	freeClusters(&clusters);
	return 0;
}

