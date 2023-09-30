from client import Client

server_address = '127.0.0.1'  # Replace with the actual server address
server_port = 8080  # Replace with the actual server port
client = Client(server_address, server_port)
client.connect()
data_to_send = "Hello, server! This is the client sending a message."
print("Sending data:", data_to_send)
client.send_data(data_to_send)

response = client.receive_response()
print("Received response:", response)

client.close()