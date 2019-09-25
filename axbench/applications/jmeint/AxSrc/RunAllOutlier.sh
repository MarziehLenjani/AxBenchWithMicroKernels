mkdir IgnoreOutlier
for j in {-20..0}
do
bash CompilAndRun.sh $j >> IgnoreOutlier/ErrorRate/errorrate${j}
done