/*
 * rgb_image.cpp
 * 
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */

#include "rgb_image.hpp"
#include "../../shortvariable/shortVariable.hpp"
#include <cmath>
double maxAbsoluteError=0;
void Image::printPixel(int x, int y)
{
	std::cout << "# Red: 	" << this->pixels[x][y]->r << std::endl;
	std::cout << "# Green: 	" << this->pixels[x][y]->g << std::endl;
	std::cout << "# Blue: 	" << this->pixels[x][y]->b << std::endl; 
}

int Image::loadRgbImage(std::string filename)
{
	std::ifstream imageFile ;

	if(DEBUG)
		std::cout << "# Loading " << filename << " ..." << std::endl ;

	imageFile.open(filename.c_str()) ;
	if(! imageFile.is_open())
	{
		std::cerr << "# Error openning the file!" << std::endl ;
		return 0 ;
	}

	// Read first line and split based on the , and any spaces before or after
	std::string line ;
	std::getline(imageFile, line) ;
	std::vector<std::string> imageInfo ;
	boost::algorithm::split_regex(imageInfo, line, boost::regex("\t*,\t*")) ;
	this->width  = atoi(imageInfo[0].c_str()) ;
	this->height = atoi(imageInfo[1].c_str()) ;

	if(DEBUG)
	{
		std::cout << "# Width:  " << this->width ;
		std::cout << "# Height: " << this->height  << std::endl ;
	} 
	

	// We assume there is a newline after each row
	for (int h = 0 ; h < this->height ; h++)
	{
		std::getline(imageFile, line) ;
		std::vector<std::string> currRowString ;
		boost::algorithm::split_regex(currRowString, line, boost::regex("\t*,\t*")) ;

		std::vector<boost::shared_ptr<Pixel> > currRow ;

		for(int w = 0 ; w < this->width ; w++)
		{
			int index = w * 3 ;
			float r = atoi(currRowString[index++].c_str()) ;
			float g = atoi(currRowString[index++].c_str()) ;
			float b = atoi(currRowString[index++].c_str()) ;

			float temp1=r;
			float temp2=g;
			float temp3=b;
		#ifdef ActiveMasking
			maskVariable(&r, posMask,negMask);
			maskVariable(&g, posMask,negMask);
			maskVariable(&b, posMask,negMask);
		#endif
			SetMaxAndMin(r,&maxValue1,&minValue1,&NumberOfBits1,StepSize1,h* this->width*3+w*3);
			SetMaxAndMin(g,&maxValue1,&minValue1,&NumberOfBits1,StepSize1, h* this->width*3+w*3+1);
			SetMaxAndMin(b,&maxValue1,&minValue1,&NumberOfBits1,StepSize1, h* this->width*3+w*3+2);

		#ifdef Quantization
			//std::cout << "before quantize" <<r<<"\n";
			Quantize(&r,StepSize1,NumberOfBits1,h* this->width*3+w*3);
			//std::cout <<"after quantize"<< r<<"\n";i
			Quantize(&g,StepSize1,NumberOfBits1,h* this->width*3+w*3+1);
			Quantize(&b,StepSize1,NumberOfBits1,h* this->width*3+w*3+2);
		#endif
			if(std::abs(r-temp1)> maxAbsoluteError)
			{
				maxAbsoluteError=std::abs(r-temp1);
				
			}
			if(std::abs(g-temp2)> maxAbsoluteError)
			{
				maxAbsoluteError=std::abs(g-temp2);
			}
			if(std::abs(b-temp3)> maxAbsoluteError)
			{
				maxAbsoluteError=std::abs(b-temp3);
			}
			// Add pixel to the current row
			boost::shared_ptr<Pixel> currPixel(new Pixel(r, g, b)) ;
			currRow.push_back(currPixel) ;
		}
		this->pixels.push_back(currRow) ;
	}
	std::getline(imageFile, line) ;
	this->meta = line ;
	return 1 ;
}

int Image::saveRgbImage(std::string outFilename, float scale)
{
	if(DEBUG)
	{
		std::cout << "# Savint into " << outFilename << " ..." << std::endl ;
	}

	std::ofstream outFile ;
	outFile.open(outFilename.c_str()) ;

	outFile << this->width << "," << this->height << std::endl ;

	for(int h = 0 ; h < this->height ; h++)
	{
		for(int j = 0; j < (this->width); j++) {
#ifdef ActiveMasking
		maskVariable(&(this->pixels[h][j]->r), posMask,negMask);
		maskVariable(&(this->pixels[h][j]->g), posMask,negMask);
		maskVariable(&(this->pixels[h][j]->b), posMask,negMask);
#endif
		SetMaxAndMin( this->pixels[h][j]->r,&maxValue2,&minValue2,&NumberOfBits2,StepSize2,h* this->width*3+3*j);
		SetMaxAndMin( this->pixels[h][j]->g,&maxValue2,&minValue2,&NumberOfBits2,StepSize2,h* this->width*3+3*j+1);
		SetMaxAndMin(this->pixels[h][j]->b,&maxValue2,&minValue2,&NumberOfBits2,StepSize2,h* this->width*3+3*j+2);
#ifdef Quantization
		//std::cout << "stepsize: "<<StepSize1<<"number of bits "<<NumberOfBits<<"before quantize" <<this->pixels[h][j]->r<<"\n";
		Quantize(&(this->pixels[h][j]->r),StepSize2,NumberOfBits2,h* this->width*3+3*j);
		//std::cout << "before quantize" <<this->pixels[h][j]->r<<"\n";
		Quantize(&(this->pixels[h][j]->g),StepSize2,NumberOfBits2,h* this->width*3+3*j+1);
		Quantize(&(this->pixels[h][j]->b),StepSize2,NumberOfBits2,h* this->width*3+3*j+2);

#endif
		}
		for(int w = 0 ; w < (this->width - 1); w++)
		{
			// Write Red
			int red   = (int)(this->pixels[h][w]->r * scale) ;
			int green = (int)(this->pixels[h][w]->g * scale) ;
			int blue  = (int)(this->pixels[h][w]->b * scale) ;

			//if ( red > 255 )
		//		red = 255 ;
		//	if ( green > 255 )
		//		green = 255 ;
		///	if ( blue > 255 )
			//	blue = 255 ;
			outFile << red << "," ;
			// Write Green
			outFile << green << "," ;
			// Write Blue
			outFile << blue << "," ;
			
		}

		int red   = (int)(this->pixels[h][this->width - 1]->r * scale) ;
		int green = (int)(this->pixels[h][this->width - 1]->g * scale);
		int blue  = (int)(this->pixels[h][this->width - 1]->b * scale) ;


		// Write Red
		outFile << red  << "," ;
		// Write Green
		outFile << green << "," ;
		// Write Blue
		outFile << blue << std::endl ;
	}
	EndOfSimulationReport(2*this->width* this->height* 3);
	printf("  %f", maxAbsoluteError);
	// Print the meta information
	outFile << this->meta ;
	outFile.close() ;
	return 1 ;
}

void Image::makeGrayscale()
{

	float luminance ;

	float rC = 0.30 / 256.0 ;
	float gC = 0.59 / 256.0 ;
	float bC = 0.11 / 256.0 ;

	for(int h = 0 ; h < this->height ; h++)
	{
		for(int w = 0 ; w < this->width ; w++)
		{
			luminance = ( rC * this->pixels[h][w]->r ) + 
						( gC * this->pixels[h][w]->g ) + 
						( bC * this->pixels[h][w]->b ) ;

			this->pixels[h][w]->r = luminance ;
			this->pixels[h][w]->g = luminance ; 
			this->pixels[h][w]->b = luminance ; 
		}
	}
}
