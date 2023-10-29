#include <vector>
#include <deque>
#include "PurchaseOrder.h"
#include "SaleOrder.h"

#ifndef STOCK_INFO_H
#define STOCK_INFO_H

struct StockInfo
{
    double bid = 0;
    double ask = 0;
    int totalTradedQuantity = 0;
    deque<PurchaseOrder> purchaseOrders;
    deque<SaleOrder> saleOrders;
    double lastTradePrice = 0;
};

#endif