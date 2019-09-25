/*
 * jpeg.c
 * 
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datatype.h"
#include "jpegconfig.h"
#include "prototype.h"

#include "rgbimage.h"
#include "shortVariable.hpp"


#define OUT_BUFFER_SIZE 500000 // in bytes


int main (int argc, const char* argv[]) {
	char inputFileName[32], outputFileName[32];
	UINT32 qualityFactor;
	UINT32 imageFormat;

	UINT8 *outputBuffer;
	FILE *fp;

	UINT8 *outputBufferPtr;

	strcpy(inputFileName, argv[1]);
	strcpy(outputFileName, argv[2]);

	//------------------Added Part
	#if defined(ActiveMasking) || defined(Quantization)
	NumberOfBits			= atoi(argv[3]);
	setTheMasks(NumberOfBits, &posMask, &negMask);
	#endif
	#ifdef Quantization
		StepSize1=	1/pow(2,NumberOfBits); //TODO:change 111
	#endif
	
	#ifdef IgnoreOulierQuantization
	StepSize1=	 pow(2,atoi(argv[4]));
	#endif
	#ifdef multiple_StepSize	
	//ratio= atof(argv[4]);
	ratio= 4;
	StepSize1=((1.0/ratio) 1.0*)/pow(2,NumberOfBits-2);//TODO:change 111
	#endif
	
	#if defined(ActiveMasking) || defined(Quantization)
	outputFilename=outputFilename;
	outputFilename=outputFilename+"_";
	outputFilename=outputFilename + std::to_string (NumberOfBits);	
	#endif
	outputFilename=outputFilename+".txt";
//-----------End of Aded part
	qualityFactor = 1024;
	imageFormat = GRAY;


	RgbImage srcImage;
	initRgbImage(&srcImage);
	if (loadRgbImage(inputFileName, &srcImage) == 0) {
		printf("Error! Oops: Cannot load the input image!\n");
		return -1;
	}

	makeGrayscale(&srcImage);

	outputBuffer = (UINT8 *) malloc(OUT_BUFFER_SIZE * sizeof(UINT8));


	outputBufferPtr = outputBuffer;
	outputBufferPtr = encodeImage(
		&srcImage, outputBufferPtr, qualityFactor, imageFormat
	);


	freeRgbImage(&srcImage);

	fp = fopen(outputFileName, "wb");
	if (fp != NULL) {
		fwrite(outputBuffer, 1, outputBufferPtr - outputBuffer, fp);
		fclose(fp);
	}
	free(outputBuffer);
	return 0;
}

