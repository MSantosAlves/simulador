import time
import threading
import json
import random

class HomeBroker:
    def __init__(self, client):
        self.client = client
        self.trader_account = client.trader_account
        self.book = client.book

    def start_application_thread(self):
        application_thread = threading.Thread(target=self.application)
        application_thread.daemon = False 
        application_thread.start()

    def application(self):
        print("1 - Comprar")
        print("2 - Vender")
        print("3 - Encerrar")
        while True:
            user_input = input()

            if user_input == '1':
                print("Informe o símbolo desejado: ")
                symbol = input()
                print("Informe o preço desejado: ")
                price = input()
                print("Informe a quantidade desejada: ")
                qty = input()
                message = self.build_buy_offer(symbol, price, qty)
                self.client.send_data(message)
            elif user_input == '2':
                print("Informe o símbolo desejado: ")
                symbol = input()
                print("Informe o preço desejado: ")
                price = input()
                print("Informe a quantidade desejada: ")
                qty = input()
                message = self.build_sell_offer(symbol, price, qty)
                self.client.send_data(message)
            elif user_input == '3':
                print("Closing client.")
                self.client.close()
            else:
                print("Invalid input.")

            # Add a delay to avoid constantly looping and overwhelming the server
            time.sleep(1)

    def build_buy_offer(self, symbol, price, qty):
        id = ''.join([str(random.randint(0, 8)) for _ in range(10)])
        offer = "2019-12-20;{};1;{};0;002;10:27:01.798000;0;{};{};0;2019-12-20;2019-12-20 10:27:01;5;0;0;CLIENT".format(symbol, id, price, qty)
        
        json_data = {
            "offer": offer
        }

        return  json.dumps(json_data)

    def build_sell_offer(self, symbol, price, qty):
        id = ''.join([str(random.randint(0, 8)) for _ in range(10)])
        offer = "2019-12-20;{};2;{};0;002;10:27:01.798000;0;{};{};0;2019-12-20;2019-12-20 10:27:01;5;0;0;CLIENT".format(symbol, id, price, qty)

        json_data = {
            "offer": offer
        }

        return  json.dumps(json_data)
