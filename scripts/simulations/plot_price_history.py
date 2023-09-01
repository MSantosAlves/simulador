import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

path = os.getcwd()
directory_path = path + '/data/history'  # Replace with the actual directory path
file_list = [f for f in os.listdir(directory_path) if os.path.isfile(os.path.join(directory_path, f))]
file_name = file_list[-1]

file_path =  path + '/data/history/' + file_name
point_nb = 1

data_Y = []

with open(file_path, 'r') as file:
    for line in file:
        data_points = line.strip().split(';')
        data_Y.append(float(data_points[2]) / 100)

data_X =  [i for i in range(1, len(data_Y) + 1)]

fig, ax = plt.subplots()
ax.plot(data_X, data_Y)

ax.set(xlabel='Ficticious time', ylabel='Price (R$)',
       title='Stock price over time')
ax.grid()

filename = path + '/data/history/plots/' + file_name.split('.txt')[0] + "_plot.png"
fig.savefig(filename)
