

#!/bin/bash
applicationName=jpeg
Compilefilenames="encoder.c dct.c huffman.c jpeg.c  marker.c quant.c rgbimage.c"
BaseDirectory=/if22/ml2au/AxBenchCPU/axbench/applications/${applicationName}/ 
cd ${BaseDirectory}src 
cwd=$(pwd)
RangeFilePath=${cwd}/Rangefiles/
mkdir -p ${RangeFilePath}
[ -e ${RangeFilePath1}_0 ] && rm ${RangeFilePath1}_0
RangeFilePath1=${RangeFilePath}1.txt
optiont1="${BaseDirectory}test.data/input/5.rgb ${RangeFilePath1}_0 32 32 1"
compileFlag="-static -std=c++11"
#: << 'END'
[ -e ${applicationName}Range ] && rm ${applicationName}Range
g++ ${compileFlag} ${Compilefilenames} -o ${applicationName}Range 
time ./${applicationName}Range ${optiont1} 
#cd ~/gem5/gem5
cd ~/gem5/UndertestBetaGemv1
build/X86/gem5.opt  -d ${cwd}/Real  configs/example/se.py  --maxinsts=500000000 --reducedVarSize1=2 --rangeFileName=${RangeFilePath1} --cpu-type=detailed --mem-type=DDR3_1600_x64 --caches --l2cache --l3cache --l1d_size=32kB --l2_size=256kB --l3_size=2MB --cacheline_size=64 -c $cwd/${applicationName}Range  --options="${optiont1}"
#END
: << 'END'
#!/bin/bash
BaseDirectory=/bigtemp/ml2au/AxBench/AxBenchCPU/axbench/applications/jpeg/ 
inputFile=${BaseDirectory}test.data/input/5.rgb
cd ${BaseDirectory}src 
compileFlag="-static -std=c++11"
g++ ${compileFlag} encoder.c dct.c huffman.c jpeg.c  marker.c quant.c rgbimage.c -o jpegRange

time ./jpegRange ${inputFile} RangeOutput.txt
END