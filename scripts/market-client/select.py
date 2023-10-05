import socket
import select
import sys
import time

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect(('localhost', 8080))
#client_socket.setblocking(0)
#client_socket.settimeout(0.001)

inputs = [sys.stdin, client_socket]

while True:
    readable, _, _ = select.select(inputs, [], [])

    for sock in readable:
        if sock == client_socket:
            # Data received from the server
            data = sock.recv(1024)
            if data:
                print('Received from server:', data.decode('utf-8'))
                sock.close()
            else:
                print('Server closed the connection.')
                client_socket.close()
                sys.exit()
        elif sock == sys.stdin:
            # User input to send to the server
            message = sys.stdin.readline().strip()
            client_socket.sendall(message.encode('utf-8'))

            if message.lower() == 'exit':
                client_socket.close()
                sys.exit()