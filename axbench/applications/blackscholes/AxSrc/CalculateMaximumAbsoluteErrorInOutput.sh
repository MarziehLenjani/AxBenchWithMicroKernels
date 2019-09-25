#!/bin/bash
BaseDirectory=/bigtemp/ml2au/AxBench/AxBenchCPU/axbench/applications/blackscholes/ 
cd ${BaseDirectory}AxSrc 

inputFile=${BaseDirectory}test.data/output/blackscholesTest_200K_blackscholes_orig.data



python ../scripts/qosOutputMarginalVsQuan.py ${inputFile}

