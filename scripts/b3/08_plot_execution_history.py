import os
import matplotlib.pyplot as plt
import numpy as np
import time
from datetime import datetime

path = os.getcwd()

history_directory_path = "./scripts/market-client/execution-history"
history_files = [filename for filename in os.listdir(history_directory_path) if os.path.isfile(os.path.join(history_directory_path, filename))]
history_files.remove(".gitkeep")
history_file_path = history_directory_path + "/" + sorted(history_files)[-1]

data_line = []
data_plot_x = []
data_plot_y = []

stock_symbol = ""

with open(history_file_path, 'r') as data_file:
    for line in data_file:
        data_line = line.split(",")
        if(data_line[0] == "Symbol"):
            continue

        stock_symbol = data_line[0].strip()
        data_plot_x.append(data_line[2].strip())
        data_plot_y.append(float(data_line[1].strip()))


size = 500#len(data_plot_y)

downsample_factor = len(data_plot_x) // size

x_values = data_plot_x[::downsample_factor]
y_values = data_plot_y[::downsample_factor]

fig, ax = plt.subplots()

ax.plot(x_values,y_values)

plt.xticks(ticks=[x_values[0], x_values[-1]])
plt.yticks(np.arange(min(y_values), max(y_values), (max(y_values) - min(y_values)) // 10))

ax.set(xlabel='Time (at 20/12/2019)', ylabel='Price (R$)',
       title='{} Price variation over time (Simulation)'.format(stock_symbol))
ax.grid()



filename = path + '/data/plots/' + stock_symbol + "_execution_history_plot_price_" + datetime.utcnow().strftime('%T.%f')[:-3] + ".png"
print(filename)
fig.savefig(filename)
