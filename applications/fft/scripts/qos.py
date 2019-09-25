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
MinAbsError=100000
AbsoulteError=0.0
index1=1
index2=1

for i in range(len(origLines)):
#for i in range(1):
	origLine 	= origLines[i].rstrip()
	nnLine 		= nnLines[i].rstrip()


	origItem1 	= float(origLine.split(",")[0])

	nnItem1 	= float(nnLine.split(",")[0])

	origItem2	= float(origLine.split(",")[1])
	nnItem2	= 	float(nnLine.split(",")[1])

 	diff1		= origItem1 - nnItem1
	diff2		= origItem2 - nnItem2
	if (diff1==0 and  diff2==0):
		nominator=0
	else:
#		print "--------------",diff1 , diff2, origItem1 ,origItem2, nnItem1, nnItem2 
 		nominator   = math.sqrt(diff1*diff1+diff2*diff2 )
	AbsoulteError +=nominator
	if (origItem1==0 and origItem2==0) :
		denominator=0
	else:
 		denominator = math.sqrt(origItem1*origItem1+origItem2*origItem2)
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
	if(math.fabs(diff2)>MaxAbsoulteErrorVar1 and not math.isnan(diff2)):
		MaxAbsoulteErrorVar1=math.fabs(diff2)
		index1=math.fabs(origItem2)
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
	relError=(absError/len(origLines))
relAbsError=0
if(MaxAbsoulteErrorVar1==0):
        relAbsError=0
elif(index1==0 or (index1<MaxAbsoulteErrorVar1)):
        relAbsError=1
else:
        relAbsError=MaxAbsoulteErrorVar1/index1
print "  %1.8f" % relError + "    "+ "  %1.8f" % (MaxAbsoulteErrorVar1)+ "  %1.8f" % (relAbsError)
#print "index is %d" % index1  
