#!/bin/bash 
source ../../shortvariable/Common.sh
BaseDirectory=${SharedPath}Oldsobel/ 
cd ${BaseDirectory}AxSrc
[ -e sobelAx ] && rm  sobelAx 
g++ -w -std=c++11 convolution.cpp sobelAx.cpp rgb_image.cpp  ../../shortvariable/shortVariable.cpp -lboost_regex  -o sobelAx  
if [ $? -ne 0 ]; then
    echo compile failed 
        exit  1
fi
fourthArg=""
outputdir=QuantOutput
#outputdir=MargOutput
#outputdir=IgnoreOutlier/StepSize 
#outputdir=MultiStepOutput
inputFile=${BaseDirectory}test.data/input/4.rgb
fourthArg=$1
outputdir=${outputdir}$fourthArg
[ -d ${outputdir} ] && rm  -rf ${outputdir}
mkdir  ${outputdir}
echo ${fourthArg}
#FthArg=$1
#SthArg=$2
#TthArg=$3
#FOthArg=$4
FthArg=0
SthArg=-10
TthArg=-8
FOthArg=-8
#echo NumberOfBits	AverageRelativeError	MaximumAbsoluteError
if [ $Sampling == 1 ] ; then
	for  (( i=0; i<=$numOfRun; i=i+$step )) 
	do
 	#echo -n $i
	 ./sobelAx ${inputFile} ${outputdir}/AxOutOrg ${i} ${FthArg} ${SthArg} ${TthArg} ${FOthArg}
	 python ../scripts/qos.py ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_0.txt ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_${i}.txt 
	done
else

	for i in {0..32}
	do
	# echo -n $i
	 ./sobelAx ${inputFile} ${outputdir}/AxOutOrg ${i} ${fourthArg}
	 python ../scripts/qos.py ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_0.txt ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_${i}.txt
	done
fi

