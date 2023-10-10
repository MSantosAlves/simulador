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

        volume_directory_path = "./scripts/market-client/market-volume"
        volume_files = [filename for filename in os.listdir(volume_directory_path) if os.path.isfile(os.path.join(volume_directory_path, filename))]
        volume_files.remove(".gitkeep")
        self.volume_file_path = volume_directory_path + "/" + sorted(volume_files)[-1]

    def start_plotting_thread(self):
        plotting_thread = threading.Thread(target=self.start_plotting)
        plotting_thread.daemon = True
        plotting_thread.start()

    def start_plotting(self):
       
        self.app.layout = html.Div(
            html.Div([
                html.H4('Simulador [B3]'),
                dcc.Graph(id='stock-price-graph'),
                dcc.Graph(id='stock-market-offers-graph'),
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
            fig = px.line(symbol_data, x='Time', y='Price', title='{} Price over Time'.format(self.symbol))
            fig.update_xaxes(title_text='Time', tickformat='%H:%M:%S.%f', tickvals=symbol_data['Time'][::100])
            fig.update_yaxes(title_text='Price')
            
            return fig
        
        @callback(Output('stock-market-offers-graph', 'figure'),
                      Input('interval-component', 'n_intervals'))
        def update_volme_graph_live(n):

            df = pd.read_csv(self.volume_file_path)

            df = df[df['Symbol'] == self.symbol]

            fig = go.Figure()

            buy_data = df[df['Direction'] == 'BUY']
            fig.add_trace(go.Scatter(x=buy_data['Price'], y=buy_data['Quantity'], mode='lines', fill='tozeroy',
                                     line=dict(color='blue'), name='Oferta de Compra'))

            sale_data = df[df['Direction'] == 'SALE']
            fig.add_trace(go.Scatter(x=sale_data['Price'], y=sale_data['Quantity'], mode='lines', fill='tozeroy',
                                     line=dict(color='red'), name='Oferta de Venda'))

            fig.update_layout(title='Preço vs Quantidade em oferta ({})'.format(self.symbol),
                              xaxis_title='Preço',
                              yaxis_title='Quantidade',
                              xaxis=dict(tickformat='.2f'))

            return fig
        
        self.app.run_server(debug=False)