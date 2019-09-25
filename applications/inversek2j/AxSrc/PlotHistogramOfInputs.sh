#!/bin/bash
BaseDirectory=/bigtemp/ml2au/AxBench/AxBenchCPU/axbench/applications/inversek2j/ 
cd ${BaseDirectory}AxSrc 

inputFile=${BaseDirectory}test.data/input/theta_1000K.data

python3 ../scripts/PlotHistogramInput.py ${inputFile}