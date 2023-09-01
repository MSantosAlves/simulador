import os
import json

path = os.getcwd()

target_data_path = path + '/data/b3/20191220/' + 'PROCESSED'

file_list = [f for f in os.listdir(target_data_path) if os.path.isfile(
    os.path.join(target_data_path, f))]

final_data = {}

for file_name in file_list:
    file_path = target_data_path + '/' + file_name
    with open(file_path, 'r') as json_file:
        data = json.load(json_file)

    for stock_symbol in data:
        if stock_symbol not in final_data:
            final_data[stock_symbol] = data[stock_symbol]
        else:
            if final_data[stock_symbol]["min_buy_price"] == -1:
                final_data[stock_symbol]["min_buy_price"] = data[stock_symbol]["min_buy_price"]
            elif data[stock_symbol]["min_buy_price"] != -1 and data[stock_symbol]["min_buy_price"] < final_data[stock_symbol]["min_buy_price"]:
                final_data[stock_symbol]["min_buy_price"] = data[stock_symbol]["min_buy_price"]

            if final_data[stock_symbol]["max_buy_price"] == -1:
                final_data[stock_symbol]["max_buy_price"] = data[stock_symbol]["max_buy_price"]
            elif data[stock_symbol]["max_buy_price"] != -1 and data[stock_symbol]["max_buy_price"] > final_data[stock_symbol]["max_buy_price"]:
                final_data[stock_symbol]["max_buy_price"] = data[stock_symbol]["max_buy_price"]

            if final_data[stock_symbol]["min_sale_price"] == -1:
                final_data[stock_symbol]["min_sale_price"] = data[stock_symbol]["min_sale_price"]
            elif data[stock_symbol]["min_sale_price"] != -1 and data[stock_symbol]["min_sale_price"] < final_data[stock_symbol]["min_sale_price"]:
                final_data[stock_symbol]["min_sale_price"] = data[stock_symbol]["min_sale_price"]

            if final_data[stock_symbol]["max_sale_price"] == -1:
                final_data[stock_symbol]["max_sale_price"] = data[stock_symbol]["max_sale_price"]
            elif data[stock_symbol]["max_sale_price"] != -1 and data[stock_symbol]["max_sale_price"] > final_data[stock_symbol]["max_sale_price"]:
                final_data[stock_symbol]["max_sale_price"] = data[stock_symbol]["max_sale_price"]

            final_data[stock_symbol]["buy_offers"] += data[stock_symbol]["buy_offers"]

            final_data[stock_symbol]["sales_offers"] += data[stock_symbol]["sales_offers"]

for stock in final_data:
    final_data[stock]["min_buy_price"] = 0 if final_data[stock]["min_buy_price"] == - \
        1 else final_data[stock]["min_buy_price"]
    final_data[stock]["max_buy_price"] = 0 if final_data[stock]["max_buy_price"] == - \
        1 else final_data[stock]["max_buy_price"]
    final_data[stock]["min_sale_price"] = 0 if final_data[stock]["min_sale_price"] == - \
        1 else final_data[stock]["min_sale_price"]
    final_data[stock]["max_sale_price"] = 0 if final_data[stock]["max_sale_price"] == - \
        1 else final_data[stock]["max_sale_price"]


# MOST ORDERS PER STOCK: WING20

json_object = json.dumps(final_data, indent=4)
destiny_path = path + '/data/b3/20191220/PROCESSED/COMPILED/' + "B3_COMPILED_DATA.json"

with open(destiny_path, 'w') as f:
    f.write(json_object)
