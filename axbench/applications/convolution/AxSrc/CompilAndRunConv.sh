#!/bin/bash
source ../../shortvariable/Common.sh
BaseDirectory=${SharedPath}convolution/

cd ${BaseDirectory}AxSrc 
[ -e convolutionAx ] && rm  convolutionAx 
 g++ -static -std=c++11  ../../shortvariable/shortVariable.cpp convolution.cpp   -o convolutionAx  
if [ $? -ne 0 ]; then
    echo compile failed 
        exit  1
fi
fourthArg=""
#FthArg=$1
#SthArg=$2
#TthArg=$3
#FOthArg=$4
FthArg=-8
SthArg=-8
TthArg=-8
FOthArg=0
outputdir=QuantOutput
#outputdir=MargOutput
#outputdir=IgnoreOutlier/StepSize 
#outputdir=MultiStepOutput
fourthArg=$1
outputdir=${outputdir}$fourthArg
[ -d ${outputdir} ] && rm  -r ${outputdir}
mkdir  ${outputdir}
echo ${fourthArg}

#echo  Convolution  iNumberOfBits	AverageRelativeError	MaximumAbsoluteError
if [ $Sampling == 1 ] ; then
for  (( i=0; i<=$numOfRun; i=i+$step )) 
do
# echo -n $i
 ./convolutionAx 224 224 11 11 0 0 2 2 100 100  ${outputdir}/AxOutOrg ${i} 6 1.5 32 1  ${FthArg} ${SthArg} ${TthArg} ${FOthArg}
 python ../scripts/qos.py ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_0.txt ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_${i}.txt 
done
else
for i in {0..32}
do
 #echo -n $i
 ./convolutionAx 224 224 11 11 0 0 2 2 100 100  ${outputdir}/AxOutOrg ${i} 6 1.5 32 1
 python ../scripts/qos.py ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_0.txt ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_${i}.txt
done
fi
:<<'END'
#----------------
#FthArg=$1
#SthArg=$2
#TthArg=$3
#FOthArg=$4
FthArg=-8
SthArg=-8
TthArg=-8
FOthArg=0
[ -d ${outputdir} ] && rm  -r ${outputdir}
mkdir  ${outputdir}
#-----------
#echo  VGG  NumberOfBits		AverageRelativeError	MaximumAbsoluteError
if [ $Sampling == 1 ] ; then
	for  (( i=0; i<=32; i=i+$step )) 
	do
	# echo -n $i
		 ./convolutionAx 224 224 3 3 1 1 2 2 512 1  ${outputdir}/AxOutOrg ${i} 1 0.05 2 0  ${FthArg} ${SthArg} ${TthArg} ${FOthArg}
 	python ../scripts/qos.py ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_0.txt ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_${i}.txt
 	done
else
	for i in {0..32}
	do
	 #echo -n $i
	 ./convolutionAx 224 224 3 3 1 1 2 2 512 1  ${outputdir}/AxOutOrg ${i} 1 0.05 2 0
 	python ../scripts/qos.py ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_0.txt ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_${i}.txt
	 done

fi	
END
:<<'END'
[ -d ${outputdir} ] && rm  -r ${outputdir}
 mkdir  ${outputdir}

echo  Conv  NumberOfBits	AverageRelativeError	MaximumAbsoluteError
for i in {0..32}
 do
 echo -n $i
 ./convolutionAx 224 224 11 11 0 0 2 2 100 1  ${outputdir}/AxOutOrg ${i} 1 0.5 2 0
 python ../scripts/qos.py ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_0.txt ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_${i}.txt
 done

[ -d ${outputdir} ] && rm  -r ${outputdir}
 mkdir  ${outputdir}
#--------------
echo  DotProduct  NumberOfBits          AverageRelativeError    MaximumAbsoluteError
for i in {0..32}
 do
 echo -n $i
 ./convolutionAx 512 512 512 512 1 1 512 512 1 1   ${outputdir}/AxOutOrg ${i} 1 0.05 2 0
 python ../scripts/qos.py ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_0.txt ${BaseDirectory}AxSrc/${outputdir}/AxOutOrg_${i}.txt
 done
END
