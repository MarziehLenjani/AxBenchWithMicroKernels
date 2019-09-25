#!/bin/bash
source ../../shortvariable/Common.sh
#BaseDirectory=/if22/ml2au/AxBenchCPU/axbench/applications/fft/ 
BaseDirectory=${SharedPath}fft/ 
cd ${BaseDirectory}AxSrc 
[ -e fftAx ] && rm  fftAx
#echo NumberOfBits	AverageRelativeError	MaximumAbsoluteError
g++ -std=c++11 fftAx.cpp fourier.cpp complex.cpp ../../shortvariable/shortVariable.cpp  -o fftAx 

if [ $? -ne 0 ]; then
    echo compile failed 
	exit  1
fi
fourthArg=""
outputdir=QuantOutput
#outputdir=MargOutpu
#outputdir=IgnoreOutlier/StepSize 
#outputdir=MultiStepOutput
[ -d ${outputdir} ] && rm  -r ${outputdir}
inputFile=${BaseDirectory}/AxSrc/Train_Arabic_Digit.txt
#FthArg=$1
#SthArg=$2
#TthArg=$3
#FOthArg=$4
FthArg=-10
SthArg=-3
TthArg=-3
FOthArg=-3

#inputSize=2048
inputSize=262144
outputdir=${outputdir}$fourthArg

mkdir -p ${outputdir}
echo ${fourthArg}

#echo $inputFile
#to 31 for quant to 23 for marginal
if [ $Sampling == 1 ] ; then
 	for  (( i=0; i<=$numOfRun; i=i+$step )) do	
#		printf "%2d"  "$i"
 		./fftAx ${inputSize}  ${inputFile} ${outputdir}/AxOutOrg $i ${FthArg} ${SthArg} ${TthArg} ${FOthArg}
 		python ../scripts/qos.py ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_0.txt ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_${i}.txt
	done 
else

	for i in {0..32}	
	do
#		echo -n  $i
 		./fftAx ${inputSize}  ${inputFile} ${outputdir}/AxOutOrg ${i} 
 		python ../scripts/qos.py ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_0.txt ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_${i}.txt 
	done
fi
#temp runs 
#./blackscholesAx /bigtemp/ml2au/AxBench/AxBenchCPU/axbench/applications/blackscholes/test.data/input/blackscholesTest_200K.data testOutput.txt 

#./jmeintAx ${inputFile} ${outputdir}/AxOutOrg 0
