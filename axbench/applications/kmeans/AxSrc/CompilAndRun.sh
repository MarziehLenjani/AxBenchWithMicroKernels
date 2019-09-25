#!/bin/bash
source ../../shortvariable/Common.sh
BaseDirectory=${SharedPath}kmeans/ 
cd ${BaseDirectory}AxSrc 
[ -e kmeansAx ] && rm  kmeansAx 
g++ -std=c++11 kmeansAx.c distance.c rgbimage.c segmentation.c  ../../shortvariable/shortVariable.cpp  -o kmeansAx  
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
[ -d ${outputdir} ] && rm  -r ${outputdir}
mkdir  ${outputdir}
echo ${fourthArg}
#FthArg=$1
#SthArg=$2
#TthArg=$3
#FOthArg=$4
FthArg=-8
SthArg=-8
TthArg=0
FOthArg=0

#echo NumberOfBits	AverageRelativeError	MaximumAbsoluteError
if [ $Sampling == 1 ] ; then
	for  (( i=0; i<=$numOfRun; i=i+$step ))
	do
#		echo -n $i
	 	./kmeansAx ${inputFile} ${outputdir}/AxOutOrg ${i} ${FthArg} ${SthArg} ${TthArg} ${FOthArg}
	 	python ../scripts/qos.py ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_0.txt ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_${i}.txt 
	done
else

	for i in {0..32}
	do
 #	echo -n $i
 		./kmeansAx ${inputFile} ${outputdir}/AxOutOrg ${i} ${fourthArg}
 		python ../scripts/qos.py ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_0.txt ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_${i}.txt 
	done
fi
