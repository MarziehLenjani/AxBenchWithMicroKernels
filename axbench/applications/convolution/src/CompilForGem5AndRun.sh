
#!/bin/bash
applicationName=convolution
Compilefilenames="convolution.cpp"
BaseDirectory=/if22/ml2au/AxBenchCPU/axbench/applications/${applicationName}/ 
cd ${BaseDirectory}src 
cwd=$(pwd)
RangeFilePath=${cwd}/Rangefiles/
mkdir -p ${RangeFilePath}
[ -e ${RangeFilePath1}_0 ] && rm ${RangeFilePath1}_0
RangeFilePath1=${RangeFilePath}1.txt
#optiont1="224 224 11 11 0 0 2 2 100 100  ${RangeFilePath1}_0 "
optiont1="224 224 11 11 1 1 2 2 3 512  ${RangeFilePath1}_0 "
compileFlag="-static -std=c++11"
#: << 'END'
[ -e ${applicationName}Range ] && rm ${applicationName}Range
g++ ${compileFlag} ${Compilefilenames} -o ${applicationName}Range 
time ./${applicationName}Range ${optiont1} 
cd ~/gem5/gem5
build/X86/gem5.opt  -d ${cwd}/Real  configs/example/se.py  --caches -c $cwd/${applicationName}Range  --options="${optiont1}"

: << 'END'
[#!/bin/bash
BaseDirectory=/bigtemp/ml2au/AxBench/AxBenchCPU/axbench/applications/convolution/ 
cd ${BaseDirectory}src 
compileFlag="-static -std=c++11"
#g++ ${compileFlag} -g convolution.cpp -o convolution
g++ ${compileFlag} -g convolutionBeforeOnePartMemory.cpp -o convolution

convOptions="224 224 11 11 0 0 2 2 100 100"
#temp runs 
#gdb ./convolution ${convOptions} 
time ./convolution ${convOptions} 

END
