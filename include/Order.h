#include <string>
#include <vector>

using namespace std;

#ifndef __Order_H__
#define __Order_H__

class Order
{
private:
    // Order fields
    string type;
    string sessionDate;
    string instrumentSymbol;
    string orderSide;
    string sequentialOrderNumber;
    string secondaryOrderID;
    string executionType;
    string priorityTime;
    string priorityIndicator;
    string orderPrice;
    string totalQuantityOfOrder;
    string tradedQuantityOfOrder;
    string orderDate;
    string orderDatetimeEntry;
    string orderStatus;
    string aggressorIndicator;
    string member;

    // OrderNeg fields
    string iradeNumber;
    string iradePrice;
    string iradedQuantity;
    string iradeTime;
    string iradeIndicator;
    string buyOrderDate;
    string sequentialBuyOrderNumber;
    string secondaryOrderIDBuyOrder;
    string aggressorBuyOrderIndicator;
    string sellOrderDate;
    string sequentialSellOrderNumber;
    string secondaryOrderIDSellOrder;
    string aggressorSellOrderIndicator;
    string crossTradeIndicator;
    string buyMember;
    string sellMember;

    vector<string> arrayOfCalculatedFields;
    int source; // 0: Data / 1: Client

public:
    Order();

    string getType();
    void setType(string type);

    string getSessionDate();
    void setSessionDate(string sessionDate);

    string getInstrumentSymbol();
    void setInstrumentSymbol(string instrumentSymbol);

    string getOrderSide();
    void setOrderSide(string orderSide);

    string getSequentialOrderNumber();
    void setSequentialOrderNumber(string sequentialOrderNumber);

    string getSecondaryOrderID();
    void setSecondaryOrderID(string secondaryOrderID);

    string getExecutionType();
    void setExecutionType(string executionType);

    string getPriorityTime();
    void setPriorityTime(string priorityTime);

    string getPriorityIndicator();
    void setPriorityIndicator(string priorityIndicator);

    string getOrderPrice();
    void setOrderPrice(string orderPrice);

    string getTotalQuantityOfOrder();
    void setTotalQuantityOfOrder(string totalQuantityOfOrder);

    string getTradedQuantityOfOrder();
    void setTradedQuantityOfOrder(string tradedQuantityOfOrder);

    string getOrderDate();
    void setOrderDate(string orderDate);

    string getOrderDatetimeEntry();
    void setOrderDatetimeEntry(string orderDatetimeEntry);

    string getOrderStatus();
    void setOrderStatus(string orderStatus);

    string getAggressorIndicator();
    void setAggressorIndicator(string aggressorIndicator);

    string getMember();
    void setMember(string member);

    string getIradeNumber();
    void setIradeNumber(string iradeNumber);

    string getIradePrice();
    void setIradePrice(string iradePrice);

    string getIradedQuantity();
    void setIradedQuantity(string iradedQuantity);

    string getIradeTime();
    void setIradeTime(string iradeTime);

    string getIradeIndicator();
    void setIradeIndicator(string iradeIndicator);

    string getBuyOrderDate();
    void setBuyOrderDate(string buyOrderDate);

    string getSequentialBuyOrderNumber();
    void setSequentialBuyOrderNumber(string sequentialBuyOrderNumber);

    string getSecondaryOrderIDBuyOrder();
    void setSecondaryOrderIDBuyOrder(string secondaryOrderIDBuyOrder);

    string getAggressorBuyOrderIndicator();
    void setAggressorBuyOrderIndicator(string aggressorBuyOrderIndicator);

    string getSellOrderDate();
    void setSellOrderDate(string sellOrderDate);

    string getSequentialSellOrderNumber();
    void setSequentialSellOrderNumber(string sequentialSellOrderNumber);

    string getSecondaryOrderIDSellOrder();
    void setSecondaryOrderIDSellOrder(string secondaryOrderIDSellOrder);

    string getAggressorSellOrderIndicator();
    void setAggressorSellOrderIndicator(string aggressorSellOrderIndicator);

    string getCrossTradeIndicator();
    void setCrossTradeIndicator(string crossTradeIndicator);

    string getBuyMember();
    void setBuyMember(string buyMember);

    string getSellMember();
    void setSellMember(string sellMember);

    vector<string> getArrayOfCalculatedFields(bool allFields, vector<int> fieldsIndexes);
    void setArrayOfCalculatedFields(vector<string> arrayOfCalculatedFields);

    int getOrderSource();
    void setOrderSource(int source);
};

#endif