mkdir MultiStepSize
for j in {-20..0}
do
bash CompilAndRun.sh $j >> MultiStepSize/errorrate${j}
done
