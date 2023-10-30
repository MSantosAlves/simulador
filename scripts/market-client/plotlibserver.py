import signal
import plotly.express as px
import plotly.graph_objects as go
import pandas as pd
import threading
import os
from dash import Dash, dcc, html, Input, Output, callback
import plotly.graph_objects as go

class PlotlibServer:

    def __init__(self, symbol):
        self.symbol = symbol
        self.app = Dash()
        history_directory_path = "./scripts/market-client/execution-history"
        history_files = [filename for filename in os.listdir(history_directory_path) if os.path.isfile(os.path.join(history_directory_path, filename))]
        history_files.remove(".gitkeep")
        self.history_file_path = history_directory_path + "/" + sorted(history_files)[-1]

    def start_plotting_thread(self):
        plotting_thread = threading.Thread(target=self.start_plotting)
        plotting_thread.daemon = True
        plotting_thread.start()

    def start_plotting(self):
       
        self.app.layout = html.Div(
            html.Div([
                html.H4('Simulador [B3]'),
                dcc.Graph(id='stock-price-graph'),
                # dcc.Graph(id='stock-market-offers-graph'),
                dcc.Interval(
                    id='interval-component',
                    interval=3*1000, # in milliseconds
                    n_intervals=0
                )
            ])
        )

        @callback(Output('stock-price-graph', 'figure'),
                      Input('interval-component', 'n_intervals'))
        def update_history_graph_live(n):

            df = pd.read_csv(self.history_file_path)

            # Filter data for target symbol
            symbol_data = df[df['Symbol'] == self.symbol]

            # Convert the 'Time' column to datetime
            symbol_data['Time'] = pd.to_datetime(symbol_data['Time'], format="%H:%M:%S.%f")
            symbol_data['Time'] = symbol_data['Time'].dt.strftime('%H:%M:%S.%f')

            # Plot the graph using Plotly Express
            fig = px.line(symbol_data, x='Time', y='Price', title='Preço Negociado x Tempo ({})'.format(self.symbol))
            downsamplefactor = 10000 if len(symbol_data['Time']) > 10000000 else 1000
            fig.update_xaxes(title_text='Time', tickformat='%H:%M:%S.%f', tickvals=symbol_data['Time'][::downsamplefactor])
            fig.update_yaxes(title_text='Price')
            
            return fig
        
        self.app.run_server(debug=False)