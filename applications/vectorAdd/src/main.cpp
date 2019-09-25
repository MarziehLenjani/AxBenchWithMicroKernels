#include<stdlib.h>
#include<stdio.h>
#include <random>
#include <iostream>
#include <fstream>
using namespace std;
int main (int argc, char * argv[]){
	float * valueArrayX;
	float * valueArrayY;
	float * valueArrayZ;
	//Axy=> y=alpha *X+Y
	if(argc<3)
	{
		cout<<" help: you need two command line arguments n and range file name"<<endl;
		exit(1);
	}
	int n =atoi(argv[1]);
	float alpha=0.3;

	valueArrayX= (float *) malloc(n*sizeof(float));
	valueArrayY= (float *) malloc(n*sizeof(float));
	valueArrayZ= (float *) malloc(n*sizeof(float));
	//----------------------------------------
	std::string rangeFilename 	= argv[2];
	std::ofstream rangeFileHandler;
	rangeFileHandler.open(rangeFilename);
	rangeFileHandler <<3<< std::endl;
	rangeFileHandler <<long(&valueArrayX[0])<< " " << long(&valueArrayX[n-1]) << std::endl;
	rangeFileHandler <<long(&valueArrayY[0])<< " " << long(&valueArrayY[n-1]) << std::endl;
	rangeFileHandler <<long(&valueArrayZ[0])<< " " << long(&valueArrayZ[n-1]) << std::endl;
	rangeFileHandler.flush();
	rangeFileHandler.close();
	//-------------------------------------------


	/*//No initialization here to inlcude only the core part of the algorithem
	 default_random_engine generator;
	normal_distribution<float> distribution(min,max);
	float stpSize=(max-min)/numOfBins; //TODO: check for divide by zero
	for (int i=0; i< n; i++){
		valueArrayX[i]=distribution(generator);
		valueArrayY[i]=distribution(generator);
	}
	*/
	//end of iniitialization
	for (int i=0; i<n; i++){
		valueArrayZ[i]= alpha*valueArrayX[i]+valueArrayY[i];

	}

	return 0;

}
