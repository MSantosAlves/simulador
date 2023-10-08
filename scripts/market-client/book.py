import time  
from datetime import datetime
from filehandler import FileHandler

class Book:
    def __init__(self):
        self.stocks = {}
        self.stocks_last_price = {}

        directory_path = "./scripts/market-client/execution-history"
        self.history_file_name = "{}/history_{}.csv".format(directory_path, str(time.time()).split(".")[0])
        self.history_file_handler = FileHandler("{}".format(self.history_file_name))
        self.history_file_handler.create_file()
        self.history_file_handler.open_file()
        self.history_file_handler.write_to_file("Symbol,Price,Time\n")

        directory_path = "./scripts/market-client/market-volume"
        self.volume_file_name = "{}/volume_{}.csv".format(directory_path, str(time.time()).split(".")[0])
        self.volume_file_handler = FileHandler("{}".format(self.volume_file_name))
        self.volume_file_handler.create_file()
        self.volume_file_handler.open_file()

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
            self.history_file_handler.write_to_file(data_to_write)
            
    def update_market_volume(self, data):
        header = "Symbol,Price,Quantity,Direction\n"
        data_to_write = header + "\n".join(["AFSF20,{},{},{}".format(el["price"], el["quantity"], el["direction"]) for el in  data["market_volume"]["AFSF20"]])
        self.volume_file_handler.file.seek(0)
        self.volume_file_handler.write_to_file(data_to_write)
        self.volume_file_handler.truncate()

