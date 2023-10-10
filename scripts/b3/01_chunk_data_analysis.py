import os
import json 

path = os.getcwd()

target_data_directories = ["cpa", "vda"]

for data_dir in target_data_directories:
    directory_path = path + '/data/b3/20191220/raw/' + data_dir
    file_list = [f for f in os.listdir(directory_path) if os.path.isfile(os.path.join(directory_path, f))]
    file_list = [f for f in file_list if f not in ["CPA_MOCK.txt", "VDA_MOCK.txt", ".gitkeep"]]

    for file_name in file_list:
    
        file_path =  directory_path + '/' + file_name

        file_info = {}
        default_stock_info = {
            "min_buy_price": -1,
            "max_buy_price": -1,
            "min_sale_price": -1,
            "max_sale_price": -1,
            "buy_offers": 0,
            "sales_offers": 0
        }
        line_data = []

        with open(file_path, 'r') as file:
            line_idx = 1
            for line in file:
                line_data = line.split(";")

                stock_symbol = line_data[1].strip()
                stock_direction = int(line_data[2])
                stock_price = float(line_data[8].strip())
                stock_qty = int(line_data[9])

                if stock_symbol not in file_info:
                    file_info[stock_symbol] = {
                        "min_buy_price": -1,
                        "max_buy_price": -1,
                        "min_sale_price": -1,
                        "max_sale_price": -1,
                        "buy_offers": 0,
                        "sales_offers": 0,
                        "first_line_index": line_idx,
                        "last_line_index": line_idx
                    }
                else:
                    file_info[stock_symbol]["last_line_index"] = line_idx

                line_idx += 1

                if stock_direction == 1:
                    file_info[stock_symbol]["buy_offers"] += 1
                else:
                    file_info[stock_symbol]["sales_offers"] += 1

                if stock_direction == 1:
                    if file_info[stock_symbol]["min_buy_price"] < 0 or (stock_price < file_info[stock_symbol]["min_buy_price"] and stock_price != 0.0):
                        file_info[stock_symbol]["min_buy_price"] = stock_price

                    if file_info[stock_symbol]["max_buy_price"] < 0 or (stock_price > file_info[stock_symbol]["max_buy_price"] and stock_price != 0.0):
                        file_info[stock_symbol]["max_buy_price"] = stock_price
                else:
                    if file_info[stock_symbol]["min_sale_price"] < 0 or (stock_price < file_info[stock_symbol]["min_sale_price"] and stock_price != 0.0):
                        file_info[stock_symbol]["min_sale_price"] = stock_price

                    if file_info[stock_symbol]["max_sale_price"] < 0 or (stock_price > file_info[stock_symbol]["max_sale_price"] and stock_price != 0.0):
                        file_info[stock_symbol]["max_sale_price"] = stock_price

        json_object = json.dumps(file_info, indent = 4)

        file_name_without_extension = file_name.split(".txt")[0]
        destiny_path =  path + '/data/b3/20191220/processed/' + file_name_without_extension + "_processed.json"

        with open(destiny_path, 'w') as f:
            f.write(json_object)

        print('Last file processed: {}'.format(file_name_without_extension))