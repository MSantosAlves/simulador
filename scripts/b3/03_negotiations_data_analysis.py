import os
import json 

path = os.getcwd()

target_data_path = path + '/data/b3/20191220/raw/neg/NEG_BMF_20191220_00.txt'
stock_neg_data = {}
data_line = []

with open(target_data_path, 'r') as data_file:
    for line in data_file:
        data_line = line.split(";")

        stock_symbol = data_line[1].strip()
        stock_price = float(data_line[3])
        traded_qty = int(data_line[4])

        trade_time = data_line[5]
        trade_time = trade_time.split(":")

        trade_hour = int(trade_time[0]) # 09 up to 17
        trade_minute = int(trade_time[1]) # 00 up to 59
        trade_seconds = float(trade_time[2]) # 00.000 up to 59.999

        if stock_symbol not in stock_neg_data:
            stock_neg_data[stock_symbol] = {}
            stock_neg_data[stock_symbol]["traded_qty"] = traded_qty
            stock_neg_data[stock_symbol]["prices"] = {}
            stock_neg_data[stock_symbol]["prices"][trade_hour] = {}
            stock_neg_data[stock_symbol]["prices"][trade_hour][trade_minute] = {}
            stock_neg_data[stock_symbol]["prices"][trade_hour][trade_minute]["price"] = stock_price
            stock_neg_data[stock_symbol]["prices"][trade_hour][trade_minute]["last_price_seconds"] = trade_seconds
        else:
            stock_neg_data[stock_symbol]["traded_qty"] += traded_qty

            if trade_hour not in stock_neg_data[stock_symbol]["prices"]:
                stock_neg_data[stock_symbol]["prices"][trade_hour] = {}
                stock_neg_data[stock_symbol]["prices"][trade_hour][trade_minute] = {}
                stock_neg_data[stock_symbol]["prices"][trade_hour][trade_minute]["price"] = stock_price
                stock_neg_data[stock_symbol]["prices"][trade_hour][trade_minute]["last_price_seconds"] = trade_seconds
            elif trade_minute not in stock_neg_data[stock_symbol]["prices"][trade_hour]:
                stock_neg_data[stock_symbol]["prices"][trade_hour][trade_minute] = {}
                stock_neg_data[stock_symbol]["prices"][trade_hour][trade_minute]["price"] = stock_price
                stock_neg_data[stock_symbol]["prices"][trade_hour][trade_minute]["last_price_seconds"] = trade_seconds
            elif stock_neg_data[stock_symbol]["prices"][trade_hour][trade_minute]["last_price_seconds"] <= trade_seconds:
                stock_neg_data[stock_symbol]["prices"][trade_hour][trade_minute]["price"] = stock_price
                stock_neg_data[stock_symbol]["prices"][trade_hour][trade_minute]["last_price_seconds"] = trade_seconds

json_object = json.dumps(stock_neg_data, indent=4)
destiny_path = path + '/data/b3/20191220/compiled/B3_NEG_DATA.json'

with open(destiny_path, 'w') as f:
    f.write(json_object)


