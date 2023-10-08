#ifndef STOCK_MARKET_VOLUME_H
#define STOCK_MARKET_VOLUME_H

struct StockMarketVolume
{
    double price = 0;
    int quantity = 0;
    string direction;
};

#endif