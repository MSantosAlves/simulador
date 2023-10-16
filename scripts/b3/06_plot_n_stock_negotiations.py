import os
import matplotlib.pyplot as plt
import numpy as np

path = os.getcwd()

target_data_path = path + '/data/b3/20191220/raw/neg/NEG_BMF_20191220_00.txt'
data_line = []

data_line = []
data_plot_x = []
data_plot_y = []

target_stock_symbol = "DOLF20"
with open(target_data_path, 'r') as data_file:
    for line in data_file:
        data_line = line.split(";")
        stock_symbol = data_line[1].strip()
        trade_cancelled = data_line[6] == 2

        if trade_cancelled or stock_symbol != target_stock_symbol:
            continue

        stock_price = float(data_line[3])
        trade_time = data_line[5]

        data_plot_x.append(trade_time)
        data_plot_y.append(stock_price)

fig, ax = plt.subplots()

size = 500

downsample_factor = len(data_plot_x) // size

x_values = data_plot_x[::downsample_factor]
y_values = data_plot_y[::downsample_factor]

ax.plot(x_values,y_values)

plt.xticks(ticks=[x_values[0], x_values[-1]])
plt.yticks(np.arange(min(y_values), max(y_values), (max(y_values) - min(y_values)) // 10))

ax.set(xlabel='Time (20/12/2019)', ylabel='Price (R$)',
       title='{} Price variation over time (Historic Data)'.format(target_stock_symbol))
ax.grid()

filename = path + '/data/plots/' + target_stock_symbol + "_first_{}_price_history_plot.png".format(size)
print(filename)
fig.savefig(filename)


