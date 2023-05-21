#include "Semaphore.h"
#include "TraderStockPosition.h"
#include "StockInfo.h"

#include <string>
#include <map>

using namespace std;

#ifndef __TRADER_H__
#define __TRADER_H__

class Trader
{
private:
    map<string, TraderStockPosition> position;
    double profitAndLoss;
    double cash;
public:
    Trader(double cash);
    void onTick(string symbol, double price, string type, map<string, StockInfo>* offersBook);
    void onBook();
    void onTrade();
    void onTimer();

    void setPosition(map<string, TraderStockPosition> position);
    map<string, TraderStockPosition> getPosition();
    
    void setProfitAndLoss(double profitAndLoss);
    double getProfitAndLoss();

    void setCash(double cash);
    double getCash();

    void buy(string symbol, int quantity, map<string, StockInfo>* offersBook);
    void sell(string symbol, int quantity, map<string, StockInfo>* offersBook);
    
};

#endif