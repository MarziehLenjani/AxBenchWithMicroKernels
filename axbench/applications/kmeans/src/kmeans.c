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
#include <string.h>
#include <cstdlib>
#define RangeGeneration
int originalVariableLength ;
int reducedVariableLength ;
int FileReadCycle;
float ratio=1;
FILE *Rangefp;
char rangeFilename[500];
int main (int argc, const char* argv[]) {

	RgbImage srcImage;
	Clusters clusters;

	initRgbImage(&srcImage);

	std::string inImageName  = argv[1];
	//std::string outImageName = argv[2];

	

	

	#ifdef RangeGeneration
	
	
	std::string rangeFilenameTemp=argv[2];
	originalVariableLength =atoi(argv[3]);
	reducedVariableLength =atoi(argv[4]);
	FileReadCycle =atoi(argv[5]);
	ratio= float(originalVariableLength)/float(reducedVariableLength);
	
	strcpy(rangeFilename,(char*)rangeFilenameTemp.c_str());
    Rangefp = fopen(rangeFilename, "w");
    if(Rangefp == NULL) {
      printf("ERROR: Unable to open file `%s'.\n", rangeFilename);
      exit(1);
    }

	#endif
	loadRgbImage(inImageName.c_str(), &srcImage, 256);
	initClusters(&clusters, 6, 1);
	segmentImage(&srcImage, &clusters, 1);
	//saveRgbImage(&srcImage, outImageName.c_str(), 255);


	freeRgbImage(&srcImage);
	freeClusters(&clusters);
	return 0;
}
