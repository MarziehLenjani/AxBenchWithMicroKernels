#!/bin/bash
source ../../shortvariable/Common.sh
BaseDirectory=${SharedPath}blackscholes/ 
cd ${BaseDirectory}AxSrc 
[ -e blackscholesAx ] && rm  blackscholesAx

g++ -std=c++11 blackscholes.c  ../../shortvariable/shortVariable.cpp  -o blackscholesAx 

if [ $? -ne 0 ]; then
    echo compile failed 
        exit  1
fi
fourthArg=""
outputdir=outputdir="${outputdir}/blackscholes"
#outputdir=MargOutpu
#outputdir=IgnoreOutlier/StepSize 
#outputdir=MultiStepOutput
[ -d ${outputdir} ] && rm  -r ${outputdir}
inputFile=${BaseDirectory}test.data/input/blackscholesTest_200K.data
fourthArg=$1
outputdir=${outputdir}$fourthArg
#FthArg=$1
#SthArg=$2
#TthArg=$3
#FOthArg=$4
FthArg=-6
SthArg=-12
TthArg=-3
FOthArg=-6
mkdir -p ${outputdir}
echo ${fourthArg}

#echo $inputFile
#to 31 for quant to 23 for marginal
if [ $Sampling == 1 ] ; then
        for  (( i=0; i<=$numOfRun; i=i+$step )) do 
#                printf "%2d"  "$i"
                ./blackscholesAx  ${inputFile} ${outputdir}/AxOutOrg ${i}  ${FthArg} ${SthArg} ${TthArg} ${FOthArg}
                python ../scripts/qos.py ${outputdir}/AxOutOrg_0.txt ${outputdir}/AxOutOrg_${i}.txt
        done
else
	for i in {0..32}
	do
# 		echo -n $i
 		./blackscholesAx  ${inputFile} ${outputdir}/AxOutOrg ${i} ${fourthArg}
 	python ../scripts/qos.py ${outputdir}/AxOutOrg_0.txt ${outputdir}/AxOutOrg_${i}.txt 
done
fi
#temp runs 
#./blackscholesAx /bigtemp/ml2au/AxBench/AxBenchCPU/axbench/applications/blackscholes/test.data/input/blackscholesTest_200K.data testOutput.txt 

#./jmeintAx ${inputFile} ${outputdir}/AxOutOrg 0
