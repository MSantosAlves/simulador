from client import Client

import time

server_address = '127.0.0.1'  # Replace with the actual server address
server_port = 8080  # Replace with the actual server port
client = Client(server_address, server_port)
client.connect()

client.start_receive_thread() 

cpa_offer = "2019-12-20;AFSF20                                            ;1;000743936817797;000000179361519;002;10:27:01.798000;1491778864; 000000014254.000000;000000000000000100;000000000000000000;2019-12-20;2019-12-20 10:27:01;5;0;00000088;CLIENT"
vda_offer = "2019-12-20;AFSF20                                            ;2;000743936817797;000000179361519;002;10:27:01.798000;1491778864; 000000005000.000000;000000000000000100;000000000000000000;2019-12-20;2019-12-20 10:27:01;5;0;00000088;CLIENT"

while True:
    user_input = input()

    if user_input == '1':
        message = cpa_offer
        client.send_data(message)
    elif user_input == '2':
        message = vda_offer
        client.send_data(message)
    elif user_input == '3':
        print("Closing client.")
        client.close();
    else:
        print("Invalid input.")

    # Add a delay to avoid constantly looping and overwhelming the server
    time.sleep(1)