#!/usr/bin/python

import sys
import math


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def printUsage():
	print "Usage: python qos.py <original file> <nn file>"
	exit(1)
pass;


if(len(sys.argv) != 3):
	printUsage()

origFilename 	= sys.argv[1]
nnFilename		= sys.argv[2]

origLines 		= open(origFilename).readlines()
nnLines			= open(nnFilename).readlines()


e = 0.0
absError = 0.0
MaxAbsoulteErrorVar1=0.0
#MaxAbsoulteErrorVar2=0.0
AbsoulteError=0.0
index1=1
index2=1
count=0
for i in range(0,len(origLines)):
#for i in range(1)
	origLine 	= origLines[i].rstrip()
	nnLine 		= nnLines[i].rstrip()
	origElements=origLine.split(",")
	nnElements=nnLine.split(",")
	for j in range( len(origElements)):
		count=count+1

		origItem1 	= float(origElements[j])

		nnItem1 	= float(nnElements[j])

 		diff1		= origItem1 - nnItem1
		if (diff1==0 ):
			nominator=0
		else:
#			print "--------------",diff1 , diff2, origItem1 ,origItem2, nnItem1, nnItem2 
 			nominator   = math.sqrt(diff1*diff1 )
		AbsoulteError +=nominator
		if (origItem1==0) :
			denominator=0
		else:
 			denominator = math.sqrt(origItem1*origItem1)
	#print"  non and denom", nominator ,"    " , denominator
 		if(denominator == 0 and nominator==0):
			e =0
		elif (denominator == 0):
 			e = 1
 		elif(math.isnan(nominator)):
			if(math.isnan(denominator)):
				e=0
			else :
 				e = 1.0
 		elif ((nominator / denominator > 1)):
 			e = 1.0
 		else:
 			e = nominator / denominator

 		absError += e
		if(math.fabs(diff1)>MaxAbsoulteErrorVar1 and not math.isnan(diff1)):
			MaxAbsoulteErrorVar1=math.fabs(diff1)
			index1=math.fabs(origItem1)

pass;

#print bcolors.FAIL	+ "*** RelError: %1.8f" % (absError/len(origLines)) + "    "+ bcolors.ENDC
#print bcolors.FAIL	+"*** AbsoulteError1:    %f" % (AbsoulteError) + "    "+ bcolors.ENDC
#print bcolors.FAIL	+"*** MaxAbsoulteErrorVar1:    %f" % (MaxAbsoulteErrorVar1) + "    "+ bcolors.ENDC
#print bcolors.FAIL	+"*** MaxAbsoulteErrorVar2:    %f" % (MaxAbsoulteErrorVar2) + "    "+ bcolors.ENDC
#print bcolors.FAIL	+"*** index1:    %f" % (index1) + "    "+ bcolors.ENDC
#print bcolors.FAIL	+"*** index2:    %f" % (index2) + "    "+ bcolors.ENDC
relError=0
if (absError==0):
	relError=0;
else:
	relError=(absError/count)
relAbsError=0
if(MaxAbsoulteErrorVar1==0):
        relAbsError=0
elif(index1==0 or (index1<MaxAbsoulteErrorVar1)):
        relAbsError=1
else:
        relAbsError=MaxAbsoulteErrorVar1/index1
print "  %1.8f" % relError + "    "+ "  %1.8f" % (MaxAbsoulteErrorVar1) +"  %1.8f" % (relAbsError)
