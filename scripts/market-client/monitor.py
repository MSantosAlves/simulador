class Monitor:

    def __init__(self, book):
        self.stocks_last_prices = {}
        self.book = book

    def handle_event(self, data):
        #print(data)
        return