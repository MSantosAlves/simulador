#include "ArrayUtils.h"

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool comparePurchaseOffers(const PurchaseOffer& a, const PurchaseOffer& b) {
    if (a.getOrderPrice() != b.getOrderPrice()) {
        return a.getOrderPrice() > b.getOrderPrice();
    }
    else {
        return a.getPriorityTimeInteger() < b.getPriorityTimeInteger();
    }
}

bool compareSaleOffers(const SaleOffer& a, const SaleOffer& b) {
    if (a.getOrderPrice() != b.getOrderPrice()) {
        return a.getOrderPrice() < b.getOrderPrice();
    }
    else {
        return a.getPriorityTimeInteger() < b.getPriorityTimeInteger();
    }
}

ArrayUtils::ArrayUtils() {}

void ArrayUtils::insertPurchaseOffer(vector<PurchaseOffer>& offers, const PurchaseOffer& newOffer) {
    auto it = lower_bound(offers.begin(), offers.end(), newOffer, comparePurchaseOffers);
    offers.insert(it, newOffer);
}

void ArrayUtils::insertSaleOffer(vector<SaleOffer>& offers, const SaleOffer& newOffer) {
    auto it = lower_bound(offers.begin(), offers.end(), newOffer, compareSaleOffers);
    bool newAskPrice = false;
    offers.insert(it, newOffer);
}