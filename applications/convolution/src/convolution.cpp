//#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
//#include <time.h>


static float* activations;
static float* weights;
static float* out ;


#if defined(SWQNT) || defined(SWQNTInPlace)
float SWQntstepsize=0.0125;
float mid=5.5;
int TmpSwQnt=0;
static float* SWQntactivations;
static float* SWQntweights;
static float* SWQntout ;
#endif
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
							 #if defined(SWQNT) || defined(SWQNTInPlace)
							out[index1*cols+index2]+= activations[ch*cols*rows+ii*cols+jj]* weights[ch*kCols*kRows+mm*kCols+nn]*SWQntstepsize*SWQntstepsize+mid;
							#endif

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
		
	n1=n1+p1;
	n2=n2+p2;

	long outRow=n1/s1;
	long outCol=n2/s2;


	// create the arrays
	activations 		= (float*)malloc(channel*n1*n2 * sizeof (float));
	weights 		= (float*)malloc(numFilt *channel*f1 *f2  * sizeof (float));
	out 		= (float*)malloc(numFilt*outRow*outCol * sizeof (float));
#ifdef SWQNT
	SWQntactivations = (float*)malloc(channel*n1*n2 * sizeof (float));
	SWQntweights= (float*)malloc(numFilt *channel*f1 *f2  * sizeof (float));
	SWQntout= (float*)malloc(numFilt*outRow*outCol * sizeof (float));
#endif

	if(activations == NULL || weights == NULL || out==NULL )
	{
		std::cout << "cannot allocate memory for the triangle coordinates!" << std::endl;
		return -1 ;
	}
	
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
	int dim1, dim2, dim3;

	
#ifndef VeryVeryShort
	for(long ch=0; ch<channel;ch++ )
	{
		for(long i = 0;i < n1 ; i++)
		{
			for(long j = 0;j < n2 ; j++)
			{
				
					activations[ch*n1*n2+i*n2+j]=1.5; //something to fill the matrix
					#ifdef SWQNTInPlace
						activations[ch*n1*n2+i*n2+j]=(activations[ch*n1*n2+i*n2+j]-mid)/SWQntstepsize;
					#endif
					
				
				
			}
			
		}
		
	}
	for (long fCount=0 ; fCount< numFilt; fCount++)
	{
		for(long ch=0; ch<channel;ch++ )
		{
			for(long i = 0;i < f1 ; i++)
			{
				for(long j = 0;j < f2 ; j++)
				{
					
					weights[fCount*channel*f1*f2+ch*f1*f2+i*f2+j]=1.5; //something to fill the matrix		
					#ifdef SWQNTInPlace
					weights[fCount*channel*f1*f2+ch*f1*f2+i*f2+j]=(weights[fCount*channel*f1*f2+ch*f1*f2+i*f2+j]-mid)/SWQntstepsize;//something to fill the matrix				
					#endif
				}
					
				
			}
		}
	}
        #ifdef SWQNT
	//access data to bring data to cache
	for(long ch=0; ch<channel;ch++ )
	{
		for(long i = 0;i < n1 ; i++)
		{
			for(long j = 0;j < n2 ; j++)
			{
				
				SWQntactivations[ch*n1*n2+i*n2+j]=(activations[ch*n1*n2+i*n2+j]-mid)/SWQntstepsize; //something to fill the matrix
					
				
				
			}
			
		}
		
	}
	for (long fCount=0 ; fCount< numFilt; fCount++)
	{
		for(long ch=0; ch<channel;ch++ )
		{
			for(long i = 0;i < f1 ; i++)
			{
				for(long j = 0;j < f2 ; j++)
				{
					
					SWQntweights[fCount*channel*f1*f2+ch*f1*f2+i*f2+j]=(weights[fCount*channel*f1*f2+ch*f1*f2+i*f2+j]-mid)/SWQntstepsize; //something to fill the matrix				
					
				}
					
				
			}
		}
	}
	#endif
#endif
#ifndef VeryVeryShort
	for (long fCount=0 ; fCount< numFilt; fCount++)
	{
		conv(activations, &weights[fCount*channel*f1*f2], n1, n2, f1, f2, s1,s2,channel,&out[fCount*outRow*outCol]);
	}
	#ifdef SWQNT
	for (long fCount=0 ; fCount< numFilt; fCount++)
	{
		for(long i = 0;i < outRow ; i++)
		{
			for(long j = 0;j < outCol ; j++)
			{
				
				SWQntout[fCount*outRow*outCol+i*outRow+j] =(out[fCount*outRow*outCol+i*outRow+j]+mid)*SWQntstepsize;
			}
		}
	}
	#endif
#endif
	return 0 ;
}

