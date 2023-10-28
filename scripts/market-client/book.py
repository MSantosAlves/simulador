import time  
from datetime import datetime
from filehandler import FileHandler

class Book:
    def __init__(self, symbol):
        self.stocks = {}
        self.stocks_last_price = {}
        self.symbol = symbol

        file_time_discriminator = str(time.time()).split(".")[0]

        # directory_path = "./scripts/market-client/book-history"
        # self.book_history_file_name = "{}/book_history_{}.csv".format(directory_path, file_time_discriminator)
        # self.book_history_file_handler = FileHandler("{}".format(self.book_history_file_name))
        # self.book_history_file_handler.create_file()
        # self.book_history_file_handler.open_file()
        # self.book_history_file_handler.write_to_file("Symbol,Price,Type,Time\n")

        # directory_path = "./scripts/market-client/execution-history"
        # self.history_file_name = "{}/history_{}.csv".format(directory_path, file_time_discriminator)
        # self.history_file_handler = FileHandler("{}".format(self.history_file_name))
        # self.history_file_handler.create_file()
        # self.history_file_handler.open_file()
        # self.history_file_handler.write_to_file("Symbol,Price,Time\n")

        directory_path = "./scripts/market-client/market-volume"
        self.volume_file_name = "{}/volume_{}.csv".format(directory_path, file_time_discriminator)
        self.volume_file_handler = FileHandler("{}".format(self.volume_file_name))
        self.volume_file_handler.create_file()
        self.volume_file_handler.open_file()
        self.volume_file_handler.write_to_file("BuyInfo,SellInfo,Time\n")

    def update_book(self, data):
        symbol = data["symbol"]
        last_trade_price = float(data["last_trade_price"])
        update_direction = data["direction"]
        time = data["time"]

        self.stocks[symbol] = {
            "traded_volume": data["traded_volume"],
            "buy_offers_qty": data["buy_offers"],
            "sell_offers_qty": data["sell_offers"],
            "bid": float(data["bid"]),
            "ask": float(data["ask"]),
            "last_trade_price": last_trade_price,
        }

        # new_price = self.stocks[symbol][update_direction.lower()]

        # data_to_write = "{},{},{},{}\n".format(symbol, new_price, update_direction, time)
        # self.book_history_file_handler.write_to_file(data_to_write)

        # if last_trade_price!= 0 and (symbol not in self.stocks_last_price or self.stocks_last_price[symbol] != last_trade_price):
        #     self.stocks_last_price[symbol] = last_trade_price
        #     data_to_write = "{},{},{}\n".format(symbol, last_trade_price, time)
        #     self.history_file_handler.write_to_file(data_to_write)
            
    def update_market_volume(self, data):

        if self.symbol not in data["market_volume"]:
            return
            
        mv = data["market_volume"][self.symbol]
        time = data["time"]
        buy_data = [item for item in mv if item["direction"] == "BUY"]
        sale_data = [item for item in mv if item["direction"] == "SALE"]
        
        data_to_write = "BUY"

        for buy in buy_data:
            data_to_write += ";" + str(buy["price"]) + "-" + str(buy["quantity"])
        data_to_write += ",SELL"

        for sale in sale_data:
            data_to_write += ";" + str(sale["price"]) + "-" + str(sale["quantity"])
        data_to_write += "," + time + "\n"
        
        self.volume_file_handler.write_to_file(data_to_write)

