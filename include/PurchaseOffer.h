#include <string>
#include <vector>

using namespace std;

#ifndef __PURCHASE_OFFER_H__
#define __PURCHASE_OFFER_H__

class PurchaseOffer
{
private:
    string sequentialOrderNumber;
    string secondaryOrderID;
    string priorityTime;
    double orderPrice;
    long long priorityTimeInteger;
    int totalQuantityOfOrder;
    int tradedQuantityOfOrder;

public:
    PurchaseOffer();
    PurchaseOffer(string sequentialOrderNumber, string secondaryOrderID, string priorityTime, string orderPrice, string totalQuantityOfOrder, string tradedQuantityOfOrder);
};

#endif