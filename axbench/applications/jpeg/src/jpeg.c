/*
 * jpeg.c
 * 
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>

#include "datatype.h"
#include "jpegconfig.h"
#include "prototype.h"

#include "rgbimage.h"


#define OUT_BUFFER_SIZE 500000 // in bytes
#define RangeGeneration

int main (int argc, const char* argv[]) {
	char inputFileName[300], outputFileName[300];
	UINT32 qualityFactor;
	UINT32 imageFormat;

	UINT8 *outputBuffer;
	FILE *fp;

	UINT8 *outputBufferPtr;

	strcpy(inputFileName, argv[1]);
	//strcpy(outputFileName, argv[2]);

	qualityFactor = 1024;
	imageFormat = GRAY;
	int rv;


	RgbImage srcImage;
	initRgbImage(&srcImage);
	if (loadRgbImage(inputFileName, &srcImage) == 0) {
		printf("Error! Oops: Cannot load the input image!\n");
		return -1;
	}
	//----------------------------------------

	#ifdef RangeGeneration
	FILE *Rangefp;
	char rangeFilename[300];
	strcpy(rangeFilename, argv[2]);
	
    Rangefp = fopen(rangeFilename, "w");
    if(Rangefp == NULL) {
      printf("ERROR: Unable to open file `%s'.\n", rangeFilename);
      exit(1);
    }

    rv = fprintf(Rangefp, "%i\n", (int)srcImage.h );
    if(rv < 0) {
      printf("ERROR: Unable to write to file `%s'.\n", rangeFilename);
      fclose(Rangefp);
      exit(1);
    }  
	for(int i=0; i < srcImage.h; i++) {
		rv = fprintf(Rangefp, "%lu   ", (long unsigned int)(&srcImage.pixels[i][0]));
		  if(rv < 0) {
			printf("ERROR: Unable to write to file `%s'.\n", rangeFilename);
			fclose(Rangefp);
			exit(1);
		}
		rv = fprintf(Rangefp, "%.lu\n", ((long)&srcImage.pixels[i][srcImage.w]+sizeof(RgbPixel)));
		  if(rv < 0) {
			printf("ERROR: Unable to write to file `%s'.\n", rangeFilename);
			fclose(Rangefp);
			exit(1);
		}   
	}
	rv = fclose(Rangefp);
	if(rv != 0) {
	  printf("ERROR: Unable to close file `%s'.\n", rangeFilename);
	  exit(1);
	}
	#endif
	//------------------------------------------
	makeGrayscale(&srcImage);

	outputBuffer = (UINT8 *) malloc(OUT_BUFFER_SIZE * sizeof(UINT8));


	outputBufferPtr = outputBuffer;
	outputBufferPtr = encodeImage(
		&srcImage, outputBufferPtr, qualityFactor, imageFormat
	);


	freeRgbImage(&srcImage);
	
	/*

	fp = fopen(outputFileName, "wb");
	if (fp != NULL) {
		fwrite(outputBuffer, 1, outputBufferPtr - outputBuffer, fp);
		fclose(fp);
	}
*/
	free(outputBuffer);
	return 0;
}

