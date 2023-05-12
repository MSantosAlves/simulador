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

    string getSequentialOrderNumber();
    void setSequentialOrderNumber(string sequentialOrderNumber);

    string getSecondaryOrderID();
    void setSecondaryOrderID(string secondaryOrderID);
    
    string getPriorityTime();
    void setPriorityTime(string priorityTime);
    
    double getOrderPrice() const;
    void setOrderPrice(string orderPrice);
    
    long long getPriorityTimeInteger() const;
    void setPriorityTimeInteger(long long priorityTimeInteger);
    
    int getTotalQuantityOfOrder();
    void setTotalQuantityOfOrder(int totalQuantityOfOrder);
    
    int getTradedQuantityOfOrder();
    void setTradedQuantityOfOrder(int tradedQuantityOfOrder);
    
};

#endif