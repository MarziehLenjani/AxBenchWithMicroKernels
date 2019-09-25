//Copyright (c) 2007 Intel Corp.
// Black-Scholes
// Analytical method for calculating European Options
//
// 
// Reference Source: Options, Futures, and Other Derivatives, 3rd Edition, Prentice 
// Hall, John C. Hull,

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include "../../shortvariable/shortVariable.hpp"
#include <cmath>
#include <random>
//double max_otype, min_otype ;
//double max_sptprice, min_sptprice;
//double max_strike, min_strike;
//double max_rate, min_rate ;
//double max_volatility, min_volatility;
//double max_otime, min_otime ;
//double max_out_price, min_out_price;


#define DIVIDE 120.0


//Precision to use for calculations
#define fptype float

#define NUM_RUNS 1

typedef struct OptionData_ {
        fptype s;          // spot price
        fptype strike;     // strike price
        fptype r;          // risk-free interest rate
        fptype divq;       // dividend rate
        fptype v;          // volatility
        fptype t;          // time to maturity or option expiration in years 
                           //     (1yr = 1.0, 6mos = 0.5, 3mos = 0.25, ..., etc)  
        char OptionType;   // Option type.  "P"=PUT, "C"=CALL
        fptype divs;       // dividend vals (not used in this test)
        fptype DGrefval;   // DerivaGem Reference Value
} OptionData;

OptionData *data;
fptype *prices;
int numOptions;

int    * otype;
fptype * sptprice;
fptype * strike;
fptype * rate;
fptype * volatility;
fptype * otime;
int numError = 0;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Cumulative Normal Distribution Function
// See Hull, Section 11.8, P.243-244
#define inv_sqrt_2xPI 0.39894228040143270286

fptype CNDF ( fptype InputX ) 
{
    int sign;

    fptype OutputX;
    fptype xInput;
    fptype xNPrimeofX;
    fptype expValues;
    fptype xK2;
    fptype xK2_2, xK2_3;
    fptype xK2_4, xK2_5;
    fptype xLocal, xLocal_1;
    fptype xLocal_2, xLocal_3;

    // Check for negative value of InputX
    if (InputX < 0.0) {
        InputX = -InputX;
        sign = 1;
    } else 
        sign = 0;

    xInput = InputX;
 
    // Compute NPrimeX term common to both four & six decimal accuracy calcs
    expValues = exp(-0.5f * InputX * InputX);
    xNPrimeofX = expValues;
    xNPrimeofX = xNPrimeofX * inv_sqrt_2xPI;

    xK2 = 0.2316419 * xInput;
    xK2 = 1.0 + xK2;
    xK2 = 1.0 / xK2;
    xK2_2 = xK2 * xK2;
    xK2_3 = xK2_2 * xK2;
    xK2_4 = xK2_3 * xK2;
    xK2_5 = xK2_4 * xK2;
    
    xLocal_1 = xK2 * 0.319381530;
    xLocal_2 = xK2_2 * (-0.356563782);
    xLocal_3 = xK2_3 * 1.781477937;
    xLocal_2 = xLocal_2 + xLocal_3;
    xLocal_3 = xK2_4 * (-1.821255978);
    xLocal_2 = xLocal_2 + xLocal_3;
    xLocal_3 = xK2_5 * 1.330274429;
    xLocal_2 = xLocal_2 + xLocal_3;

    xLocal_1 = xLocal_2 + xLocal_1;
    xLocal   = xLocal_1 * xNPrimeofX;

    //printf("# xLocal: %10.10f\n", xLocal);



    xLocal   = 1.0 - xLocal;

    OutputX  = xLocal;

    //printf("# Output: %10.10f\n", OutputX);
    
    if (sign) {
        OutputX = 1.0 - OutputX;
    }
    
    return OutputX;
} 

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
fptype BlkSchlsEqEuroNoDiv( fptype sptprice,
                            fptype strike, fptype rate, fptype volatility,
                            fptype time, int otype, float timet, fptype* N1, fptype* N2)
{
    fptype OptionPrice;

    // local private working variables for the calculation
    //fptype xStockPrice;
    //fptype xStrikePrice;
    fptype xRiskFreeRate;
    fptype xVolatility;
    fptype xTime;
    fptype xSqrtTime;

    fptype logValues;
    fptype xLogTerm;
    fptype xD1; 
    fptype xD2;
    fptype xPowerTerm;
    fptype xDen;
    fptype d1;
    fptype d2;
    fptype FutureValueX;
    fptype NofXd1;
    fptype NofXd2;
    fptype NegNofXd1;
    fptype NegNofXd2;  
    
    //xStockPrice = sptprice;
    //xStrikePrice = strike;
    xRiskFreeRate = rate;
    xVolatility = volatility;
    xTime = time;


    xSqrtTime = sqrt(xTime);

    logValues = log( sptprice / strike );
        
    xLogTerm = logValues;
        
    
    xPowerTerm = xVolatility * xVolatility;
    xPowerTerm = xPowerTerm * 0.5;
        
    xD1 = xRiskFreeRate + xPowerTerm;
    xD1 = xD1 * xTime;
    xD1 = xD1 + xLogTerm;

    

    xDen = xVolatility * xSqrtTime;
    xD1 = xD1 / xDen;
    xD2 = xD1 -  xDen;

    d1 = xD1;
    d2 = xD2;
    
    NofXd1 = CNDF( d1 );

    if(NofXd1 > 1.0)
        std::cerr << "Greater than one!" << std::endl ;
    //printf("# d1: %10.10f\n", NofXd1);

    NofXd2 = CNDF( d2 );
    if(NofXd2 > 1.0)
         std::cerr << "Greater than one!" << std::endl ;
    //printf("# d2: %10.10f\n", NofXd2);

    *N1 = NofXd1 ;
    *N2 = NofXd2 ;

    FutureValueX = strike * ( exp( -(rate)*(time) ) );        
    if (otype == 0) {            
        OptionPrice = (sptprice * NofXd1) - (FutureValueX * NofXd2);
        
    } else { 
        NegNofXd1 = (1.0 - NofXd1);
        NegNofXd2 = (1.0 - NofXd2);
        OptionPrice = (FutureValueX * NegNofXd2) - (sptprice * NegNofXd1);
    }
    
    return OptionPrice;
}


double normalize(double in, double min, double max, double min_new, double max_new)
{
    return (((in - min) / (max - min)) * (max_new - min_new)) + min_new ;
}

int bs_thread(void *tid_ptr) {
    int i, j;

    int tid = *(int *)tid_ptr;
    int start = tid * (numOptions);
    int end = start + (numOptions);
    fptype price_orig;

    for (j=0; j<NUM_RUNS; j++) {
        for (i=start; i<end; i++) {
            /* Calling main function to calculate option value based on 
             * Black & Scholes's equation.
             */
            fptype price;
            fptype N1, N2;

            double dataIn[6];
            double dataOut[1];

            dataIn[0]   = sptprice[i];
            dataIn[1]   = strike[i];
            dataIn[2]   = rate[i];
            dataIn[3]   = volatility[i];
            dataIn[4]   = otime[i];
            dataIn[5]   = otype[i];

//#pragma parrot(input, "blackscholes", [6]dataIn)

                price_orig = BlkSchlsEqEuroNoDiv( sptprice[i], strike[i],
                                         rate[i], volatility[i], otime[i], 
                                         otype[i], 0, &N1, &N2);
                dataOut[0] = price_orig;

//#pragma parrot(output, "blackscholes", [1]<0.1; 0.9>dataOut)

                price_orig = dataOut[0];
                prices[i] = price_orig;
        }
    }
    return 0;
}

int main (int argc, char **argv)
{
    FILE *file;
    int i;
    int loopnum;
    fptype * buffer;
    int * buffer2;
    int rv;


	fflush(NULL);
	double maxAbsoluteError=0;

    char *inputFile = argv[1];
    char *outputFile = argv[2];
	//------------------Added Part
	
	char *StrfileNamingArg=argv[3];
//	printf("************  %s\n", StrfileNamingArg);
	fileNamingArg = atoi(StrfileNamingArg);
//	printf("************  %d\n", fileNamingArg);
	NumberOfBits1                    = fileNamingArg;
        NumberOfBits2                    = fileNamingArg;
        NumberOfBits3                    = fileNamingArg;
        NumberOfBits4                    = fileNamingArg;

	setTheMasks(fileNamingArg, &posMask, &negMask);
	StartOfSimulationReport();
	
	StepSize1=	0.100000/pow(2,NumberOfBits1); //TODO:change 111
	StepSize2=	0.128/pow(2,NumberOfBits2);
	StepSize3=	128.0/pow(2,NumberOfBits3);
	StepSize4=	1.0/pow(2,NumberOfBits3);
	
	
	#ifdef multiple_StepSize	
	//ratio= atof(argv[4]);
	ratio= 4;
	StepSize1=((1.0/ratio) 0.100000*)/pow(2,NumberOfBits-2);//TODO:change 111
	#endif
        #ifdef Sampling
        StepSize1=       pow(2,atoi(argv[4]));
        StepSize2=       pow(2,atoi(argv[5]));
        StepSize3=       pow(2,atoi(argv[6]));
        StepSize4=       pow(2,atoi(argv[7]));
        NumberOfSamplingItr= SamplingItrUnit * fileNamingArg ;
        #endif	
	//#if defined(ActiveMasking) || defined(Quantization)
	strcat(outputFile,"_");
	char tmpstr[10];
	sprintf(tmpstr,"%d",fileNamingArg);
	strcat(outputFile,tmpstr);
	
	//#endif
	strcat(outputFile,".txt");
//	printf(" number of bits is  %s %s  == %d",outputFile,StrNumberOfBits, NumberOfBits);
	
//-----------End of Aded part

    //Read input data from file
    file = fopen(inputFile, "r");
    if(file == NULL) {
      printf("ERROR: Unable to open file `%s'.\n", inputFile);
      exit(1);
    }
    rv = fscanf(file, "%i", &numOptions);
    if(rv != 1) {
      printf("ERROR: Unable to read from file `%s'.\n", inputFile);
      fclose(file);
      exit(1);
    }
//--------------------------------------------------------
     std::default_random_engine ActivationGenerator(1024);
     std::normal_distribution<double> distribution1(0,1);
//-------------------------------------------------------
    // alloc spaces for the option data
    data = (OptionData*)malloc(numOptions*sizeof(OptionData));
    prices = (fptype*)malloc(numOptions*sizeof(fptype));
    for ( loopnum = 0; loopnum < numOptions; ++ loopnum )
    {
        rv = fscanf(file, "%f %f %f %f %f %f %c %f %f", &data[loopnum].s, &data[loopnum].strike, &data[loopnum].r, &data[loopnum].divq, &data[loopnum].v, &data[loopnum].t, &data[loopnum].OptionType, &data[loopnum].divs, &data[loopnum].DGrefval);
        if(rv != 9) {
          printf("ERROR: Unable to read from file `%s'.\n", inputFile);
          fclose(file);
          exit(1);
        }
//--------------------------Get data from distribution instead of file
		float ftemp=std::abs(distribution1(ActivationGenerator)/(8.0/256.0));
		data[loopnum].s=ftemp;
		ftemp=ftemp-0.01*ftemp;
		data[loopnum].strike=ftemp;
//------------------------------------------------------------------
		//printf("before %f  \n ",data[loopnum].DGrefval );
		float temp=data[loopnum].r;
		#ifdef ActiveMasking
		maskVariable(&data[loopnum].r, posMask,negMask);	
		#endif		
		SetMaxAndMin( data[loopnum].r,&maxValue1,&minValue1,&NumberOfBits1,StepSize1, loopnum);
		#ifdef Quantization		
		Quantize(&data[loopnum].r,StepSize1,NumberOfBits1,loopnum);
		#endif
		//printf("after %f  \n",data[loopnum].DGrefval );
		if(std::abs(data[loopnum].r-temp)>maxAbsoluteError)
		{
			maxAbsoluteError=std::abs(data[loopnum].r-temp);
		}

//------------------------------
                #ifdef ActiveMasking
                maskVariable(&data[loopnum].s, posMask,negMask);
                #endif
                SetMaxAndMin( data[loopnum].s,&maxValue3,&minValue3,&NumberOfBits3,StepSize3, loopnum);
                #ifdef Quantization             
                Quantize(&data[loopnum].s,StepSize3,NumberOfBits3,loopnum);
                #endif
//--------------------------
                #ifdef ActiveMasking
                maskVariable(&data[loopnum].strike, posMask,negMask);
                #endif
                SetMaxAndMin( data[loopnum].strike,&maxValue3,&minValue3,&NumberOfBits3,StepSize3, loopnum);
                #ifdef Quantization             
                Quantize(&data[loopnum].strike,StepSize3,NumberOfBits3,loopnum);
                #endif
//---------------------

                #ifdef ActiveMasking
                maskVariable(&data[loopnum].v, posMask,negMask);
                #endif
                SetMaxAndMin( data[loopnum].v,&maxValue4,&minValue4,&NumberOfBits4,StepSize4, loopnum);
                #ifdef Quantization             
                Quantize(&data[loopnum].v,StepSize4,NumberOfBits4,loopnum);
                #endif
//--------------------
                #ifdef ActiveMasking
                maskVariable(&data[loopnum].t, posMask,negMask);
                #endif
                SetMaxAndMin( data[loopnum].t,&maxValue4,&minValue4,&NumberOfBits1,StepSize1, loopnum);
                #ifdef Quantization             
                Quantize(&data[loopnum].t,StepSize4,NumberOfBits4,loopnum);
                #endif
    }
    rv = fclose(file);
    if(rv != 0) {
      printf("ERROR: Unable to close file `%s'.\n", inputFile);
      exit(1);
    }

#define PAD 256
#define LINESIZE 64

    buffer = (fptype *) malloc(5 * numOptions * sizeof(fptype) + PAD);
    sptprice = (fptype *) (((unsigned long long)buffer + PAD) & ~(LINESIZE - 1));
    strike = sptprice + numOptions;
    rate = strike + numOptions;
    volatility = rate + numOptions;
    otime = volatility + numOptions;

    buffer2 = (int *) malloc(numOptions * sizeof(fptype) + PAD);
    otype = (int *) (((unsigned long long)buffer2 + PAD) & ~(LINESIZE - 1));

    for (i=0; i<numOptions; i++) {
        otype[i]      = (data[i].OptionType == 'P') ? 1 : 0;
        sptprice[i]   = data[i].s / DIVIDE;
        strike[i]     = data[i].strike / DIVIDE;
        rate[i]       = data[i].r;
        volatility[i] = data[i].v;    
        otime[i]      = data[i].t;
    }

    //serial version
    int tid=0;
    bs_thread(&tid);


    //Write prices to output file
    file = fopen(outputFile, "w");
    if(file == NULL) {
      printf("ERROR: Unable to open file `%s'.\n", outputFile);
      exit(1);
    }
    //rv = fprintf(file, "%i\n", numOptions);
    if(rv < 0) {
      printf("ERROR: Unable to write to file `%s'.\n", outputFile);
      fclose(file);
      exit(1);
    }
    for(i=0; i<numOptions; i++) {

		#ifdef ActiveMasking
		maskVariable(&prices[i], posMask,negMask);
		#endif
		SetMaxAndMin( prices[i],&maxValue2,&minValue2,&NumberOfBits2,StepSize2, i);
		#ifdef Quantization
		Quantize(&prices[i],StepSize2,NumberOfBits2,i);
		#endif

      rv = fprintf(file, "%.18f\n", prices[i]);
      if(rv < 0) {
        printf("ERROR: Unable to write to file `%s'.\n", outputFile);
        fclose(file);
        exit(1);
      }
    }
    rv = fclose(file);
    if(rv != 0) {
      printf("ERROR: Unable to close file `%s'.\n", outputFile);
      exit(1);
    }

    free(data);
    free(prices);
//	printf("Max1 is:  %f   , min1 is %f  \n", maxValue2,minValue2);
//	printf("MaxEXP is:  %d   , minEXP is %d  \n", maxExp,minExp);
    EndOfSimulationReport(10*numOptions);
	printf("  %f", maxAbsoluteError);
    return 0;
}

