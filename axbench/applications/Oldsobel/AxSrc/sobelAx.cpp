/*
 * sobel.cpp
 * 
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */
#include "rgb_image.hpp"
#include "convolution.hpp"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include "../../shortvariable/shortVariable.hpp"
//#ifdef NPU_FANN
// #include "floatfann.h"
//#endif

int main ( int argc, const char* argv[])
{
	int x, y;
	float s = 0;

	// Source and destination image	
	boost::shared_ptr<Image> srcImagePtr(new Image());
	boost::shared_ptr<Image> dstImagePtr(new Image());
	std::string outImageName = argv[2];
//#if defined(ActiveMasking) || defined(Quantization)
	fileNamingArg=atoi(argv[3]);
	NumberOfBits1=fileNamingArg;
	NumberOfBits2=fileNamingArg;
	//NumberOfBits3=fileNamingArg;
	//NumberOfBits4=fileNamingArg;
//#endif 
	StepSize1= 256/pow(2,NumberOfBits1);
	StepSize2=0.707107/pow(2,NumberOfBits2);

       #ifdef Sampling
        StepSize1=       pow(2,atoi(argv[4]));
        StepSize2=       pow(2,atoi(argv[5]));
        StepSize3=       pow(2,atoi(argv[6]));
        StepSize4=       pow(2,atoi(argv[7]));
        NumberOfSamplingItr= SamplingItrUnit * fileNamingArg ;
        #endif

//#if defined(ActiveMasking) || defined(Quantization)
	outImageName=outImageName;
	outImageName=outImageName+"_";
	outImageName=outImageName + std::to_string (fileNamingArg);
//#endif
	outImageName=outImageName+".txt";
	setTheMasks(fileNamingArg, &posMask, &negMask);
	StartOfSimulationReport();
	float w[][3] = {
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
	};


	srcImagePtr->loadRgbImage( argv[1] ); // source image
	dstImagePtr->loadRgbImage( argv[1] ); // destination image

	srcImagePtr->makeGrayscale( ); // convert the source file to grayscale

	y = 0 ;
	
	// Start performing Sobel operation
	for( x = 0 ; x < srcImagePtr->width ; x++ ) {
		HALF_WINDOW(srcImagePtr, x, y, w) ;


			s = sobel(w);


		dstImagePtr->pixels[y][x]->r = s ;
		dstImagePtr->pixels[y][x]->g = s ;
		dstImagePtr->pixels[y][x]->b = s ;
	}

	for (y = 1 ; y < (srcImagePtr->height - 1) ; y++) {
		x = 0 ;
		HALF_WINDOW(srcImagePtr, x, y, w);

			s = sobel(w);

	
		dstImagePtr->pixels[y][x]->r = s ;
		dstImagePtr->pixels[y][x]->g = s ;
		dstImagePtr->pixels[y][x]->b = s ;


		for( x = 1 ; x < srcImagePtr->width - 1 ; x++ ) {
			WINDOW(srcImagePtr, x, y, w) ;
				
				s = sobel(w);

			dstImagePtr->pixels[y][x]->r = s ;
			dstImagePtr->pixels[y][x]->g = s ;
			dstImagePtr->pixels[y][x]->b = s ;

		}

		x = srcImagePtr->width - 1 ;
		HALF_WINDOW(srcImagePtr, x, y, w) ;
		

			s = sobel(w);

		dstImagePtr->pixels[y][x]->r = s ;
		dstImagePtr->pixels[y][x]->g = s ;
		dstImagePtr->pixels[y][x]->b = s ;
	}

	y = srcImagePtr->height - 1;

	for(x = 0 ; x < srcImagePtr->width ; x++) {
		HALF_WINDOW(srcImagePtr, x, y, w) ;
		
			s = sobel(w);

		dstImagePtr->pixels[y][x]->r = s ;
		dstImagePtr->pixels[y][x]->g = s ;
		dstImagePtr->pixels[y][x]->b = s ;

	}

	dstImagePtr->saveRgbImage(outImageName, std::sqrt(256 * 256 + 256 * 256)) ;
//	std::cout << "maxValue2: "<<maxValue2<<" minvalue2"<<minValue2<<"\n";

	
	return 0 ;
}
