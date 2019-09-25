#!/bin/bash
applicationName="axpy"
Compilefilenames="main.cpp"
#SWQNTFLAG="-DSWQNT"
#SWQNTFLAG="-DSWQNTInPlace"
SWQNTFLAG="-DAlaki"
#BinaryNamePost="SWQNT"
BaseDirectory=/zf14/ml2au/AxBenchCPU/axbench/applications/${applicationName}/ 
cd ${BaseDirectory}src 
cwd=$(pwd)
RangeFilePath=${cwd}/Rangefiles/
mkdir -p ${RangeFilePath}
[ -e ${RangeFilePath1}_0 ] && rm ${RangeFilePath1}_0
RangeFilePath1=${RangeFilePath}1.txt
optiont1="1000 ${RangeFilePath1}_0 32 32 1"
compileFlag="-static -std=c++11"
#: << 'END'
[ -e ${applicationName}Range${BinaryNamePost} ] && rm ${applicationName}Range${BinaryNamePost}
g++ ${compileFlag} "${SWQNTFLAG}"  ${Compilefilenames} -o ${applicationName}Range${BinaryNamePost} 
time ./${applicationName}Range${BinaryNamePost} ${optiont1} 
#cd ~/gem5/gem5
AddMaxInstr="--maxinsts=100000000"

cd ~/gem5/UndertestBetaGemv1
build/X86/gem5.opt  -d ${cwd}/32  configs/example/se.py  --cpu-type=detailed   --warmup-insts=100000 --reducedVarSize1=32  ${AddMaxInstr}  --rangeFileName=${RangeFilePath1} --caches -c $cwd/${applicationName}Range${BinaryNamePost}  --options="${optiont1}"
build/X86/gem5.opt  -d ${cwd}/12  configs/example/se.py  --cpu-type=detailed   --warmup-insts=100000 --reducedVarSize1=12  ${AddMaxInstr}  --rangeFileName=${RangeFilePath1} --caches -c $cwd/${applicationName}Range${BinaryNamePost}  --options="${optiont1}"


#END

:<< 'END'
[ -e ${applicationName}Warmup ] && rm ${applicationName}Warmup
g++ ${compileFlag} ${Compilefilenames} -o ${applicationName}Warmup 
time ./${applicationName}Warmup ${optiont1}
[ -e ${RangeFilePath1}_0 ] && rm ${RangeFilePath1}_0
cd ~/gem5/gem5
build/X86/gem5.opt  -d ${cwd}/Warmup  configs/example/se.py  --caches -c $cwd/${applicationName}Warmup  --options="${optiont1}"
END











:<< 'END'

#!/bin/bash
BaseDirectory=/bigtemp/ml2au/AxBench/AxBenchCPU/axbench/applications/blackscholes/ 
cd ${BaseDirectory}src 
compileFlag="-static -std=c++11"




g++ ${compileFlag} blackscholes.c  -o blackscholesRange 

inputFile=${BaseDirectory}test.data/input/blackscholesTest_400K.data 
 ./blackscholesRange ${inputFile} RangeOutput.txt 
 
 
END
