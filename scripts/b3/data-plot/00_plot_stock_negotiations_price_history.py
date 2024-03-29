import os
import json 
import matplotlib.pyplot as plt
import numpy as np

path = os.getcwd()

target_data_path = path + '/data/b3/20191220/compiled/B3_NEG_DATA.json'
stock_neg_data = {}
data_line = []

data_plot_x = []
data_plot_y = []

stock_symbol = "WDOF20"
with open(target_data_path, 'r') as data_file:
    data = json.load(data_file)

for hour in data[stock_symbol]["prices"].keys():
    for minute in data[stock_symbol]["prices"][hour].keys():
        hours = "0" + hour if int(hour) < 10 else hour
        minutes = "0" + minute if int(minute) < 10 else minute
        time = hours + ":" + minutes
        price = float(data[stock_symbol]["prices"][hour][minute]["price"])

        data_plot_x.append(time)
        data_plot_y.append(price)

fig, ax = plt.subplots()
ax.plot(data_plot_x, data_plot_y)

plt.xticks(ticks=[data_plot_x[0], data_plot_x[-1]])
plt.yticks(np.arange(min(data_plot_y), max(data_plot_y), (max(data_plot_y) - min(data_plot_y)) // 10))

ax.set(xlabel='Time (1 price per minute)', ylabel='Price (R$)',
       title='{} Price x Time (Historic Data)'.format(stock_symbol))
ax.grid()

filename = path + '/data/plots/' + stock_symbol + "_price_history_plot.png"
fig.savefig(filename)
print(filename)

if stock_symbol not in data:
    raise Exception("Stock Symbol {} not found in negotiations file.".format(stock_symbol))

