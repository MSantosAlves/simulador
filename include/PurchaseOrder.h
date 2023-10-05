#include <string>
#include <vector>

using namespace std;

#ifndef __PURCHASE_ORDER_H__
#define __PURCHASE_ORDER_H__

class PurchaseOrder
{
private:
    string sequentialOrderNumber;
    string secondaryOrderID;
    string priorityTime;
    double orderPrice = -1;
    long long priorityTimeInteger;
    int totalQuantityOfOrder = 0;
    int tradedQuantityOfOrder = 0;
    int orderSource = 0;

public:
    PurchaseOrder();
    PurchaseOrder(string sequentialOrderNumber, string secondaryOrderID, string priorityTime, string orderPrice, string totalQuantityOfOrder, string tradedQuantityOfOrder, int orderSource);

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

    int getOrderSource();
    void setOrderSource(int orderSource);
};

#endif