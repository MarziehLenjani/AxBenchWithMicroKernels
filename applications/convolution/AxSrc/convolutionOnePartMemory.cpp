//#include <cstdio>
#include <cstdlib>
#include <iostream>
//#include <fstream>
//#include <time.h>


static float*** activations;
static float**** weights;
static float*** out ;


//convolution algorithm
float** conv(float ***activations, float ***weights, long n1, long n2, long f1, long f2, long s1, long s2, long channel,float** out  )
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
			out[index1][index2]=0;
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
							out[index1][index2]+= activations[ch][ii][jj]* weights[ch][mm][nn];
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
	activations 		= (float***)malloc(channel*n1*n2 * sizeof (float**));
	weights 		= (float****)malloc(numFilt *channel*f1 *f2  * sizeof (float***));
	out 		= (float***)malloc(numFilt*outRow*outCol * sizeof (float**));
	if(activations == NULL || weights == NULL || out==NULL )
	{
		std::cout << "cannot allocate memory for the triangle coordinates!" << std::endl;
		return -1 ;
	}
	
	
	int dim1, dim2, dim3;
	
	 activations = (float ***)malloc(channel*sizeof(float**));
	 
	for(long ch=0; ch<channel;ch++ )
	{		
		 activations[ch] = (float **) malloc(n1*sizeof(float *));
		
		if(activations[ch] ==NULL)
		{
			std::cout << "cannot allocate memory for the triangle coordinates!" << std::endl;
			return -1 ;
		}

		 for (long i = 0; i < n1; i++) {

			activations[ch][i] = (float *)malloc(n2*sizeof(float));
			if(activations[ch][i] == NULL  )
			{
				std::cout << "cannot allocate memory for the triangle coordinates!" << std::endl;
				return -1 ;
			}

		}		
	}
	for (long fCount=0 ; fCount< numFilt; fCount++)
	{
		weights[fCount] = (float ***) malloc(channel*sizeof(float **));
		if(weights[fCount] == NULL  )
		{
			std::cout << "cannot allocate memory for the triangle coordinates!" << std::endl;
			return -1 ;
		}


		for(long ch=0; ch<channel;ch++ )
		{		
		 	weights[fCount][ch] = (float **) malloc(f1*sizeof(float *));
			if(weights[fCount][ch] == NULL  )
			{
				std::cout << "cannot allocate memory for the triangle coordinates!" << std::endl;
				return -1 ;
			}


		 	for (long i = 0; i < f1; i++) {

				weights[fCount][ch][i] = (float *)malloc(f2*sizeof(float));
				if(weights[fCount][ch][i]==NULL )
				{
					std::cout << "cannot allocate memory for the triangle coordinates!" << std::endl;
					return -1 ;
				}

			}		
		}
	}
	
	
	for (long fCount=0 ; fCount< numFilt; fCount++)
	{		
		 out[fCount] = (float **) malloc(outRow*sizeof(float *));
		if(out[fCount] ==NULL)
		{
			std::cout << "cannot allocate memory for the triangle coordinates!" << std::endl;
			return -1 ;
		}


		 for (int i = 0; i < outRow; i++) {

			out[fCount][i] = (float *)malloc(outCol*sizeof(float));
			if(out[fCount][i] == NULL  )
			{
				std::cout << "cannot allocate memory for the triangle coordinates!" << std::endl;
				return -1 ;
			}

		}		
	}
	

	//access data to bring data to cache
	for(long ch=0; ch<channel;ch++ )
	{
		for(long i = 0;i < n1 ; i++)
		{
			for(long j = 0;j < n2 ; j++)
			{
				
				activations[ch][i][j]=1.5; //something to fill the matrix
					
				
				
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
					
					weights[fCount][ch][i][j]=1.5; //something to fill the matrix				
					
				}
					
				
			}
		}
	}
	for (long fCount=0 ; fCount< numFilt; fCount++)
	{
		conv(activations, weights[fCount], n1, n2, f1, f2, s1,s2,channel,out[fCount]);
	}
	/*
	
	for (long fCount=0 ; fCount< numFilt; fCount++)
	{
		for(long i = 0;i < outRow ; i++)
		{
			for(long j = 0;j < outCol ; j++)
			{
					
				std::cout<<out[fCount][i][j]<<" "	;			
					
			}
				std::cout<<"\n";			
		}

	}
	*/
	

	return 0 ;
}

