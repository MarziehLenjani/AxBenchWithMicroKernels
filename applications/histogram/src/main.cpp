#include<stdlib.h>
#include<stdio.h>
#include <random>
#include <iostream>
#include <fstream>
using namespace std;
int main (int argc, char * argv[]){
	float * valueArray;
	int * histCount;
	if(argc<5)
	{
		cout<<" help: you need five command line arguments n numOfBin max min range file name"<<endl;
		exit(1);
	}
	int n =atoi(argv[1]);
	int numOfBins=atoi(argv[2]);
	float max=atof(argv[3]);
	float min=atof(argv[4]);
	if(min> max)
		cout<< "min is larger than max"<<endl;
	valueArray= (float *) malloc(n*sizeof(float));
	histCount= (int*) malloc(numOfBins*sizeof(int));
	//----------------------------------------
	std::string rangeFilename 	= argv[5];
	std::ofstream rangeFileHandler;
	rangeFileHandler.open(rangeFilename);
	rangeFileHandler <<1<< std::endl;
	rangeFileHandler <<long(&valueArray[0])<< " " << long(&valueArray[n-1]) << std::endl;
	rangeFileHandler.flush();
	rangeFileHandler.close();
	//-------------------------------------------

	float stpSize=(max-min)/numOfBins; //TODO: check for divide by zero
	//initialization
	for(int i=0; i<numOfBins; i++){
		histCount[i]=0;

	}
	/*//No initialization here to inlcude only the core part of the algorithem
	 * 	default_random_engine generator;
	normal_distribution<float> distribution(min,max);
	for (int i=0; i< n; i++){
		valueArray[i]=distribution(generator);
	}
	*/
	//end of iniitialization
	for (int i=0; i<n; i++){
		int binIndex=0;
		if(valueArray[i]> max)
		{
				binIndex=n-1;
		}else{
			if(valueArray[i]<min)
				{
					binIndex=0;
				}else{
					binIndex=valueArray[i]/stpSize;
			}
		}
		histCount[binIndex]++;

	}
	/*
	for(int i=0; i<numOfBins;i++){
		cout<<histCount[i]<<endl;
	}
	cout<<"end of the program";
	*/
	return 0;

}
