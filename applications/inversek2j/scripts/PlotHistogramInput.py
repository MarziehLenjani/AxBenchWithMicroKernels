#!/usr/bin/python

import sys
import math

import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt



# Learn about API authentication here: https://plot.ly/python/getting-started
# Find your api_key here: https://plot.ly/settings/api





if(len(sys.argv) != 2):
	printUsage()

origFilename 	= sys.argv[1]


origLines 		= open(origFilename).readlines()


origLine 	= origLines[1].rstrip()
Arraay1 =[]
Arraay2 =[]
for i in range(2,len(origLines)):

	origLine 	= origLines[i].rstrip()
	Arraay1.append(float(origLine.split("\t")[0]))
	Arraay2.append(float(origLine.split("\t")[1]))

pass;



mu, sigma = 100, 15
x = mu + sigma*np.random.randn(10000)

SMALL_SIZE = 8
MEDIUM_SIZE = 10
BIGGER_SIZE = 2

#plt.rc('font', size=SMALL_SIZE)          # controls default text sizes
#plt.rc('axes', titlesize=SMALL_SIZE)     # fontsize of the axes title
#plt.rc('axes', labelsize=MEDIUM_SIZE)    # fontsize of the x and y labels
plt.rc('xtick', labelsize=28)    # fontsize of the tick labels
plt.rc('ytick', labelsize=28)    # fontsize of the tick labels
#plt.rc('legend', fontsize=SMALL_SIZE)    # legend fontsize
#plt.rc('figure', titlesize=BIGGER_SIZE)  # fontsize of the figure title
# the histogram of the data
plt.tick_params(top='off',right='off')
#fig = plt.figure()
#ax = fig.add_axes([0,0,1,1])
#ax.axis('off')
n, bins, patches = plt.hist(Arraay1, 50, normed=1, facecolor='white', alpha=0.75,edgecolor='navy')


#ax.spines['top'].set_visible(False)
#ax.spines['right'].set_visible(False)
#ax.spines['bottom'].set_visible(False)
#ax.spines['left'].set_visible(False)
# add a 'best fit' line
#y = mlab.normpdf( bins, mu, sigma)
#l = plt.plot(bins, y, 'r--', linewidth=1)
#plt.xlabel('values',fontsize=20)
#plt.ylabel('Probability',fontsize=20)
#plt.title(r'$\mathrm{Histogram\ of\ IQ:}\ \mu=100,\ \sigma=15$')
#plt.title(r'Histogram of the input values of the inversek2j',fontsize=20)
plt.axis([-1, 2, 0, 0.01],fontsize=36 )
plt.grid(True)
#plt.show()

plt.savefig('HistoramOfFirstInput.jpg')

''''
plt.hist(Arraay1)
plt.title("Inversej2k Histogram")
plt.xlabel("Value")
plt.ylabel("Frequency")
fig = plt.gcf()
plt.show()
plot_url = py.plot_mpl(fig, filename='Input1histogram')


plt.hist(Arraay2)
plt.title("Inversej2k Histogram")
plt.xlabel("Value")
plt.ylabel("Frequency")
fig2 = plt.gcf()
plt.show()
plot_url = py.plot_mpl(fig2, filename='Input2histogram2')
'''

