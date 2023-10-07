from monitor import Monitor

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
        print("Server connected")

    def send_data(self, data):
        self.client_socket.sendall(data.encode())

    def receive_response(self):
        response = self.client_socket.recv(1024)
        return response.decode()

    def close(self):
        print("Server closed")
        self.client_socket.close()
    
    def start_receive_thread(self):
        receive_thread = threading.Thread(target=self.receive_data_thread)
        receive_thread.daemon = False
        receive_thread.start()

    def receive_data_thread(self):
        while True:
            try:
                response = self.client_socket.recv(1024)
                if response:
                    response = json.loads(response.decode())
                    
                    self.monitor.handle_event(response)
                    self.trader_account.handle_event(response)
                    
                    if response["event"] == "UPDATE_BOOK":
                        self.book.update_book(response)

            except socket.error:
                # Handle socket error (e.g., server closed the connection)
                print("Server disconnected.")
                break