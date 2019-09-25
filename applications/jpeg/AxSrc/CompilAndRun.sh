#!/bin/bash
BaseDirectory=/bigtemp/ml2au/AxBench/AxBenchCPU/axbench/applications/jpeg/ 
cd ${BaseDirectory}AxSrc 

g++ -std=c++11 encoder.c dct.c huffman.c jpeg.c  marker.c quant.c rgbimage.c -o jpegAx
fourthArg=""
outputdir=QuantOutput
#outputdir=MargOutpu
#outputdir=IgnoreOutlier/StepSize 
#outputdir=MultiStepOutput
inputFile=${BaseDirectory}test.data/input/5.rgb
fourthArg=$1
outputdir=${outputdir}$fourthArg

mkdir -p ${outputdir}
echo ${fourthArg}

echo $inputFile
#to 31 for quant to 23 for marginal
for i in {0..31}
do
 echo $i
 ./jpegAx ${inputFile} ${outputdir}/AxOutOrg ${i} ${fourthArg}
 python ../scripts/qos.py ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_0.txt ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_${i}.txt 
done

#temp runs 
#./jmeintAx /bigtemp/ml2au/AxBench/AxBenchCPU/axbench/applications/jmeint/test.data/input/5.rgb testOutput.txt 

#./jmeintAx ${inputFile} ${outputdir}/AxOutOrg 0