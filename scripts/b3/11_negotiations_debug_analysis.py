import os
import matplotlib.pyplot as plt
import numpy as np
from datetime import datetime

path = os.getcwd()

negotiations_path = path + '/data/b3/20191220/raw/neg/NEG_BMF_20191220_00.txt'
stock_neg_data = {}
data_line = []

target_stock_symbol = "DOLF20"
target_negotiations_qty = 100
counter = 0

negotiations = {}

with open(negotiations_path, 'r') as data_file:
    for line in data_file:
        data_line = line.split(";")

        stock_symbol = data_line[1].strip()

        if stock_symbol != target_stock_symbol:
            continue

        trade_number = float(data_line[2])
        trade_price = float(data_line[3])
        traded_qty = int(data_line[4])
        trade_time = data_line[5]
        buy_order_number = data_line[8]
        sell_order_number = data_line[12]
        buy_order_agressor_indicator = data_line[10]
        sell_order_agressor_indicator = data_line[14]

        negotiations[trade_number] = {
            "price": trade_price,
            "qty": traded_qty,
            "time": trade_time,
            "buy_order_number": buy_order_number,
            "sell_order_number": sell_order_number,
            "buy_order_agressor_indicator": buy_order_agressor_indicator,
            "sell_order_agressor_indicator": sell_order_agressor_indicator,
        }

        counter += 1

        if counter == target_negotiations_qty:
            break

values = []
        
for neg in negotiations.values():
    values.append(neg["price"])

x_values = [i for i in range(0, len(values))]

target_buy_orders = [neg["buy_order_number"] for neg in negotiations.values()]
target_sell_orders = [neg["sell_order_number"] for neg in negotiations.values()]

buys_path = path + '/data/b3/20191220/raw/cpa/OFER_CPA_BMF_20191220_00_SORTED.txt'
sells_path = path + '/data/b3/20191220/raw/vda/OFER_VDA_BMF_20191220_00_SORTED.txt'

buy_offers = {}
sell_offers = {}

with open(sells_path, 'r') as data_file:
    for line in data_file:
        data_line = line.split(";")
        order_number = data_line[3]
        status = -1 if data_line[-3] == "C" else int(data_line[-3])

        if len(sell_offers.keys()) == target_negotiations_qty:
            break

        if order_number in target_sell_orders and status in [1, 2]:
            if order_number in sell_offers:
                sell_offers[order_number].append(data_line)
            else:
                sell_offers[order_number] = [data_line]

with open(buys_path, 'r') as data_file:
    for line in data_file:
        data_line = line.split(";")
        order_number = data_line[3]
        status = -1 if data_line[-3] == "C" else int(data_line[-3])

        if len(buy_offers.keys()) == target_negotiations_qty:
            break

        if order_number in target_buy_orders and status in [1, 2]:
            if order_number in buy_offers:
                buy_offers[order_number].append(data_line)
            else:
                buy_offers[order_number] = [data_line]

for count, negotiation in enumerate(negotiations.values(), start=1):
    buy_orders = buy_offers[negotiation["buy_order_number"]]
    buy_prices = [float(b[8]) for b in buy_orders]

    sell_orders = sell_offers[negotiation["sell_order_number"]]
    sell_prices = [float(s[8]) for s in sell_orders]

    print("[{}]\nNegotiation: {}".format(count, negotiation["price"]))
    print("Buy  #{}: {}".format(negotiation["buy_order_number"], buy_prices))
    print("Sell #{}: {}".format(negotiation["sell_order_number"], sell_prices))
    print("Buy  agressor: {}".format(negotiation["buy_order_agressor_indicator"]))
    print("Sell agressor: {}\n".format(negotiation["sell_order_agressor_indicator"]))

# print(values)

# plt.plot(x_values,values)
# plt.xticks(np.arange(min(x_values), max(x_values), (max(x_values) - min(x_values)) // 20))
# plt.yticks(np.arange(min(values), max(values), (max(values) - min(values)) / 20))
# plt.show()