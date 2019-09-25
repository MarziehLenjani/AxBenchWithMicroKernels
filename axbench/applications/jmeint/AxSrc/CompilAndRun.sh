#!/bin/bash
source ../../shortvariable/Common.sh
BaseDirectory=${SharedPath}jmeint/ 
cd ${BaseDirectory}AxSrc 

g++ -std=c++11 jmeint.cpp ../../shortvariable/shortVariable.cpp tritri.cpp -o jmeintAx

if [ $? -ne 0 ]; then
    echo compile failed 
        exit  1
fi

fourthArg=""
outputdir=QuantOutput
#outputdir=MargOutpu
#outputdir=IgnoreOutlier/StepSize 
#outputdir=MultiStepOutput
inputFile=${BaseDirectory}test.data/input/jmeint_1000K.data
fourthArg=$1
outputdir=${outputdir}$fourthArg
#FthArg=$1
#SthArg=$2
#TthArg=$3
#FOthArg=$4
FthArg=-6
SthArg=-3
TthArg=-3
FOthArg=-3

mkdir -p ${outputdir}
rm -r ${outputdir}/*
echo ${fourthArg}

#echo $inputFile
#to 31 for quant to 23 for marginal
if [ $Sampling == 1 ] ; then
	for  (( i=0; i<=$numOfRun; i=i+$step )) 
	do
# 	echo -n $i
	 	./jmeintAx ${inputFile} ${outputdir}/AxOutOrg ${i}  ${FthArg} ${SthArg} ${TthArg} ${FOthArg}
 		python ../scripts/qos.py ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_0.txt ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_${i}.txt 
	done
else
	for i in {0..31}
	do
# 		echo -n $i
 		./jmeintAx ${inputFile} ${outputdir}/AxOutOrg ${i} ${fourthArg}
		 python ../scripts/qos.py ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_0.txt ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_${i}.txt 
	done
fi

#temp runs 
#./jmeintAx /bigtemp/ml2au/AxBench/AxBenchCPU/axbench/applications/jmeint/test.data/input/jmeint_1000K.data testOutput.txt 

#./jmeintAx ${inputFile} ${outputdir}/AxOutOrg 0
