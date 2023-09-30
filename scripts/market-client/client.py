import socket

class Client:
    def __init__(self, server_address, server_port):
        self.server_address = server_address
        self.server_port = server_port
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def connect(self):
        self.client_socket.connect((self.server_address, self.server_port))

    def send_data(self, data):
        self.client_socket.sendall(data.encode())

    def receive_response(self):
        response = self.client_socket.recv(1024)
        return response.decode()

    def close(self):
        self.client_socket.close()

