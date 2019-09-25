/*
 * rgbimage.h
 * 
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */


#ifndef RGB_IMAGE_H_
#define RGB_IMAGE_H_
#include <iostream>
#include <fstream>
#include <sstream>
typedef struct {
   float r;
   float g;
   float b;
   int cluster;
   float distance;
} RgbPixel;

typedef struct {
   int w;
   int h;
   RgbPixel** pixels;
   char* meta;
} RgbImage;

void initRgbImage(RgbImage* image);
int loadRgbImage(const char* fileName, RgbImage* image, float scale);
int saveRgbImage(RgbImage* image, const char* fileName, float scale);
void freeRgbImage(RgbImage* image);

void makeGrayscale(RgbImage* rgbImage);
extern int originalVariableLength ;
extern int reducedVariableLength ;
extern int FileReadCycle;
extern float ratio;
extern FILE *Rangefp;
extern char rangeFilename[500];

#endif /* RGB_IMAGE_H_ */
