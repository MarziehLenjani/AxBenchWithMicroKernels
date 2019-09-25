#!/bin/bash
BaseDirectory=/bigtemp/ml2au/AxBench/AxBenchCPU/axbench/applications/jmeint/ 
cd ${BaseDirectory}AxSrc 


inputFile=${BaseDirectory}test.data/input/jmeint_1000K.data

python ../scripts/qosInputMarginalVsQuan.py ${inputFile}

