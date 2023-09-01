import os
import json

path = os.getcwd()

target_data_path = path + '/data/b3/20191220/' + 'PROCESSED'

file_list = [f for f in os.listdir(target_data_path) if os.path.isfile(os.path.join(target_data_path, f))]

final_data = {}

for file_name in file_list:
    buy_file = "CPA" in file_name
    file_path = target_data_path + '/' + file_name
    with open(file_path, 'r') as json_file:
        data = json.load(json_file)

    for stock_symbol in data.keys():
        if stock_symbol not in final_data:
            final_data[stock_symbol] = {}
            if buy_file:
                final_data[stock_symbol]["buy_files"] = [file_name]
            else:
                final_data[stock_symbol]["sell_files"] = [file_name]
            
        else:
            if "buy_files" not in final_data[stock_symbol]:
                final_data[stock_symbol]["buy_files"] = []
            elif "sell_files" not in final_data[stock_symbol]:
                final_data[stock_symbol]["sell_files"] = []

            if buy_file:
                final_data[stock_symbol]["buy_files"].append(file_name)
            else:
                final_data[stock_symbol]["sell_files"].append(file_name)

json_object = json.dumps(final_data, indent=4)
destiny_path = path + '/data/b3/20191220/PROCESSED/COMPILED/' + "INDEX_STOCK_FILES.json"

with open(destiny_path, 'w') as f:
    f.write(json_object)