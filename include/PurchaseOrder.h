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
    long long priorityIndicator;
    int totalQuantityOfOrder = 0;
    int tradedQuantityOfOrder = 0;
    int orderSource = 0;
    int agressorIndicator = 0;

public:
    PurchaseOrder();
    PurchaseOrder(string sequentialOrderNumber, string secondaryOrderID, string priorityTime, string priorityIndicator, string orderPrice, string totalQuantityOfOrder, string tradedQuantityOfOrder, int orderSource, string agressorIndicator);

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

    long long getPriorityIndicator() const;
    void setPriorityIndicator(long long priorityIndicator);

    int getTotalQuantityOfOrder();
    void setTotalQuantityOfOrder(int totalQuantityOfOrder);

    int getTradedQuantityOfOrder();
    void setTradedQuantityOfOrder(int tradedQuantityOfOrder);

    int getOrderSource();
    void setOrderSource(int orderSource);

    int getAgressorIndicator();
    void setAgressorIndicator(string agressorIndicator);

    bool operator<(const PurchaseOrder& other) const;
};

#endif