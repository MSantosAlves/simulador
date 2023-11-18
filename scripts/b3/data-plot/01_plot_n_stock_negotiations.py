import os
import matplotlib.pyplot as plt
import numpy as np

path = os.getcwd()

target_data_path = path + '/data/b3/20191220/raw/neg/NEG_BMF_20191220_00.txt'
data_line = []

data_line = []
data_plot_x = []
data_plot_y = []

target_stock_symbol = "WING20"
with open(target_data_path, 'r') as data_file:
    for line in data_file:
        data_line =  line.split(";")

        if len(data_line) == 1:
            continue

        stock_symbol = data_line[1].strip()

        trade_cancelled = data_line[6] == 2

        if trade_cancelled or stock_symbol != target_stock_symbol:
            continue

        stock_price = float(data_line[3])
        trade_time = data_line[5]

        data_plot_y.append(float(stock_price))

size = len(data_plot_y)

downsample_factor = len(data_plot_y) // size

y_values = data_plot_y[::downsample_factor]
x_values = [i for i in range(0, len(y_values))]

fig, ax = plt.subplots()

ax.plot(x_values,y_values)

plt.xticks(ticks=[x_values[0], x_values[-1]])
plt.yticks(np.arange(min(y_values), max(y_values), (max(y_values) - min(y_values)) / 10))

print('{} Price variation over time (Real World Data)'.format(target_stock_symbol))
ax.set(xlabel='Time'.format(downsample_factor), ylabel='Price (R$)',
       title='{} Price x Time (Historic Data)'.format(target_stock_symbol))
ax.grid()

filename = path + '/data/plots/' + target_stock_symbol + "_negotiations_plot.png"
print(filename)
fig.savefig(filename)



