import os
import json

path = os.getcwd()

target_data_path = path + '/data/b3/20191220/processed'

file_list = [f for f in os.listdir(target_data_path) if os.path.isfile(os.path.join(target_data_path, f))]

file_list.remove(".gitkeep")

final_data = {}

for file_name in file_list:
    buy_file = "CPA" in file_name
    file_path = target_data_path + '/' + file_name
    new_file_name = file_name.split("_processed.json")[0] + ".txt"
    
    with open(file_path, 'r') as json_file:
        data = json.load(json_file)

    for stock_symbol in data.keys():
        if stock_symbol not in final_data:
            final_data[stock_symbol] = {}
            final_data[stock_symbol]["buy_files_info"] = {}
            final_data[stock_symbol]["sell_files_info"] = {}

            if buy_file:
                final_data[stock_symbol]["buy_files"] = [new_file_name]
                final_data[stock_symbol]["buy_files_info"][new_file_name] = {}
                final_data[stock_symbol]["buy_files_info"][new_file_name] = {
                    "first_line_index": data[stock_symbol]["first_line_index"],
                    "last_line_index": data[stock_symbol]["last_line_index"],
                }
            else:
                final_data[stock_symbol]["sell_files"] = [new_file_name]
                final_data[stock_symbol]["sell_files_info"][new_file_name] = {}
                final_data[stock_symbol]["sell_files_info"][new_file_name] = {
                    "first_line_index": data[stock_symbol]["first_line_index"],
                    "last_line_index": data[stock_symbol]["last_line_index"],
                }
        else:
            if "buy_files" not in final_data[stock_symbol]:
                final_data[stock_symbol]["buy_files"] = []
            elif "sell_files" not in final_data[stock_symbol]:
                final_data[stock_symbol]["sell_files"] = []

            if buy_file and new_file_name not in final_data[stock_symbol]["buy_files_info"]:
                final_data[stock_symbol]["buy_files_info"][new_file_name] = {}
                final_data[stock_symbol]["buy_files_info"][new_file_name] = {
                    "first_line_index": data[stock_symbol]["first_line_index"],
                    "last_line_index": data[stock_symbol]["last_line_index"],
                }
            elif not buy_file and new_file_name not in final_data[stock_symbol]["buy_files_info"]:
                final_data[stock_symbol]["sell_files_info"][new_file_name] = {}
                final_data[stock_symbol]["sell_files_info"][new_file_name] = {
                    "first_line_index": data[stock_symbol]["first_line_index"],
                    "last_line_index": data[stock_symbol]["last_line_index"],
                }

            if buy_file:
                final_data[stock_symbol]["buy_files"].append(new_file_name)
            else:
                final_data[stock_symbol]["sell_files"].append(new_file_name)
        
json_object = json.dumps(dict(sorted(final_data.items())), indent=4)
destiny_path = path + '/data/b3/20191220/compiled/INDEX_STOCK_FILES.json'

with open(destiny_path, 'w') as f:
    f.write(json_object)