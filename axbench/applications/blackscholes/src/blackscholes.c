// Copyright (c) 2007 Intel Corp.

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
/*
            double dataIn[6];
            double dataOut[1];

            dataIn[0]   = sptprice[i];
            dataIn[1]   = strike[i];
            dataIn[2]   = rate[i];
            dataIn[3]   = volatility[i];
            dataIn[4]   = otime[i];
            dataIn[5]   = otype[i];
			*/

//#pragma parrot(input, "blackscholes", [6]dataIn)

                price_orig = BlkSchlsEqEuroNoDiv( sptprice[i], strike[i],
                                         rate[i], volatility[i], otime[i], 
                                         otype[i], 0, &N1, &N2);
                //dataOut[0] = price_orig;

//#pragma parrot(output, "blackscholes", [1]<0.1; 0.9>dataOut)

                //price_orig = dataOut[0];
                prices[i] = price_orig;
        }
    }
    return 0;
}

int main (int argc, char **argv)
{
    FILE *file,*outFile;
    int i;
    int loopnum;
    fptype * buffer;
    int * buffer2;
    int rv;
	float ratio=1;

	fflush(NULL);


    char *inputFile = argv[1];
    char *outputFile = argv[2];
	int originalVariableLength =atoi(argv[3]);
	int reducedVariableLength =atoi(argv[4]);
	int FileReadCycle =atoi(argv[5]);
	ratio= float(originalVariableLength)/float(reducedVariableLength);
    //Read input data from file
    file = fopen(inputFile, "r");
    if(file == NULL) {
      printf("ERROR1: Unable to open file `%s'.\n", inputFile);
      exit(1);
    }
    rv = fscanf(file, "%i", &numOptions);
    if(rv != 1) {
      printf("ERROR2: Unable to read from file `%s'.\n", inputFile);
      fclose(file);
      exit(1);
    }


    // alloc spaces for the option data
    data = (OptionData*)malloc(numOptions*sizeof(OptionData));
    prices = (fptype*)malloc(numOptions*sizeof(fptype));
    outFile = fopen(outputFile, "w");
    if(outFile == NULL) {
      printf("ERRORo1: Unable to open file `%s'.\n", outputFile);
      exit(1);
    }

    rv = fprintf(outFile, "%i\n", 4);
    if(rv < 0) {
      printf("ERRORo2: Unable to write to file `%s'.\n", outputFile);
      fclose(outFile);
      exit(1);
    }  
    rv = fprintf(outFile, "%lu   ", (long unsigned int)(&data[0]));
      if(rv < 0) {
        printf("ERRORo3: Unable to write to file `%s'.\n", outputFile);
        fclose(outFile);
        exit(1);
    }
    rv = fprintf(outFile, "%.lu\n", (long unsigned int)(&data[numOptions]));
      if(rv < 0) {
        printf("ERRORo4: Unable to write to file `%s'.\n", outputFile);
        fclose(outFile);
        exit(1);
    }   
	rv = fprintf(outFile, "%lu   ", (long unsigned int )(&prices[0]));
    if(rv < 0) {
        printf("ERRORo5: Unable to write to file `%s'.\n", outputFile);
        fclose(outFile);
        exit(1);
    }
    rv = fprintf(outFile, "%.lu\n",(long unsigned int)(&prices[numOptions]));
    if(rv < 0) {
        printf("ERRORo6: Unable to write to file `%s'.\n", outputFile);
        fclose(outFile);
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
    rv = fprintf(outFile, "%lu   ", (long unsigned int)(&buffer[0]));
      if(rv < 0) {
        printf("ERROR: Unable to write to file `%s'.\n", outputFile);
        fclose(outFile);
        exit(1);
    }
    rv = fprintf(outFile, "%.lu\n", (long unsigned int)(&buffer[5 * numOptions +(PAD/sizeof(fptype))]));
      if(rv < 0) {
        printf("ERROR: Unable to write to file `%s'.\n", outputFile);
        fclose(outFile);
        exit(1);
    }   
    rv = fprintf(outFile, "%lu   ", (long unsigned int)(&buffer2[0]));
      if(rv < 0) {
        printf("ERROR: Unable to write to file `%s'.\n", outputFile);
        fclose(outFile);
        exit(1);
    }
    rv = fprintf(outFile, "%.lu\n", (long unsigned int)(&buffer2[numOptions +(PAD/sizeof(fptype))]));
      if(rv < 0) {
        printf("ERROR: Unable to write to file `%s'.\n", outputFile);
        fclose(outFile);
        exit(1);
    } 
	fflush(outFile);
    rv = fclose(outFile);
    if(rv != 0) {
      printf("ERROR: Unable to close file `%s'.\n", outputFile);
      exit(1);
    }
	//warmup
	//exit(0);
	OptionData tmpData;

    for ( loopnum = 0; loopnum < numOptions; ++ loopnum )
    {
		if(loopnum<(numOptions/ratio) &&( (FileReadCycle==0)||( loopnum<FileReadCycle)) )
		{
			
			rv = fscanf(file, "%f %f %f %f %f %f %c %f %f", &tmpData.s, &tmpData.strike, &tmpData.r, &tmpData.divq, &tmpData.v, &tmpData.t, &tmpData.OptionType, &tmpData.divs, &tmpData.DGrefval);
			//printf("We read Something\n");
			if(rv != 9) {
			  printf("ERROR3: Unable to read from file `%s'.\n", inputFile);
			  fclose(file);
			  exit(1);
			}
			
		}
		data[loopnum]=tmpData;
		
		
    }

	
    rv = fclose(file);
    if(rv != 0) {
      printf("ERROR: Unable to close file `%s'.\n", inputFile);
      exit(1);
    }
	


 

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

/*
    //Write prices to output file
    file = fopen(outputFile, "w");
    if(file == NULL) {
      printf("ERROR: Unable to open file `%s'.\n", outputFile);
      exit(1);
    }

    rv = fprintf(file, "%i\n", numOptions);
    if(rv < 0) {
      printf("ERROR: Unable to write to file `%s'.\n", outputFile);
      fclose(file);
      exit(1);
    }

    for(i=0; i<numOptions; i++) {
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
*/

    free(data);
    free(prices);

    return 0;
}

