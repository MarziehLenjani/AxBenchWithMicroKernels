#!/bin/bash
source ../../shortvariable/Common.sh
BaseDirectory=${SharedPath}inversek2j/ 
cd ${BaseDirectory}AxSrc 
[ -e inversek2jAx ] && rm  inversek2jAx 
g++ -std=c++11 inversek2jAx.cpp kinematics.cpp  ../../shortvariable/shortVariable.cpp  -o inversek2jAx  
if [ $? -ne 0 ]; then
    echo compile failed 
        exit  1
fi
fourthArg=""
outputdir=QuantOutput
#outputdir=MargOutput
#outputdir=IgnoreOutlier/StepSize 
#outputdir=MultiStepOutput
inputFile=${BaseDirectory}test.data/input/theta_1000K.data
#FthArg=$1
#SthArg=$2
#TthArg=$3
#FOthArg=$4
FthArg=-10
SthArg=-10
TthArg=-10
FOthArg=-10
fourthArg=$1
outputdir=${outputdir}$fourthArg
[ -d ${outputdir} ] && rm  -r ${outputdir}
mkdir -p ${outputdir}
echo ${fourthArg}

#echo NumberOfBits	AverageRelativeError	MaximumAbsoluteError

if [ $Sampling == 1 ] ; then
        for  (( i=0; i<=$numOfRun; i=i+$step )) do
                ./inversek2jAx ${inputFile} ${outputdir}/AxOutOrg ${i}  ${FthArg} ${SthArg} ${TthArg} ${FOthArg}
                 python ../scripts/qos.py ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_0.txt ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_${i}.txt		
	done
else
	for i in {0..32}
        do
#		echo -n $i
 		./inversek2jAx ${inputFile} ${outputdir}/AxOutOrg ${i} ${fourthArg}
		 python ../scripts/qos.py ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_0.txt ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_${i}.txt 
	done
fi


#temp runs 
#./inversek2jAx /bigtemp/ml2au/AxBench/AxBenchCPU/axbench/applications/inversek2j/test.data/input/theta_1000K.data testOutput.txt 

#./inversek2jAx ${inputFile} ${outputdir}/AxOutOrg 0
