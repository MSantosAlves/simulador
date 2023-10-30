import os
import matplotlib.pyplot as plt
import numpy as np

path = os.getcwd()

history_directory_path = "./scripts/market-client/execution-history"
history_files = [filename for filename in os.listdir(history_directory_path) if os.path.isfile(os.path.join(history_directory_path, filename))]
history_files.remove(".gitkeep")
history_file_path = history_directory_path + "/" + sorted(history_files)[-1]

data_line = []
data_plot_x = []
data_plot_y1 = []
data_plot_y2 = []

target_stock_symbol = ""
with open(history_file_path, 'r') as data_file:
    for line in data_file:
        data_line = line.split(",")
        if(data_line[0] == "Symbol"):
            continue;
        
        if target_stock_symbol == "":
            target_stock_symbol = data_line[0]

        data_plot_y1.append(float(data_line[1].strip()))


target_data_path = path + '/data/b3/20191220/raw/neg/NEG_BMF_20191220_00.txt'
with open(target_data_path, 'rb') as data_file:
    for line in data_file:

        if len(line) < 230:
            continue
        
        data_line = line.decode('utf-8').split(";")
        stock_symbol = data_line[1].strip()
        trade_cancelled = data_line[6] == 2

        if trade_cancelled or stock_symbol != target_stock_symbol:
            continue

        stock_price = float(data_line[3])
        trade_time = data_line[5]

        data_plot_y2.append(float(stock_price))

size = len(data_plot_y1)#500

y_values1 = data_plot_y1[::len(data_plot_y1) // size]

downsample_factor = len(data_plot_y2) // len(y_values1)

y_values2 = data_plot_y2[::downsample_factor]
x_values = [i for i in range(0, len(y_values1))]

fig, ax = plt.subplots()

movement = 0

x_values1 = [i + movement for i in range(0, len(y_values1))]
x_values2 = [i for i in range(0, len(y_values2))]

ax.plot(x_values1, y_values1, label='Simulated)')
ax.plot(x_values2, y_values2, label='Real')

#plt.xticks(1, 2)
plt.yticks(np.arange(min(y_values1), max(y_values1), (max(y_values1) - min(y_values1)) // 10))

ax.set(xlabel='Time (at 20/12/2019)', ylabel='Price (R$)',
       title='{} Price variation over time'.format(target_stock_symbol))
ax.grid()

auc_1 = np.trapz(y_values1, x_values1)
auc_2 = np.trapz(y_values2, x_values2)
error = (auc_2 - auc_1)/auc_2

print("AUC (SIMULATED): ", auc_1)
print("AUC (REAL)     : ", auc_2)
print("AUC Error (%)  :  {}%".format(error * 100))

filename = path + '/data/plots/' + target_stock_symbol +"_comparison_ignore_plot_error_" "{:.2f}".format(error * 100) + ".png"
print(filename)

fig.savefig(filename)
