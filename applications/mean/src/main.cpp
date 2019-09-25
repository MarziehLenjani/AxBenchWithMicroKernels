#include<stdlib.h>
#include<stdio.h>
#include <random>
#include <iostream>
#include <fstream>
using namespace std;
int main (int argc, char * argv[]){
	float * valueArray;
	if(argc<3)
	{
		cout<<" help: you need two command line arguments n and range file name"<<endl;
		exit(1);
	}
	int n =atoi(argv[1]);
	valueArray= (float *) malloc(n*sizeof(float));

	//----------------------------------------
	std::string rangeFilename 	= argv[2];
	std::ofstream rangeFileHandler;
	rangeFileHandler.open(rangeFilename);
	rangeFileHandler <<1<< std::endl;
	rangeFileHandler <<long(&valueArray[0])<< " " << long(&valueArray[n-1]) << std::endl;
	rangeFileHandler.flush();
	rangeFileHandler.close();
	//-------------------------------------------


	float mean=0;
	float sum=0;
	/*//No initialization here to include only the core part of the algorithem
	 	default_random_engine generator;
	normal_distribution<float> distribution(min,max);
	for (int i=0; i< n; i++){
		valueArray[i]=distribution(generator);
	}
	*/
	//end of iniitialization
	for (int i=0; i<n; i++){
		sum=sum+valueArray[i];

	}
	mean=sum/n;
	/*
	for(int i=0; i<numOfBins;i++){
		cout<<histCount[i]<<endl;
	}
	cout<<"end of the program";
	*/
	return 0;

}
