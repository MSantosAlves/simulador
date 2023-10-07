import time  
from datetime import datetime
from filehandler import FileHandler

class Book:
    def __init__(self):
        self.stocks = {}
        self.stocks_last_price = {}

        directory_path = "./scripts/market-client/execution-history"
        self.file_name = "{}/{}.csv".format(directory_path, str(time.time()).split(".")[0])
        self.file_handler = FileHandler("{}".format(self.file_name))
        self.file_handler.create_file()
        self.file_handler.open_file()
        self.file_handler.write_to_file("Symbol,Price,Time\n")

    def update_book(self, data):
        symbol = data["symbol"]
        last_trade_price = float(data["last_trade_price"])

        self.stocks[symbol] = {
            "traded_volume": data["traded_volume"],
            "buy_offers": data["buy_offers"],
            "sell_offers": data["sell_offers"],
            "bid": float(data["bid"]),
            "ask": float(data["ask"]),
            "last_trade_price": last_trade_price,
        }

        if last_trade_price!= 0 and (symbol not in self.stocks_last_price or self.stocks_last_price[symbol] != last_trade_price):
            self.stocks_last_price[symbol] = last_trade_price
            data_to_write = "{},{},{}\n".format(symbol, last_trade_price, datetime.utcnow().strftime('%T.%f')[:-3])
            self.file_handler.write_to_file(data_to_write)
            
        

