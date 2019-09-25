#include <cstdio>
#include <iostream>
#include "fourier.hpp"
#include <fstream>
#include <time.h>

static int* indices;
static Complex* x;
static Complex* f;

int main(int argc, char* argv[])
{
	int i ;

	int n 						= atoi(argv[1]);


	//std::string outputFilename 	= argv[2];

	//prepare the output file for writting the theta values
	//std::ofstream outputFileHandler;
	//outputFileHandler.open(outputFilename);
	//outputFileHandler.precision(5);

	// create the arrays
	x 		= (Complex*)malloc(n * sizeof (Complex));
	f 		= (Complex*)malloc(n * sizeof (Complex));
	indices = (int*)malloc(n * sizeof (int));
	
	//----------------------------------------
	std::string rangeFilename 	= argv[2];
	std::ofstream rangeFileHandler;
	rangeFileHandler.open(rangeFilename);
	rangeFileHandler <<2<< std::endl;
	rangeFileHandler <<long(&x[0])<< " " << long(&x[n]) << std::endl;
	rangeFileHandler << long(&f[0] )<< " " << long(&f[n]) << std::endl;
	rangeFileHandler.flush();
	rangeFileHandler.close();
	//-------------------------------------------
	//warmup
	//exit(0);

	if(x == NULL || f == NULL || indices == NULL)
	{
		std::cout << "cannot allocate memory for the triangle coordinates!" << std::endl;
		return -1 ;
	}

	int K = n;

	for(i = 0;i < K ; i++)
	{
		x[i].real = i;
		x[i].imag = 0 ;
	}
	radix2DitCooleyTykeyFft(K, indices, x, f) ;
	
	//for(i = 0;i < K ; i++)
	//{
	//	outputFileHandler << f[i].real << " " << f[i].imag << std::endl;
	//}


	//outputFileHandler.close();

	return 0 ;
}