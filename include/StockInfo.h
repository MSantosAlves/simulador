#include <vector>
#include "PurchaseOrder.h"
#include "SaleOrder.h"

#ifndef STOCK_INFO_H
#define STOCK_INFO_H

struct StockInfo
{
    double bid;
    double ask;
    vector<PurchaseOrder> purchaseOrders;
    vector<SaleOrder> saleOrders;
};

#endif