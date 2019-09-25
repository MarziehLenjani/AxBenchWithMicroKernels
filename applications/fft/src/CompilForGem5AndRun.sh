#!/bin/bash
#SWQNTFLAG="-DSWQNT"
SWQNTFLAG="-DSWQNTInPlace"
#SWQNTFLAG="-DAlaki"
BinaryNamePost="SWQNT"
applicationName=fft
Compilefilenames=" fft.cpp fourier.cpp complex.cpp"
#BaseDirectory=/bigtemp/ml2au/AxBench/AxBenchCPU/axbench/applications/${applicationName}/ 
#cd ${BaseDirectory}/src 
cwd=$(pwd)
RangeFilePath=${cwd}/Rangefiles/
mkdir -p ${RangeFilePath}
[ -e ${RangeFilePath1}_0 ] && rm ${RangeFilePath1}_0
RangeFilePath1=${RangeFilePath}1.txt
optiont1="262144 ${RangeFilePath1}_0"
compileFlag="-static -std=c++11"
#: << 'END'
[ -e ${applicationName}Range${BinaryNamePost}  ] && rm ${applicationName}Range${BinaryNamePost} 
g++ ${compileFlag} "${SWQNTFLAG}"  ${Compilefilenames} -o ${applicationName}Range${BinaryNamePost} 
time ./${applicationName}Range${BinaryNamePost}  ${optiont1}
#cd ~/gem5/gem5
#build/X86/gem5.opt  -d ${cwd}/Real  configs/example/se.py  --caches -c $cwd/${applicationName}Range${BinaryNamePost}   --options="${optiont1}"
#END
: << 'END'
[ -e ${applicationName}Warmup ] && rm ${applicationName}Warmup
g++ ${compileFlag} ${Compilefilenames} -o ${applicationName}Warmup 
time ./${applicationName}Warmup ${optiont1}
[ -e ${RangeFilePath1}_0 ] && rm ${RangeFilePath1}_0
cd ~/gem5/gem5
build/X86/gem5.opt  -d ${cwd}/Warmup  configs/example/se.py  --caches -c $cwd/${applicationName}Warmup  --options="${optiont1}"
END





