import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
from matplotlib.collections import PatchCollection
from matplotlib import collections as mc
from matplotlib.ticker import (AutoMinorLocator, MultipleLocator)
import pylab as pl


number_of_rectangles = 0
fig,ax = plt.subplots()
currentAxis = plt.gca()

file = open("data.txt","r")

number_of_rectangles = int(file.readline())

for i in range(number_of_rectangles):
    lx,ux,ly,uy = file.readline().split()
    w = int(ux) - int(lx)
    h = int(uy) - int(ly)
    currentAxis.add_patch(Rectangle((int(lx),int(ly)),int(w),int(h),alpha=0.5,color='#A9A9A9'))

measure_val = int(file.readline())

number_of_edges = int(file.readline())
lines = list()

for i in range(number_of_edges):
    temp = list()
    x1,y1,x2,y2 = file.readline().split()
    temp.append((int(x1),int(y1)))
    temp.append((int(x2),int(y2)))
    lines.append(temp)


lc = mc.LineCollection(lines,color='r',linewidths=4)
ax.add_collection(lc)


#ax.xaxis.set_major_locator(MultipleLocator(1))
#ax.yaxis.set_major_locator(MultipleLocator(1))
plt.axis('equal')
plt.grid(alpha=1)
ax.autoscale_view()
plt.title(f"Measure value is {measure_val}")
plt.show()
file.close()
