import threading
import json
import random

import tkinter as tk
from tkinter import messagebox

class HomeBroker:
    def __init__(self, client):
        self.client = client
        self.trader_account = client.trader_account
        self.book = client.book
        self.symbol = self.book.symbol

    def start_application_thread(self):
        application_thread = threading.Thread(target=self.application)
        application_thread.daemon = False 
        application_thread.start()
    
    # Function to send the order to the server
    def send_order(self):
        quantity = self.quantity_var.get()
        price = self.price_var.get()
        
        if self.order_type_var.get() == 0:
            order_type = "Compra"
            message = self.build_buy_offer(price, quantity)
        else:
            order_type = "Venda"
            message = self.build_sell_offer(price, quantity)

        self.client.send_data(message)

        messagebox.showinfo("Ordem enviada", "Símbolo: {}\nPreço: {}\nQuantidade: {}\nTipo: {}\n".format(self.symbol, price, quantity, order_type))

    def application(self):
        # Create the main application window
        root = tk.Tk()
        root.title("Simulador [B3]")

        # Set the window size (width x height)
        window_width = 500
        window_height = 300
        screen_width = root.winfo_screenwidth()
        screen_height = root.winfo_screenheight()

        # Calculate the position to center the window
        x_position = (screen_width - window_width) // 2
        y_position = (screen_height - window_height) // 2

        # Set the window size and position
        root.geometry(f"{window_width}x{window_height}+{x_position}+{y_position}")

        # Quantity input
        quantity_label = tk.Label(root, text="Quantidade:")
        quantity_label.pack(pady=10)
        self.quantity_var = tk.IntVar()
        quantity_entry = tk.Entry(root, textvariable=self.quantity_var)
        quantity_entry.pack(pady=5)

        # Target price input
        price_label = tk.Label(root, text="Preço(máx/min):")
        price_label.pack(pady=10)
        self.price_var = tk.DoubleVar()
        price_entry = tk.Entry(root, textvariable=self.price_var)
        price_entry.pack(pady=5)

        # Order type selection (buy/sell)
        order_type_label = tk.Label(root, text="Tipo de ordem:")
        order_type_label.pack(pady=10)
        self.order_type_var = tk.IntVar()
        buy_radio = tk.Radiobutton(root, text="Compra", variable=self.order_type_var, value=0)
        buy_radio.pack()
        sell_radio = tk.Radiobutton(root, text="Venda", variable=self.order_type_var, value=1)
        sell_radio.pack()

        # Send order button
        send_button = tk.Button(root, text="Enviar ordem", command=self.send_order)
        send_button.pack(pady=20)

        self.update_label = tk.Label(root, text="")
        self.update_label.pack()

        # Run the main event loop to display the GUI and handle events
        root.mainloop()

    def build_buy_offer(self, price, qty):
        id = ''.join([str(random.randint(0, 8)) for _ in range(10)])
        offer = "2019-12-20;{};1;{};0;002;10:27:01.798000;0;{};{};0;2019-12-20;2019-12-20 10:27:01;5;0;0;CLIENT".format(self.symbol, id, price, qty)
        
        json_data = {
            "offer": offer
        }

        return  json.dumps(json_data)

    def build_sell_offer(self, price, qty):
        id = ''.join([str(random.randint(0, 8)) for _ in range(10)])
        offer = "2019-12-20;{};2;{};0;002;10:27:01.798000;0;{};{};0;2019-12-20;2019-12-20 10:27:01;5;0;0;CLIENT".format(self.symbol, id, price, qty)

        json_data = {
            "offer": offer
        }

        return  json.dumps(json_data)
