#include "Event.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

Event::Event() {}

string Event::getType()
{
    return type;
}

void Event::setType(string _type)
{
    type = _type;
}

string Event::getSessionDate()
{
    return sessionDate;
}

void Event::setSessionDate(string _sessionDate)
{
    sessionDate = _sessionDate;
}

string Event::getInstrumentSymbol()
{
    return instrumentSymbol;
}

void Event::setInstrumentSymbol(string _instrumentSymbol)
{
    instrumentSymbol = _instrumentSymbol;
}

string Event::getOrderSide()
{
    return orderSide;
}

void Event::setOrderSide(string _orderSide)
{
    orderSide = _orderSide;
}

string Event::getSequentialOrderNumber()
{
    return sequentialOrderNumber;
}

void Event::setSequentialOrderNumber(string _sequentialOrderNumber)
{
    sequentialOrderNumber = _sequentialOrderNumber;
}

string Event::getSecondaryOrderID()
{
    return secondaryOrderID;
}

void Event::setSecondaryOrderID(string _secondaryOrderID)
{
    secondaryOrderID = _secondaryOrderID;
}

string Event::getExecutionType()
{
    return executionType;
}

void Event::setExecutionType(string _executionType)
{
    executionType = _executionType;
}

string Event::getPriorityTime()
{
    return priorityTime;
}

void Event::setPriorityTime(string _priorityTime)
{
    priorityTime = _priorityTime;
}

string Event::getPriorityIndicator()
{
    return priorityIndicator;
}

void Event::setPriorityIndicator(string _priorityIndicator)
{
    priorityIndicator = _priorityIndicator;
}

string Event::getOrderPrice()
{
    return orderPrice;
}

void Event::setOrderPrice(string _orderPrice)
{
    orderPrice = _orderPrice;
}

string Event::getTotalQuantityOfOrder()
{
    return totalQuantityOfOrder;
}

void Event::setTotalQuantityOfOrder(string _totalQuantityOfOrder)
{
    totalQuantityOfOrder = _totalQuantityOfOrder;
}

string Event::getTradedQuantityOfOrder()
{
    return tradedQuantityOfOrder;
}

void Event::setTradedQuantityOfOrder(string _tradedQuantityOfOrder)
{
    tradedQuantityOfOrder = _tradedQuantityOfOrder;
}

string Event::getOrderDate()
{
    return orderDate;
}

void Event::setOrderDate(string _orderDate)
{
    orderDate = _orderDate;
}

string Event::getOrderDatetimeEntry()
{
    return orderDatetimeEntry;
}

void Event::setOrderDatetimeEntry(string _orderDatetimeEntry)
{
    orderDatetimeEntry = _orderDatetimeEntry;
}

string Event::getOrderStatus()
{
    return orderStatus;
}

void Event::setOrderStatus(string _orderStatus)
{
    orderStatus = _orderStatus;
}

string Event::getAggressorIndicator()
{
    return aggressorIndicator;
}

void Event::setAggressorIndicator(string _aggressorIndicator)
{
    aggressorIndicator = _aggressorIndicator;
}

string Event::getMember()
{
    return member;
}

void Event::setMember(string _member)
{
    member = _member;
}

string Event::getIradeNumber()
{
    return iradeNumber;
}

void Event::setIradeNumber(string _iradeNumber)
{
    iradeNumber = _iradeNumber;
}

string Event::getIradePrice()
{
    return iradePrice;
}

void Event::setIradePrice(string _iradePrice)
{
    iradePrice = _iradePrice;
}

string Event::getIradedQuantity()
{
    return iradedQuantity;
}

void Event::setIradedQuantity(string _iradedQuantity)
{
    iradedQuantity = _iradedQuantity;
}

string Event::getIradeTime()
{
    return iradeTime;
}

void Event::setIradeTime(string _iradeTime)
{
    iradeTime = _iradeTime;
}

string Event::getIradeIndicator()
{
    return iradeIndicator;
}

void Event::setIradeIndicator(string _iradeIndicator)
{
    iradeIndicator = _iradeIndicator;
}

string Event::getBuyOrderDate()
{
    return buyOrderDate;
}

void Event::setBuyOrderDate(string _buyOrderDate)
{
    buyOrderDate = _buyOrderDate;
}

string Event::getSequentialBuyOrderNumber()
{
    return sequentialBuyOrderNumber;
}

void Event::setSequentialBuyOrderNumber(string _sequentialBuyOrderNumber)
{
    sequentialBuyOrderNumber = _sequentialBuyOrderNumber;
}

string Event::getSecondaryOrderIDBuyOrder()
{
    return secondaryOrderIDBuyOrder;
}

void Event::setSecondaryOrderIDBuyOrder(string _secondaryOrderIDBuyOrder)
{
    secondaryOrderIDBuyOrder = _secondaryOrderIDBuyOrder;
}

string Event::getAggressorBuyOrderIndicator()
{
    return aggressorBuyOrderIndicator;
}

void Event::setAggressorBuyOrderIndicator(string _aggressorBuyOrderIndicator)
{
    aggressorBuyOrderIndicator = _aggressorBuyOrderIndicator;
}

string Event::getSellOrderDate()
{
    return sellOrderDate;
}

void Event::setSellOrderDate(string _sellOrderDate)
{
    sellOrderDate = _sellOrderDate;
}

string Event::getSequentialSellOrderNumber()
{
    return sequentialSellOrderNumber;
}

void Event::setSequentialSellOrderNumber(string _sequentialSellOrderNumber)
{
    sequentialSellOrderNumber = _sequentialSellOrderNumber;
}

string Event::getSecondaryOrderIDSellOrder()
{
    return secondaryOrderIDSellOrder;
}

void Event::setSecondaryOrderIDSellOrder(string _secondaryOrderIDSellOrder)
{
    secondaryOrderIDSellOrder = _secondaryOrderIDSellOrder;
}

string Event::getAggressorSellOrderIndicator()
{
    return aggressorSellOrderIndicator;
}

void Event::setAggressorSellOrderIndicator(string _aggressorSellOrderIndicator)
{
    aggressorSellOrderIndicator = _aggressorSellOrderIndicator;
}

string Event::getCrossTradeIndicator()
{
    return crossTradeIndicator;
}

void Event::setCrossTradeIndicator(string _crossTradeIndicator)
{
    crossTradeIndicator = _crossTradeIndicator;
}

string Event::getBuyMember()
{
    return buyMember;
}

void Event::setBuyMember(string _buyMember)
{
    buyMember = _buyMember;
}

string Event::getSellMember()
{
    return sellMember;
}

void Event::setSellMember(string _sellMember)
{
    sellMember = _sellMember;
}

vector<string> Event::getArrayOfCalculatedFields(bool allFields, vector<int> fieldsIndexes)
{
    if (allFields) return arrayOfCalculatedFields;
    vector<string> aux;
    for (int i = 0; i < fieldsIndexes.size(); i++)
    {
        aux.push_back(arrayOfCalculatedFields[fieldsIndexes[i]]);
    }
    return aux;
}

void Event::setArrayOfCalculatedFields(vector<string> _arrayOfCalculatedFields)
{
    arrayOfCalculatedFields = _arrayOfCalculatedFields;
}