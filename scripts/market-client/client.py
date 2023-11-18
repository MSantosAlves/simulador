from monitor import Monitor
import signal
import os

import socket
import threading
import json

class Client:
    def __init__(self, server_address, server_port, trader_account, book):
        self.server_address = server_address
        self.server_port = server_port
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.trader_account = trader_account
        self.book = book
        self.monitor = Monitor(self.book)

    def connect(self):
        self.client_socket.connect((self.server_address, self.server_port))
        print("Cliente connected to the server.")

    def send_data(self, data):
        self.client_socket.sendall(data.encode())

    def receive_response(self):
        response = self.client_socket.recv(1024)
        return response.decode()

    def close(self):
        print("Client closed")
        self.client_socket.close()
    
    def start_receive_thread(self):
        receive_thread = threading.Thread(target=self.receive_data_thread)
        receive_thread.daemon = False
        receive_thread.start()

    def receive_data_thread(self):
        while True:
            try:
                response = self.client_socket.recv(int(1024*1e6))
                if response:
                    responses = response.decode().split("}{")
                
                    #TODO: Find the error causing 2 responses at the same time
                    if len(responses) > 1:
                        responses = [res if res.startswith("{") else "{" + res for res in responses]
                        responses = [res if res.endswith("}") else res + "}" for res in responses]
                        
                    responses = [json.loads(data) for data in responses]

                    for res in responses:
                    
                        self.monitor.handle_event(res)
                        self.trader_account.handle_event(res)

                        if res["event"].startswith("UPDATE_BOOK"):
                            self.book.update_book(res)

                        if res["event"] == "NEW_NEGOTIATION":
                            self.book.new_negotiation(res)

                        if res["event"] == "UPDATE_MARKET_VOLUME":
                            self.book.update_market_volume(res)

                        if res["event"] == "SIMULATION_END":
                            print(self.trader_account.to_json())
                            self.close()
                            # Kill plotly server
                            os.kill(os.getpid(), signal.SIGTERM)

            except socket.error:
                # Handle socket error (e.g., server closed the connection)
                print("Server disconnected.")
                break