import os
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from datetime import datetime

path = os.getcwd()

path = os.getcwd()

book_history_directory_path = "./scripts/market-client/book-history"
book_history_files = [filename for filename in os.listdir(book_history_directory_path) if os.path.isfile(os.path.join(book_history_directory_path, filename))]
book_history_files.remove(".gitkeep")
book_history_file_path = book_history_directory_path + "/" + sorted(book_history_files)[-1]

# Read the CSV file into a DataFrame
df = pd.read_csv(book_history_file_path)

# Separate the data into ASK and BID DataFrames
ask_data = df[df['Type'] == 'ASK']
bid_data = df[df['Type'] == 'BID']

size = 20

ask_downsample_factor = 1#len(ask_data) // size
bid_downsample_factor = 1#len(bid_data) // size

ask_data = ask_data[0:500]#[::ask_downsample_factor]
bid_data = bid_data[0:500]#[::bid_downsample_factor]

# Create a figure and axis
fig, ax = plt.subplots()

# Plot the ASK data in blue
# ax.step(ask_data['Time'], ask_data['Price'], label='ASK', color='blue')

# Plot the BID data in red
ax.plot(bid_data['Time'], bid_data['Price'], label='BID', color='red')


max_ask = ask_data['Price'].max()
max_bid = bid_data['Price'].max()

min_ask = ask_data['Price'].min()
min_bid = bid_data['Price'].min()

global_max = max_ask if max_ask > max_bid else max_bid
global_min = min_bid if min_bid < min_ask else min_ask

# Set labels and legend
ax.set_xlabel('Time')
ax.set_ylabel('Price')
ax.set_title('ASK vs BID')
ax.legend()

# Show the plot
plt.show()
