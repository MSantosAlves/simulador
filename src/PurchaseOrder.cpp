#include "PurchaseOrder.h"
#include "StringUtils.h"
#include <string>

using namespace std;

PurchaseOrder::PurchaseOrder() {}

PurchaseOrder::PurchaseOrder(string _sequentialOrderNumber, string _secondaryOrderID,  string _priorityTime, string _priorityIndicator, string _orderPrice, string _totalQuantityOfOrder, string _tradedQuantityOfOrder, int _source, string _agressorIndicator)
{
    StringUtils stringUtils;
    sequentialOrderNumber = _sequentialOrderNumber;
    secondaryOrderID = _secondaryOrderID;
    priorityTime = _priorityTime;
    priorityTimeInteger = stringUtils.dateToInt(_priorityTime);
    priorityIndicator = stoll(_priorityIndicator);
    orderPrice = stoi(_orderPrice);
    totalQuantityOfOrder = stoi(_totalQuantityOfOrder);
    tradedQuantityOfOrder = stoi(_tradedQuantityOfOrder);
    orderSource = _source;
    agressorIndicator = stoi(_agressorIndicator);
};

string PurchaseOrder::getSequentialOrderNumber()
{
    return sequentialOrderNumber;
}

void PurchaseOrder::setSequentialOrderNumber(string _sequentialOrderNumber)
{
    sequentialOrderNumber = _sequentialOrderNumber;
}

string PurchaseOrder::getSecondaryOrderID()
{
    return secondaryOrderID;
}

void PurchaseOrder::setSecondaryOrderID(string _secondaryOrderID)
{
    secondaryOrderID = _secondaryOrderID;
}

string PurchaseOrder::getPriorityTime()
{
    return priorityTime;
}

void PurchaseOrder::setPriorityTime(string _priorityTime)
{
    priorityTime = _priorityTime;
}

double PurchaseOrder::getOrderPrice() const
{
    return orderPrice;
}

void PurchaseOrder::setOrderPrice(string _orderPrice)
{
    orderPrice = stod(_orderPrice);
}

long long PurchaseOrder::getPriorityTimeInteger() const
{
    return priorityTimeInteger;
}

void PurchaseOrder::setPriorityTimeInteger(long long _priorityTimeInteger)
{
    priorityTimeInteger = _priorityTimeInteger;
}

long long PurchaseOrder::getPriorityIndicator() const
{
    return priorityIndicator;
}

void PurchaseOrder::setPriorityIndicator(long long _priorityIndicator)
{
    priorityIndicator = _priorityIndicator;
}

int PurchaseOrder::getTotalQuantityOfOrder()
{
    return totalQuantityOfOrder;
}

void PurchaseOrder::setTotalQuantityOfOrder(int _totalQuantityOfOrder)
{
    totalQuantityOfOrder = _totalQuantityOfOrder;
}

int PurchaseOrder::getTradedQuantityOfOrder()
{
    return tradedQuantityOfOrder;
}

void PurchaseOrder::setTradedQuantityOfOrder(int _tradedQuantityOfOrder)
{
    tradedQuantityOfOrder = _tradedQuantityOfOrder;
}

int PurchaseOrder::getOrderSource()
{
    return orderSource;
}

void PurchaseOrder::setOrderSource(int _orderSource)
{
    orderSource = _orderSource;
}

int PurchaseOrder::getAgressorIndicator(){
    return agressorIndicator;
}

void PurchaseOrder::setAgressorIndicator(string _agressorIndicator){
    agressorIndicator = stoi(_agressorIndicator);
}