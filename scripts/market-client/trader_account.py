import random
import json

TRADE_EVENTS = ["SALE_OFFER_ENTIRELY_FILLED", "SALE_OFFER_PARTIALLY_FILLED", "PURCHASE_OFFER_ENTIRELY_FILLED", "PURCHASE_OFFER_PARTIALLY_FILLED"]

class TraderAccount:
    def __init__(self, book, initial_cash = 100000):
        self.INITIAL_CASH = initial_cash
        self.position = {}
        self.cash = self.INITIAL_CASH
        self.profitAndLoss = 0
        self.id = ''.join([str(random.randint(0, 4)) for _ in range(10)])
        self.book = book

    def handle_event(self, data):
        event = data["event"]
        if event in TRADE_EVENTS:
            self.onTrade(data)
        elif "UPDATE_BOOK":
            self.onBook(data)
        elif "ON_TICK":
            self.ontTick(data)

    def ontTick(self, data):
        return

    def onBook(self, data):
        return

    def onTrade(self, data):
        event = data["event"]
        symbol = data["symbol"]
        price = data["price"]
        qty = data["qty"]
        total = int(qty) * float(price)

        if(self.is_buy(event)):
            if symbol in self.position:
                self.position[symbol] += qty
            else:
                self.position[symbol] = qty
            self.cash -= total
        else:
            self.position[symbol] -= qty
            self.cash += total

        if self.trade_entirely_filled(event):
            position_value = self.position[symbol] * self.book.stocks[symbol]["last_trade_price"]
            self.profitAndLoss = (self.cash + position_value) / self.INITIAL_CASH

    def buy(self, data):
        print(data)

    def sell(self, data):
        print(data)

    def is_buy(self, event):
        if event in ["PURCHASE_OFFER_ENTIRELY_FILLED", "PURCHASE_OFFER_PARTIALLY_FILLED"]:
            return True
        return False
    
    def trade_entirely_filled(self, event):
        return event.split("_")[-2] == "ENTIRELY"
    
    def to_json(self):

        dict = {
            "id": self.id,
            "cash": self.cash,
            "profitAndLoss": self.profitAndLoss,
            "position": self.position
        }

        return json.dumps(dict)