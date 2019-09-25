//#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
//#include <time.h>
#include "../../shortvariable/shortVariable.hpp"
#include <random>
#include <cmath>
static float* activations;
static float* weights;
static float* out ;
double maxAbsoluteError=0;


//convolution algorithm
float* conv(float *activations, float *weights, long n1, long n2, long f1, long f2, long s1, long s2, long channel,float* out  )
{	
	long kCenterX = f1 / 2;
	long kCenterY = f2 / 2;
	long rows=n1;
	long cols=n2;
	long kRows=f2;
	long kCols=f1;
		
	//f3
	long index1=0;
	long index2=0;
	
	for(long i=0; i < rows; i=i+s1)              // rows
	{
		long index2=0;
		for(long j=0; j < cols; j=j+s1)          // columns
		{
			out[index1*cols+index2]=0;
			for(long m=0; m < kRows; ++m)     // kernel rows
			{
				long mm = kRows - 1 - m;      // row index of flipped kernel
				
				for(long n=0; n < kCols; ++n) // kernel columns
				{
					long nn = kCols - 1 - n;  // column index of flipped kernel

					// index of input signal, used for checking boundary
					long ii = i + (m - kCenterY);
					long jj = j + (n - kCenterX);

					// ignore input samples which are out of bound
					if( ii >= 0 && ii < rows && jj >= 0 && jj < cols )
					{
						for(long ch=0; ch<channel;ch++ )
						{
							out[index1*cols+index2]+= activations[ch*cols*rows+ii*cols+jj]* weights[ch*kCols*kRows+mm*kCols+nn];
						}
					}
				}
				
			}
			index2++;

		}
		index1++;

	}
	return out;

}


int main(int argc, char* argv[])
{
	
	//first matrix dimensions
	long n1						= atoi(argv[1]);
	long n2						= atoi(argv[2]);
	
	
	//filter matrix dimensions
	long f1						= atoi(argv[3]);
	long f2						= atoi(argv[4]);
	
	long p1						= atoi(argv[5]);
	long p2						= atoi(argv[6]);
	
	long s1 					= atoi(argv[7]);
	long s2						= atoi(argv[8]);
	//number of channel
	long channel					= atoi(argv[9]);
	
	//number of filters
	long numFilt  				= atoi(argv[10]);
//	std::string inImageName  = argv[1];
	std::string outImageName = argv[11];
//#if defined(ActiveMasking) || defined(Quantization)
	fileNamingArg=atoi(argv[12]);
	float Max1=atof(argv[13]);
	float Max2=atof(argv[14]);
	float Max3=atof(argv[15]);
	int inputSign=atoi(argv[16]);
        NumberOfBits1                    = fileNamingArg;
        NumberOfBits2                    = fileNamingArg;
        NumberOfBits3                    = fileNamingArg;
  //      NumberOfBits4                    = fileNamingArg;
//#endif
	StepSize1= Max1/pow(2,NumberOfBits1);
	StepSize2= Max2/pow(2,NumberOfBits2);
	StepSize3= Max3/pow(2,NumberOfBits3);
//#if defined(ActiveMasking) || defined(Quantization)
	outImageName=outImageName;
	outImageName=outImageName+"_";
	outImageName=outImageName + std::to_string (fileNamingArg);
//	std::cout<<"file name is "<<outImageName<<std::endl;
//#endif
	outImageName=outImageName+".txt";

        #ifdef Sampling
        StepSize1=       pow(2,atoi(argv[17]));
        StepSize2=       pow(2,atoi(argv[18]));
        StepSize3=       pow(2,atoi(argv[19]));
        StepSize4=       pow(2,atoi(argv[20]));
        NumberOfSamplingItr= SamplingItrUnit * fileNamingArg ;
        #endif
//	std::cout<<"n1 is "<<n1<<" n2 is "<<n2<<" f1 is :"<< f1<<" f2 is :"<<f2 <<" p1 is "<<p1<<" p2 is "<<p2 <<" s1 is : "<<s1<<"  s2 is :"<<s2<<" "<<std::endl;
//	std::cout<<"file name is "<<outImageName<<" "<<Max1<<" "<<Max2<<" "<<Max3<<" "<<inputSign<<" "<<std::endl;
	setTheMasks(fileNamingArg, &posMask, &negMask);		
	n1=n1+p1;
	n2=n2+p2;

	long outRow=n1/s1;
	long outCol=n2/s2;
	StartOfSimulationReport();

	// create the arrays
	activations 		= (float*)malloc(channel*n1*n2 * sizeof (float));
	weights 		= (float*)malloc(numFilt *channel*f1 *f2  * sizeof (float));
	out 		= (float*)malloc(numFilt*outRow*outCol * sizeof (float));
	if(activations == NULL || weights == NULL || out==NULL )
	{
		std::cout << "cannot allocate memory for the triangle coordinates!" << std::endl;
		return -1 ;
	}
	
/*
	//----------------------------------------
	std::string rangeFilename 	= argv[11];
	std::ofstream rangeFileHandler;
	rangeFileHandler.open(rangeFilename);
	rangeFileHandler <<3<< std::endl;
	rangeFileHandler <<long(&activations [0])<< " " << long(&activations[channel*n1*n2 ]) << std::endl;
	rangeFileHandler << long(&weights[0] )<< " " << long(&weights[numFilt *channel*f1 *f2  ]) << std::endl;
	rangeFileHandler << long(&out[0] )<< " " << long(&out[numFilt*outRow*outCol] ) << std::endl;
	rangeFileHandler.flush();
	rangeFileHandler.close();
	//-------------------------------------------	
*/
	int dim1, dim2, dim3;

	


std::default_random_engine ActivationGenerator(1024);
std::normal_distribution<double> distribution1(0,1);
//access data to bring data to cache
	for(long ch=0; ch<channel;ch++ )
	{
		for(long i = 0;i < n1 ; i++)
		{
			for(long j = 0;j < n2 ; j++)
			{
				float ftemp=distribution1(ActivationGenerator)/(8.0/Max1);
				if(ftemp<0)
				{
					ftemp=inputSign*ftemp;
				}
			float temp=ftemp;
			#ifdef ActiveMasking
				maskVariable(&ftemp, posMask,negMask);
			#endif
				SetMaxAndMin( ftemp,&maxValue1,&minValue1,&NumberOfBits1,StepSize1, ch*n1*n2+i*n2+j);
			#ifdef Quantization
				Quantize(&ftemp,StepSize1,NumberOfBits1,ch*n1*n2+i*n2+j);
			#endif
				activations[ch*n1*n2+i*n2+j]=ftemp; //something to fill the matrix
				if(std::abs(ftemp-temp)>maxAbsoluteError)
				{
					maxAbsoluteError=std::abs(ftemp-temp);
				}					
				
				
			}
			
		}
		
	}
 std::default_random_engine WeightGenerator(2048);
 std::normal_distribution<double> distribution2(0,1);
	for (long fCount=0 ; fCount< numFilt; fCount++)
	{
		for(long ch=0; ch<channel;ch++ )
		{
			for(long i = 0;i < f1 ; i++)
			{
				for(long j = 0;j < f2 ; j++)
				{
					float ftemp=distribution2(ActivationGenerator)/(8.0/Max2);
				#ifdef ActiveMasking
					maskVariable(&ftemp, posMask,negMask);
				#endif
					SetMaxAndMin( ftemp,&maxValue2,&minValue2,&NumberOfBits2,StepSize2,fCount*channel*f1*f2+ch*f1*f2+i*f2+j);
				#ifdef Quantization
					Quantize(&ftemp,StepSize2,NumberOfBits2,fCount*channel*f1*f2+ch*f1*f2+i*f2+j);
				#endif
					weights[fCount*channel*f1*f2+ch*f1*f2+i*f2+j]=ftemp;
						
							
					}
						
				
			}
		}
	}
	for (long fCount=0 ; fCount< numFilt; fCount++)
	{
		conv(activations, &weights[fCount*channel*f1*f2], n1, n2, f1, f2, s1,s2,channel,&out[fCount*outRow*outCol]);
	}

	std::ofstream outputFileHandler;
	outputFileHandler.open(outImageName);
	outputFileHandler.precision(10);

	
	
	for (long fCount=0 ; fCount< numFilt; fCount++)
	{
		for(long i = 0;i < outRow ; i++)
		{
			for(long j = 0;j < (outCol-1) ; j++)
			{
			#ifdef ActiveMasking
				maskVariable(&out[fCount*outRow*outCol+i*outCol+j], posMask,negMask);
			#endif
				SetMaxAndMin( out[fCount*outRow*outCol+i*outCol+j],&maxValue3,&minValue3,&NumberOfBits3,StepSize3,fCount*outRow*outCol+i*outCol+j);
			#ifdef Quantization
				Quantize(&out[fCount*outRow*outCol+i*outCol+j],StepSize3,NumberOfBits3,fCount*outRow*outCol+i*outCol+j);
			#endif

				//std::cout<<out[fCount][i][j]<<" "	;			
				outputFileHandler << out[fCount*outRow*outCol+i*outCol+j] << ", " ;
					
			}
			#ifdef ActiveMasking
				maskVariable(&out[fCount*outRow*outCol+i*outCol+outCol-1], posMask,negMask);
			#endif
				SetMaxAndMin( out[fCount*outRow*outCol+i*outCol+outCol-1],&maxValue3,&minValue3,&NumberOfBits3,StepSize3,fCount*outRow*outCol+i*outCol+outCol-1);
			#ifdef Quantization
				Quantize(&out[fCount*outRow*outCol+i*outCol+outCol-1],StepSize3,NumberOfBits3,fCount*outRow*outCol+i*outCol+outCol-1);
			#endif
				outputFileHandler <<out[fCount*outRow*outCol+i*outCol+(outCol-1) ] <<std::endl;			
		}

	}
	
	outputFileHandler.flush();
	outputFileHandler.close();
//	std::cout<<"max1:" <<maxValue1 <<" max2: "<<maxValue2<<" max3: "<<maxValue3;
//	std::cout<<"min1:" <<minValue1 <<" min2: "<<minValue2<<" min3: "<<minValue3;
	EndOfSimulationReport(channel*n1*n2+numFilt *channel*f1 *f2 +numFilt*outRow*outCol);
	printf("  %f", maxAbsoluteError);
	return 0 ;
}

