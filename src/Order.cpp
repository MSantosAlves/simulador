#include "Order.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

Order::Order() {}

string Order::getType()
{
    return type;
}

void Order::setType(string _type)
{
    type = _type;
}

string Order::getSessionDate()
{
    return sessionDate;
}

void Order::setSessionDate(string _sessionDate)
{
    sessionDate = _sessionDate;
}

string Order::getInstrumentSymbol()
{
    return instrumentSymbol;
}

void Order::setInstrumentSymbol(string _instrumentSymbol)
{
    instrumentSymbol = _instrumentSymbol;
}

string Order::getOrderSide()
{
    return orderSide;
}

void Order::setOrderSide(string _orderSide)
{
    orderSide = _orderSide;
}

string Order::getSequentialOrderNumber()
{
    return sequentialOrderNumber;
}

void Order::setSequentialOrderNumber(string _sequentialOrderNumber)
{
    sequentialOrderNumber = _sequentialOrderNumber;
}

string Order::getSecondaryOrderID()
{
    return secondaryOrderID;
}

void Order::setSecondaryOrderID(string _secondaryOrderID)
{
    secondaryOrderID = _secondaryOrderID;
}

string Order::getExecutionType()
{
    return executionType;
}

void Order::setExecutionType(string _executionType)
{
    executionType = _executionType;
}

string Order::getPriorityTime()
{
    return priorityTime;
}

void Order::setPriorityTime(string _priorityTime)
{
    priorityTime = _priorityTime;
}

string Order::getPriorityIndicator()
{
    return priorityIndicator;
}

void Order::setPriorityIndicator(string _priorityIndicator)
{
    priorityIndicator = _priorityIndicator;
}

string Order::getOrderPrice()
{
    return orderPrice;
}

void Order::setOrderPrice(string _orderPrice)
{
    orderPrice = _orderPrice;
}

string Order::getTotalQuantityOfOrder()
{
    return totalQuantityOfOrder;
}

void Order::setTotalQuantityOfOrder(string _totalQuantityOfOrder)
{
    totalQuantityOfOrder = _totalQuantityOfOrder;
}

string Order::getTradedQuantityOfOrder()
{
    return tradedQuantityOfOrder;
}

void Order::setTradedQuantityOfOrder(string _tradedQuantityOfOrder)
{
    tradedQuantityOfOrder = _tradedQuantityOfOrder;
}

string Order::getOrderDate()
{
    return orderDate;
}

void Order::setOrderDate(string _orderDate)
{
    orderDate = _orderDate;
}

string Order::getOrderDatetimeEntry()
{
    return orderDatetimeEntry;
}

void Order::setOrderDatetimeEntry(string _orderDatetimeEntry)
{
    orderDatetimeEntry = _orderDatetimeEntry;
}

string Order::getOrderStatus()
{
    return orderStatus;
}

void Order::setOrderStatus(string _orderStatus)
{
    orderStatus = _orderStatus;
}

string Order::getAggressorIndicator()
{
    return aggressorIndicator;
}

void Order::setAggressorIndicator(string _aggressorIndicator)
{
    aggressorIndicator = _aggressorIndicator;
}

string Order::getMember()
{
    return member;
}

void Order::setMember(string _member)
{
    member = _member;
}

string Order::getIradeNumber()
{
    return iradeNumber;
}

void Order::setIradeNumber(string _iradeNumber)
{
    iradeNumber = _iradeNumber;
}

string Order::getIradePrice()
{
    return iradePrice;
}

void Order::setIradePrice(string _iradePrice)
{
    iradePrice = _iradePrice;
}

string Order::getIradedQuantity()
{
    return iradedQuantity;
}

void Order::setIradedQuantity(string _iradedQuantity)
{
    iradedQuantity = _iradedQuantity;
}

string Order::getIradeTime()
{
    return iradeTime;
}

void Order::setIradeTime(string _iradeTime)
{
    iradeTime = _iradeTime;
}

string Order::getIradeIndicator()
{
    return iradeIndicator;
}

void Order::setIradeIndicator(string _iradeIndicator)
{
    iradeIndicator = _iradeIndicator;
}

string Order::getBuyOrderDate()
{
    return buyOrderDate;
}

void Order::setBuyOrderDate(string _buyOrderDate)
{
    buyOrderDate = _buyOrderDate;
}

string Order::getSequentialBuyOrderNumber()
{
    return sequentialBuyOrderNumber;
}

void Order::setSequentialBuyOrderNumber(string _sequentialBuyOrderNumber)
{
    sequentialBuyOrderNumber = _sequentialBuyOrderNumber;
}

string Order::getSecondaryOrderIDBuyOrder()
{
    return secondaryOrderIDBuyOrder;
}

void Order::setSecondaryOrderIDBuyOrder(string _secondaryOrderIDBuyOrder)
{
    secondaryOrderIDBuyOrder = _secondaryOrderIDBuyOrder;
}

string Order::getAggressorBuyOrderIndicator()
{
    return aggressorBuyOrderIndicator;
}

void Order::setAggressorBuyOrderIndicator(string _aggressorBuyOrderIndicator)
{
    aggressorBuyOrderIndicator = _aggressorBuyOrderIndicator;
}

string Order::getSellOrderDate()
{
    return sellOrderDate;
}

void Order::setSellOrderDate(string _sellOrderDate)
{
    sellOrderDate = _sellOrderDate;
}

string Order::getSequentialSellOrderNumber()
{
    return sequentialSellOrderNumber;
}

void Order::setSequentialSellOrderNumber(string _sequentialSellOrderNumber)
{
    sequentialSellOrderNumber = _sequentialSellOrderNumber;
}

string Order::getSecondaryOrderIDSellOrder()
{
    return secondaryOrderIDSellOrder;
}

void Order::setSecondaryOrderIDSellOrder(string _secondaryOrderIDSellOrder)
{
    secondaryOrderIDSellOrder = _secondaryOrderIDSellOrder;
}

string Order::getAggressorSellOrderIndicator()
{
    return aggressorSellOrderIndicator;
}

void Order::setAggressorSellOrderIndicator(string _aggressorSellOrderIndicator)
{
    aggressorSellOrderIndicator = _aggressorSellOrderIndicator;
}

string Order::getCrossTradeIndicator()
{
    return crossTradeIndicator;
}

void Order::setCrossTradeIndicator(string _crossTradeIndicator)
{
    crossTradeIndicator = _crossTradeIndicator;
}

string Order::getBuyMember()
{
    return buyMember;
}

void Order::setBuyMember(string _buyMember)
{
    buyMember = _buyMember;
}

string Order::getSellMember()
{
    return sellMember;
}

void Order::setSellMember(string _sellMember)
{
    sellMember = _sellMember;
}

vector<string> Order::getArrayOfCalculatedFields(bool allFields, vector<int> fieldsIndexes)
{
    if (allFields) return arrayOfCalculatedFields;
    vector<string> aux;
    for (int i = 0; i < fieldsIndexes.size(); i++)
    {
        aux.push_back(arrayOfCalculatedFields[fieldsIndexes[i]]);
    }
    return aux;
}

void Order::setArrayOfCalculatedFields(vector<string> _arrayOfCalculatedFields)
{
    arrayOfCalculatedFields = _arrayOfCalculatedFields;
}