#!/bin/bash
applicationName=inversek2j
Compilefilenames="inversek2j.cpp kinematics.cpp"
BaseDirectory=/if22/ml2au/AxBenchCPU/axbench/applications/${applicationName}/ 
cd ${BaseDirectory}src 
cwd=$(pwd)
RangeFilePath=${cwd}/Rangefiles/
mkdir -p ${RangeFilePath}
[ -e ${RangeFilePath1}_0 ] && rm ${RangeFilePath1}_0
RangeFilePath1=${RangeFilePath}1.txt
optiont1="${BaseDirectory}test.data/input/theta_1000K.data  ${RangeFilePath1}_0 32 32 1"
compileFlag="-static -std=c++11"
#: << 'END'
[ -e ${applicationName}Range ] && rm ${applicationName}Range
g++ ${compileFlag} ${Compilefilenames} -o ${applicationName}Range 
time ./${applicationName}Range ${optiont1} 
#cd ~/gem5/gem5
cd ~/gem5/UndertestBetaGemv1
build/X86/gem5.opt  -d ${cwd}/Real  configs/example/se.py --cpu-type=detailed --warmup-insts=100000 --reducedVarSize1=32 --rangeFileName=${RangeFilePath1} --caches -c $cwd/${applicationName}Range  --options="${optiont1}"
#END

: << 'END'
[ -e ${applicationName}Warmup ] && rm ${applicationName}Warmup
g++ ${compileFlag} ${Compilefilenames} -o ${applicationName}Warmup 
time ./${applicationName}Warmup ${optiont1}
[ -e ${RangeFilePath1}_0 ] && rm ${RangeFilePath1}_0
cd ~/gem5/gem5
build/X86/gem5.opt  -d ${cwd}/Warmup  configs/example/se.py  --caches -c $cwd/${applicationName}Warmup  --options="${optiont1}"
END

: << 'END'
#!/bin/bash
BaseDirectory=/bigtemp/ml2au/AxBench/AxBenchCPU/axbench/applications/inversek2j/ 
cd ${BaseDirectory}src 
compileFlag="-static -std=c++11"
g++ ${compileFlag} inversek2j.cpp kinematics.cpp  -o inversek2jRange  
./inversek2jRange  /bigtemp/ml2au/AxBench/AxBenchCPU/axbench/applications/inversek2j/test.data/input/theta_1000K.data RangeOut.txt 

#./inversek2jAx ${inputFile} ${outputdir}/AxOutOrg 0
END