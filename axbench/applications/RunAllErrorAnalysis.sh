outputBase=/zf14/ml2au/AxBenchResults/ErrorAnalysis/
#Method=Quantization
#Method=OLSB
#Method=Sampling
Method=ByteVsExponent

OutputPath="${outputBase}${Method}"
[ -e ${OutputPath} ] && rm -rf ${OutputPath}
mkdir -p ${OutputPath}
echo fft
cd /zf14/ml2au/AxBenchCPU/axbench/applications/fft/AxSrc
bash CompilAndRun.sh >> ${OutputPath}/FFT.csv
echo blackscholes
cd /zf14/ml2au/AxBenchCPU/axbench/applications/blackscholes/AxSrc
bash CompilAndRun.sh >> ${OutputPath}/Blackscholes.csv
echo Alexnet
cd /zf14/ml2au/AxBenchCPU/axbench/applications/convolution/AxSrc
bash CompilAndRunAlex.sh >> ${OutputPath}/Alex.csv
echo VGG
cd /zf14/ml2au/AxBenchCPU/axbench/applications/convolution/AxSrc
bash CompilAndRunVGG.sh >> ${OutputPath}/VGG.csv
echo Dot
cd /zf14/ml2au/AxBenchCPU/axbench/applications/convolution/AxSrc
bash CompilAndRunDot.sh >> ${OutputPath}/Dot.csv
echo Conv
cd /zf14/ml2au/AxBenchCPU/axbench/applications/convolution/AxSrc
bash CompilAndRunConv.sh >> ${OutputPath}/Conv.csv
echo inversek2j
cd /zf14/ml2au/AxBenchCPU/axbench/applications/inversek2j/AxSrc
bash CompilAndRun.sh >> ${OutputPath}/Inversek2j.csv
echo jmeint
cd /zf14/ml2au/AxBenchCPU/axbench/applications/jmeint/AxSrc
bash CompilAndRun.sh >> ${OutputPath}/Jmeint.csv
echo kmeans
cd /zf14/ml2au/AxBenchCPU/axbench/applications/kmeans/AxSrc
bash CompilAndRun.sh >> ${OutputPath}/Kmeans.csv
echo Oldsobel
cd /zf14/ml2au/AxBenchCPU/axbench/applications/Oldsobel/AxSrc
bash CompilAndRun.sh >> ${OutputPath}/Sobel.csv


