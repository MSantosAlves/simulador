#include "ArrayUtils.h"

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool comparePurchaseOrders(const PurchaseOrder &a, const PurchaseOrder &b)
{
    if (a.getOrderPrice() != b.getOrderPrice())
    {
        return a.getOrderPrice() > b.getOrderPrice();
    }
    else
    {
        return a.getPriorityIndicator() < b.getPriorityIndicator();
    }
}

bool compareSaleOrders(const SaleOrder &a, const SaleOrder &b)
{
    if (a.getOrderPrice() != b.getOrderPrice())
    {
        return a.getOrderPrice() < b.getOrderPrice();
    }
    else
    {
        return a.getPriorityIndicator() < b.getPriorityIndicator();
    }
}

ArrayUtils::ArrayUtils() {}

void ArrayUtils::insertPurchaseOrder(vector<PurchaseOrder> &orders, const PurchaseOrder &newOrder)
{
    auto it = lower_bound(orders.begin(), orders.end(), newOrder, comparePurchaseOrders);
    orders.insert(it, newOrder);
}

void ArrayUtils::insertSaleOrder(vector<SaleOrder> &orders, const SaleOrder &newOrder)
{
    auto it = lower_bound(orders.begin(), orders.end(), newOrder, compareSaleOrders);
    bool newAskPrice = false;
    orders.insert(it, newOrder);
}

int ArrayUtils::searchPurchaseOrdersPriceLastIndex(vector<PurchaseOrder> &offers, double price)
{
    int left = 0;
    int right = offers.size() - 1;
    int lastPriceIndex = -1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (offers[mid].getOrderPrice() == price)
        {
            lastPriceIndex = mid;
            left = mid + 1;
        }
        else if (offers[mid].getOrderPrice() < price)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    return lastPriceIndex;
}

int ArrayUtils::searchSaleOrdersPriceLastIndex(vector<SaleOrder> &offers, double price)
{
    int left = 0;
    int right = offers.size() - 1;
    int lastPriceIndex = -1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (offers[mid].getOrderPrice() == price)
        {
            lastPriceIndex = mid;
            left = mid + 1;
        }
        else if (offers[mid].getOrderPrice() < price)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    return lastPriceIndex;
}