#include "ArrayUtils.h"

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool comparePurchaseOrders(const PurchaseOrder& a, const PurchaseOrder& b) {
    if (a.getOrderPrice() != b.getOrderPrice()) {
        return a.getOrderPrice() > b.getOrderPrice();
    }
    else {
        return a.getPriorityTimeInteger() < b.getPriorityTimeInteger();
    }
}

bool compareSaleOrders(const SaleOrder& a, const SaleOrder& b) {
    if (a.getOrderPrice() != b.getOrderPrice()) {
        return a.getOrderPrice() < b.getOrderPrice();
    }
    else {
        return a.getPriorityTimeInteger() < b.getPriorityTimeInteger();
    }
}

ArrayUtils::ArrayUtils() {}

void ArrayUtils::insertPurchaseOrder(vector<PurchaseOrder>& orders, const PurchaseOrder& newOrder) {
    auto it = lower_bound(orders.begin(), orders.end(), newOrder, comparePurchaseOrders);
    orders.insert(it, newOrder);
}

void ArrayUtils::insertSaleOrder(vector<SaleOrder>& orders, const SaleOrder& newOrder) {
    auto it = lower_bound(orders.begin(), orders.end(), newOrder, compareSaleOrders);
    bool newAskPrice = false;
    orders.insert(it, newOrder);
}