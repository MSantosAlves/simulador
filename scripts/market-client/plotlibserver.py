from dash import Dash, dcc, html, Input, Output
import plotly.express as px
import pandas as pd
import random
import os

from dash import Dash, dcc, html, Input, Output, callback

class PlotlibServer:

    def __init__(self):
        directory_path = "./scripts/market-client/execution-history"
        self.file_path = directory_path + "/" + [filename for filename in os.listdir(directory_path) if os.path.isfile(os.path.join(directory_path, filename))][0]

    def start_plotting(self):

        app = Dash(__name__)
        app.layout = html.Div(
            html.Div([
                html.H4('Simulador'),
                html.Div(id='live-update-text'),
                dcc.Graph(id='live-update-graph'),
                dcc.Interval(
                    id='interval-component',
                    interval=1*3000, # in milliseconds
                    n_intervals=0
                )
            ])
        )


        @callback(Output('live-update-text', 'children'),
                      Input('interval-component', 'n_intervals'))
        def update_metrics(n):
            info = str(random.randint(0, 2))
            style = {'padding': '5px', 'fontSize': '16px'}
            return [
                html.Span('Teste: {}'.format(info), style=style),
            ]


        # Multiple components can update everytime interval gets fired.
        @callback(Output('live-update-graph', 'figure'),
                      Input('interval-component', 'n_intervals'))
        def update_graph_live(n):

            df = pd.read_csv(self.file_path)

            # Filter data for symbol 'AFSF20'
            symbol_data = df[df['Symbol'] == 'AFSF20']

            # Convert the 'Time' column to datetime
            symbol_data['Time'] = pd.to_datetime(symbol_data['Time'], format="%H:%M:%S.%f")
            symbol_data['Time'] = symbol_data['Time'].dt.strftime('%H:%M:%S.%f')

            # Plot the graph using Plotly Express
            fig = px.line(symbol_data, x='Time', y='Price', title='AFSF20 Price over Time')
            fig.update_xaxes(title_text='Time', tickformat='%H:%M:%S.%f')#, tickvals=symbol_data['Time'][::100]
            fig.update_yaxes(title_text='Price')

            #fig = px.line(df, x=df.index, y=df.columns, title='Price variation over time for AFSF20')
            return fig

        app.run_server(debug=True)
