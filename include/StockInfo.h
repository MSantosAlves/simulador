#include <vector>
#include "PurchaseOrder.h"
#include "SaleOrder.h"

#ifndef STOCK_INFO_H
#define STOCK_INFO_H

struct StockInfo
{
    double bid = 0;
    double ask = 0;
    int totalTradedQuantity = 0;
    vector<double> historicalPrices;
    vector<PurchaseOrder> purchaseOrders;
    vector<SaleOrder> saleOrders;
};

#endif