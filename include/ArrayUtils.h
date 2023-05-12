#include <vector>
#include <vector>
#include "PurchaseOffer.h"
#include "SaleOffer.h"

using namespace std;

#ifndef __ARRAY_UTILS_H__
#define __ARRAY_UTILS_H__

class ArrayUtils
{
public:
    ArrayUtils();
    void insertPurchaseOffer(vector<PurchaseOffer>& offers, const PurchaseOffer& newOffer);
    void insertSaleOffer(vector<SaleOffer>& offers, const SaleOffer& newOffer);
};

#endif