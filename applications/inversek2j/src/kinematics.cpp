/*
 * kinematics.cpp
 * 
 *  Created on: Sep. 10 2013
 *			Author: Amir Yazdanbakhsh <yazdanbakhsh@wisc.edu>
 */

#include <cmath>
#include <cstdlib>

#include "kinematics.hpp"


float l1 = 0.5 ;
float l2 = 0.5 ;

#if defined(SWQNT) || defined(SWQNTInPlace)
extern  float SWQntstepsize;
extern  float mid;
extern  int TmpSwQnt;
extern  float* SWQ_t1t2xy;
#endif



void forwardk2j(float theta1, float theta2, float* x, float* y) {

	*x = l1 * cos(theta1) + l2 * cos(theta1 + theta2) ;
	*y = l1 * sin(theta1) + l2 * sin(theta1 + theta2) ;
	#ifdef SWQNTInPlace
		float theta1t= (theta1+mid)*SWQntstepsize;
		float theta2t= (theta2+mid)*SWQntstepsize;
	#endif


}

void inversek2j(float x, float y, float* theta1, float* theta2) {

	double dataIn[2];
//	dataIn[0] = x;
//	dataIn[1] = y;

//	double dataOut[2];

//#pragma parrot(input, "inversek2j", [2]dataIn)
	


	*theta2 = (float)acos(((x * x) + (y * y) - (l1 * l1) - (l2 * l2))/(2 * l1 * l2));
	*theta1 = (float)asin((y * (l1 + l2 * cos(*theta2)) - x * l2 * sin(*theta2))/(x * x + y * y));
	#ifdef SWQNTInPlace
		float yt= (y+mid)*SWQntstepsize;
		float xt= (x+mid)*SWQntstepsize;
	#endif


//	dataOut[0] = (*theta1);
//	dataOut[1] = (*theta2);

//#pragma parrot(output, "inversek2j", [2]<0.0; 2.0>dataOut)


//	*theta1 = dataOut[0];
//	*theta2 = dataOut[1];
}
