#include <string>
#include <vector>

using namespace std;

#ifndef __SALE_OFFER_H__
#define __SALE_OFFER_H__

class SaleOffer
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
    SaleOffer();
    SaleOffer(string sequentialOrderNumber, string secondaryOrderID, string priorityTime, string orderPrice, string totalQuantityOfOrder, string tradedQuantityOfOrder);
};

#endif