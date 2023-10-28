import datetime
import pandas as pd
import numpy as np
import xarray as xr
import datashader as ds
import datashader.transfer_functions as tf
from datashader.transfer_functions import set_background

###########################################################################3
url = "https://raw.githubusercontent.com/MSantosAlves/simulador/master/book-data/book_history_1698431553.csv"
df_book = pd.read_csv(url)
ask_df = df_book[df_book['Type'] == 'ASK']
bid_df = df_book[df_book['Type'] == 'BID']
###################################################################################
cols = ['BidPrice', 'AskPrice']
start = datetime.datetime(2023, 10, 1, 0)

df = pd.read_csv(url)

# Create a dataframe
#data['Time'] = [start + datetime.timedelta(minutes=1)*i for i in range(len(df_book['Price']))]
df['ITime'] = pd.to_datetime([start + datetime.timedelta(minutes=1)*i for i in range(len(df_book['Price']))]).astype('int64')
df['BidPrice'] = bid_df['Price']
df['AskPrice'] = ask_df['Price']

# Default plot ranges:
x_range = (df.iloc[0].ITime, df.iloc[-1].ITime)
y_range = (df['Price'].min(), df['Price'].max())

cvs = ds.Canvas(x_range=x_range, y_range=y_range, plot_height=300, plot_width=900)
aggs= dict((c, cvs.line(df, 'ITime', c)) for c in cols)

colors = ["red", "blue"]
imgs = [tf.shade(aggs[i], cmap=[c]) for i, c in zip(cols, colors)]

img = tf.stack(*imgs)
img = set_background(img, "black")
img