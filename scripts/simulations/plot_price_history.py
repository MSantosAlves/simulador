import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os
import math

path = os.getcwd()
directory_path = path + '/data/history'  # Replace with the actual directory path
file_list = [f for f in os.listdir(directory_path) if os.path.isfile(os.path.join(directory_path, f))]
file_name = file_list[-1]

file_path =  path + '/data/history/' + file_name

data_Y = []

with open(file_path, 'r') as file:
    for line in file:
        data_points = line.strip().split(';')
        price = float(data_points[1]) / 100
        if price > 0:
            data_Y.append(price)

# TODO: improve this logic
# 540 = Number of data points in a typicall stock plot
#if len(data_Y) > 540:
#    m = math.floor(len(data_Y) / 540)
#    data_Y = np.array(data_Y)[0: m * 540].reshape(-1, m).mean(axis=1)

data_X =  [i for i in range(1, len(data_Y) + 1)]

fig, ax = plt.subplots()
ax.plot(data_X, data_Y)

ax.set(xlabel='Ficticious time', ylabel='Price (R$)',
       title='Stock price over time')
ax.grid()

filename = path + '/data/history/plots/' + file_name.split('.txt')[0] + "_plot.png"
print(filename)
fig.savefig(filename)
