/*
 * rgbimage.c
 * 
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */


#include "rgbimage.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../shortvariable/shortVariable.hpp"
#include <cmath>
double maxAbsoluteError=0;
void initRgbImage(RgbImage* image) {
	image->w = 0;
	image->h = 0;
	image->pixels = NULL;
	image->meta = NULL;
}

int readCell(FILE *fp, char* w) {
	int c;
	int i;

	w[0] = 0;
	for (c = fgetc(fp), i = 0; c != EOF; c = fgetc(fp)) {
		if (c == ' ' || c == '\t') {
			if (w[0] != '\"')
				continue;
		}

		if (c == ',' || c == '\n') {
			if (w[0] != '\"')
				break;
			else if (c == '\"') {
				w[i] = c;
				i++;
				break;
			}
		}

		w[i] = c;
		i++;
	}
	w[i] = 0;

	return c;
}

int loadRgbImage(const char* fileName, RgbImage* image, float scale) {
	int c;
	int i;
	int j;
	char w[256];
	RgbPixel** pixels;
	FILE *fp;

	//printf("Loading %s ...\n", fileName);

	fp = fopen(fileName, "r");
	if (!fp) {
		printf("Warning: Oops! Cannot open %s!\n", fileName);
		return 0;
	}

	c = readCell(fp, w);
	image->w = atoi(w);
	c = readCell(fp, w);
	image->h = atoi(w);

	//printf("%d x %d\n", image->w, image->h);

	pixels = (RgbPixel**)malloc(image->h * sizeof(RgbPixel*));

	if (pixels == NULL) {
		printf("Warning: Oops! Cannot allocate memory for the pixels!\n");

		fclose(fp);

		return 0;
	}

	c = 0;
	for(i = 0; i < image->h; i++) {
		pixels[i] = (RgbPixel*)malloc(image->w * sizeof(RgbPixel));
		if (pixels[i] == NULL) {
			c = 1;
			break;
		}
	}

	if (c == 1) {
		printf("Warning: Oops! Cannot allocate memory for the pixels!\n");

		for (i--; i >= 0; i--)
			free(pixels[i]);
		free(pixels);

		fclose(fp);

		return 0;
	}

	for(i = 0; i < image->h; i++) {
		for(j = 0; j < image->w; j++) {
			c = readCell(fp, w);
			pixels[i][j].r = atoi(w) / scale;
			float temp1=pixels[i][j].r;

			c = readCell(fp, w);
			pixels[i][j].g = atoi(w) / scale;
			float temp2=pixels[i][j].g;
			c = readCell(fp, w);
			pixels[i][j].b = atoi(w) / scale;
			float temp3=pixels[i][j].b;
		#ifdef ActiveMasking
			maskVariable(&pixels[i][j].r, posMask,negMask);
			maskVariable(&pixels[i][j].g, posMask,negMask);
			maskVariable(&pixels[i][j].b, posMask,negMask);
		#endif
			SetMaxAndMin( pixels[i][j].r,&maxValue1,&minValue1,&NumberOfBits1,StepSize1, i*(image->w*3)+j*3) ;
			SetMaxAndMin( pixels[i][j].g,&maxValue1,&minValue1,&NumberOfBits1,StepSize1, i*(image->w*3)+j*3+1);
			SetMaxAndMin( pixels[i][j].b,&maxValue1,&minValue1,&NumberOfBits1,StepSize1, i*(image->w*3)+j*3+2);
		#ifdef Quantization
			Quantize(&pixels[i][j].r,StepSize1,NumberOfBits1,i*(image->w*3)+j*3);
			Quantize(&pixels[i][j].g,StepSize1,NumberOfBits1,i*(image->w*3)+j*3+1);
			Quantize(&pixels[i][j].b,StepSize1,NumberOfBits1,i*(image->w*3)+j*3+2);
		#endif
			if(std::abs(temp1-pixels[i][j].r)>maxAbsoluteError)
			{
				maxAbsoluteError=std::abs(temp1-pixels[i][j].r);
			}
			if(std::abs(temp2-pixels[i][j].g)>maxAbsoluteError)
			{
				maxAbsoluteError=std::abs(temp2-pixels[i][j].g);
			}
			if(std::abs(temp3-pixels[i][j].b)>maxAbsoluteError)
			{
				maxAbsoluteError=std::abs(temp3-pixels[i][j].b);
			}
			pixels[i][j].cluster = 0;
			pixels[i][j].distance = 0.;
		}
	}
	image->pixels = pixels;

	c = readCell(fp, w);
	image->meta = (char*)malloc(strlen(w) * sizeof(char));
	if(image->meta == NULL) {
		printf("Warning: Oops! Cannot allocate memory for the pixels!\n");

		for (i = 0; i < image->h; i++)
			free(pixels[i]);
		free(pixels);

		fclose(fp);

		return 0;

	}
	strcpy(image->meta, w);

	//printf("%s\n", image->meta);

	//printf("w=%d x h=%d\n", image->w, image->h);

	return 1;
}

int saveRgbImage(RgbImage* image, const char* fileName, float scale) {
	int i;
	int j;
	FILE *fp;

	//printf("Saving %s ...\n", fileName);

	fp = fopen(fileName, "w");
	if (!fp) {
		printf("Warning: Oops! Cannot open %s!\n", fileName);
		return 0;
	}

	fprintf(fp, "%d,%d\n", image->w, image->h);
	//printf("%d,%d\n", image->w, image->h);

	for(i = 0; i < image->h; i++) {
		for(j = 0; j < (image->w); j++) {
		#ifdef ActiveMasking
			maskVariable(&image->pixels[i][j].r, posMask,negMask);
			maskVariable(&image->pixels[i][j].g, posMask,negMask);
			maskVariable(&image->pixels[i][j].b, posMask,negMask);
		#endif
			SetMaxAndMin( image->pixels[i][j].r,&maxValue2,&minValue2,&NumberOfBits2,StepSize2,i*(image->w*3)+j*3);
			SetMaxAndMin( image->pixels[i][j].g,&maxValue2,&minValue2,&NumberOfBits2,StepSize2,i*(image->w*3)+j*3+1);
			SetMaxAndMin( image->pixels[i][j].b,&maxValue2,&minValue2,&NumberOfBits2,StepSize2,i*(image->w*3)+j*3+2);
		#ifdef Quantization
			Quantize(&image->pixels[i][j].r,StepSize2,NumberOfBits2,i*(image->w*3)+j*3);
			Quantize(&image->pixels[i][j].g,StepSize2,NumberOfBits2,i*(image->w*3)+j*3+1);
			Quantize(&image->pixels[i][j].b,StepSize2,NumberOfBits2,i*(image->w*3)+j*3+2);
		#endif
		}
		for(j = 0; j < (image->w - 1); j++) {
			fprintf(fp, "%d,%d,%d,", int(image->pixels[i][j].r * scale), int(image->pixels[i][j].g * scale), int(image->pixels[i][j].b * scale));
		}
		fprintf(fp, "%d,%d,%d\n", int(image->pixels[i][j].r * scale), int(image->pixels[i][j].g * scale), int(image->pixels[i][j].b * scale));
	}
	EndOfSimulationReport(2*(image->w*3)*image->w);
	printf("  %f", maxAbsoluteError);
	fprintf(fp, "%s", image->meta);
	//printf("%s\n", image->meta);
	
	fclose(fp);

	return 1;
}

void freeRgbImage(RgbImage* image) {
	int i;

	if (image->meta != NULL)
		free(image->meta);

	if (image->pixels == NULL)
		return;

	for (i = 0; i < image->h; i++)
		if (image->pixels != NULL)
			free(image->pixels[i]);

	free(image->pixels);
}

void makeGrayscale(RgbImage* image) {
	int i;
	int j;
	float luminance;

	float rC = 0.30;
	float gC = 0.59;
	float bC = 0.11;

	for(i = 0; i < image->h; i++) {
		for(j = 0; j < image->w; j++) {
			luminance =
				rC * image->pixels[i][j].r +
				gC * image->pixels[i][j].g +
				bC * image->pixels[i][j].b;

			image->pixels[i][j].r = (unsigned char)luminance;
			image->pixels[i][j].g = (unsigned char)luminance;
			image->pixels[i][j].b = (unsigned char)luminance;
		}
	}
}
