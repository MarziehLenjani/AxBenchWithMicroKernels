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


if(len(sys.argv) != 2):
	printUsage()

origFilename 	= sys.argv[1]


origLines 		= open(origFilename).readlines()


e = 0.0
absError = 0.0
MaxAbsoulteErrorVar1=0.0
MaxAbsoulteErrorVar2=0.0
AbsoulteError=0.0
index1=0
index2=0
'''
pMask=0xFF800000
nMask=0x00000000
if(nmberOfBits>0):
	for i in range (0,nmberOfBits):
		pMask=pMask+ math.pow(2,23-ti);
	for i in range (0,23-numBit):
		nMask=nMask+ math.pow(2,ti)
else:
	pMask=0xFFFFFFFFFFFFFFFF;
	nMask=0x0000000000000000;		
'''
MaxItem1=0
MaxItem2=0
MaxExponentItem1=0
MaxExponentItem2=0
origLine 	= origLines[1].rstrip()
for i in range(2,len(origLines)):

	origLine 	= origLines[i].rstrip()
	for j in range(0,18):
		origItem1 	= float(origLine.split("\t")[j])
		mantiss1, exponetnt1= math.frexp(origItem1)
	
		if(math.fabs(origItem1)>MaxItem1):
			MaxItem1=origItem1

		if(exponetnt1>MaxExponentItem1):
			MaxExponentItem1=exponetnt1
	'''
	nnItem1 	= origItem1
	nnItem2 	= origItem2
	if(origItem1>0):
		for (tmpC in range(0,nmberOfBits)):
			(nnItem1 & ~(1 << nmberOfBits)) 		
	if(origItem1<0):
		(nnItem1 & (1 << nmberOfBits)) 
		
	if(origItem2>0):
		for (tmpC in range(0,nmberOfBits)):
			(nnItem2 & ~(1 << nmberOfBits)) 
		
	if(origItem2<0):
		(nnItem2 & (1 << nmberOfBits)) 	

		'''


pass;

#print bcolors.FAIL	+ "*** RelError: %1.8f" % (absError/len(origLines)) + "    "+ bcolors.ENDC
#print bcolors.FAIL	+"*** AbsoulteError1:    %f" % (AbsoulteError) + "    "+ bcolors.ENDC
#print bcolors.FAIL	+"*** MaxAbsoulteErrorVar1:    %f" % (MaxAbsoulteErrorVar1) + "    "+ bcolors.ENDC
#print bcolors.FAIL	+"*** MaxAbsoulteErrorVar2:    %f" % (MaxAbsoulteErrorVar2) + "    "+ bcolors.ENDC
#print bcolors.FAIL	+"*** index1:    %f" % (index1) + "    "+ bcolors.ENDC
#print bcolors.FAIL	+"*** index2:    %f" % (index2) + "    "+ bcolors.ENDC
print "max exponent is %d" % MaxExponentItem1+ "    "+ "max exponent is %d" % MaxExponentItem2
print "	number of bits " +"    "+"  maximum absoule Error Marginal variable 1" + "    "+ "  maximum absoule Error Marginal variable 2" + "    "+"  maximum absoule Error Quantization variable 1" +"    "+"  maximum absoule Error Quantization variable 2"
for i in range(1,32):
	print "	%d " % i+"    "+"  %f" % (math.pow(2,MaxExponentItem1)*math.pow(2,-23)*(math.pow(2,32-i)-1)) + "    "+"  %f" % (MaxItem1/math.pow(2,i+1)) 
pass;