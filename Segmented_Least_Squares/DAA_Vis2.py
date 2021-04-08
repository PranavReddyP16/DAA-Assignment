import numpy as np
import matplotlib.pyplot as plt
import pylab as pl
from matplotlib.collections import PatchCollection
from matplotlib import collections as mc

#creating plots and opening temporary file under read
fig,ax = plt.subplots()
currentAxis = plt.gca()
file = open("data.txt","r")

number_of_points = 0
number_of_lines = 0
plotting_X = list()
plotting_Y = list()
lines = list()

#read the number of points along with the points in the form (x,y)
number_of_points = int(file.readline())

for i in range(number_of_points):
    points = file.readline().split(" ")
    plotting_X.append(float(points[0]))
    plotting_Y.append(float(points[1]))

for i in range(number_of_points):
    plt.plot(plotting_X[i], plotting_Y[i], 'bo')  # plot x and y using blue circle markers

'''
input the number of lines along with the line. The line is given in the form of a start point
and an end point
'''
number_of_lines = int(file.readline())

for i in range(number_of_lines):
    temp = list()
    x1,y1,x2,y2 = file.readline().split()
    temp.append((float(x1),float(y1)))
    temp.append((float(x2),float(y2)))
    lines.append(temp)

cost_val = float(file.readline())

plt.title(f"Cost value is: {cost_val}")

#plotting the lines using the points inputted above
lc = mc.LineCollection(lines,color='y',linewidths=4)
ax.add_collection(lc)

#printing the grid as well as closing it after we are done
plt.grid()
ax.autoscale_view()
plt.show()
file.close()
