import json
import os

from client import Client
from book import Book
from homebroker import HomeBroker
from trader_account import TraderAccount
from plotlibserver import PlotlibServer


path = os.getcwd()
user_config_path = path + "/config.json"

with open(user_config_path, 'r') as json_file:
        data = json.load(json_file)

target_symbol = data["targetStocks"][0]

server_address = '127.0.0.1'
server_port = 8080

# Start Book
book = Book(target_symbol)

# Start Trader account
trader_account = TraderAccount(book)

# Start client
client = Client(server_address, server_port, trader_account, book)
client.connect()
client.start_receive_thread() 

# Start HomeBroker
broker = HomeBroker(client)
broker.start_application_thread()

# Start plotserver
plotlib_server = PlotlibServer(target_symbol)
plotlib_server.start_plotting_thread()