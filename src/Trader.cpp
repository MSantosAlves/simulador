#include "Trader.h"
#include "TraderStockPosition.h"
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

Trader::Trader(double _cash) {
    position = {};
    profitAndLoss = 0;
    cash = _cash;
}

void Trader::onTick(string symbol, double price, string type, map<string, StockInfo>* offersBook) {
    int pricesLength = offersBook->at(symbol).historicalPrices.size();

    if (pricesLength > 2) {
        double lastLastPrice = offersBook->at(symbol).historicalPrices[pricesLength - 3];
        double lastPrice = offersBook->at(symbol).historicalPrices[pricesLength - 2];

        if (lastLastPrice > lastPrice && lastPrice > price) {
            buy(symbol, 1, offersBook);
        }else if (lastLastPrice < lastPrice && lastPrice < price) {
        	sell(symbol, 1, offersBook);
        }
    }
}

void Trader::onBook() {
    cout << "Strategy OnBook" << endl;
}

void Trader::onTrade() {
    cout << "Strategy OnTrade" << endl;
}

void Trader::onTimer() {
    cout << "Strategy OnTimer" << endl;
}

void Trader::buy(string symbol, int qty, map<string, StockInfo>* offersBook) {
	cout << "Strategy Buy" << endl;
}

void Trader::sell(string symbol, int qty, map<string, StockInfo>* offersBook) {
    cout << "Strategy Sell" << endl;
}

void Trader::setPosition(map<string, TraderStockPosition> _position) {
    position = _position;
}

map<string, TraderStockPosition> Trader::getPosition() {
    return position;
}

void Trader::setProfitAndLoss(double _profitAndLoss) {
    profitAndLoss = _profitAndLoss;
}

double Trader::getProfitAndLoss() {
    return profitAndLoss;
}

void Trader::setCash(double _cash) {
    cash = _cash;
}

double Trader::getCash() {
    return cash;
}