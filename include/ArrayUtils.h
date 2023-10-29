#include <vector>
#include <deque>
#include "PurchaseOrder.h"
#include "SaleOrder.h"

using namespace std;

#ifndef __ARRAY_UTILS_H__
#define __ARRAY_UTILS_H__

class ArrayUtils
{
public:
    ArrayUtils();
    void insertPurchaseOrder(vector<PurchaseOrder> &offers, const PurchaseOrder &newOffer);
    void insertSaleOrder(vector<SaleOrder> &offers, const SaleOrder &newOffer);
    int searchPurchaseOrdersPriceLastIndex(deque<PurchaseOrder> &offers, double price);
    int searchSaleOrdersPriceLastIndex(deque<SaleOrder> &offers, double price);
};

#endif