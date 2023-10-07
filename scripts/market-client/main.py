import time

from client import Client
from book import Book
from homebroker import HomeBroker
from trader_account import TraderAccount
from plotlibserver import PlotlibServer
from filehandler import FileHandler

server_address = '127.0.0.1'
server_port = 8080

# Start Book
book = Book()

# Start Trader account
trader_account = TraderAccount(book)

# Start client
client = Client(server_address, server_port, trader_account, book)
client.connect()
client.start_receive_thread() 

# Start HomeBroker
broker = HomeBroker(client)
#broker.start_application_thread()

# Start plotserver
plotlib_server = PlotlibServer()
plotlib_server.start_plotting()