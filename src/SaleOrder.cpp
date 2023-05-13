#include "SaleOrder.h"
#include "StringUtils.h"

#include <string>

using namespace std;

SaleOrder::SaleOrder() {}

SaleOrder::SaleOrder(string _sequentialOrderNumber, string _secondaryOrderID, string _priorityTime, string _orderPrice, string _totalQuantityOfOrder, string _tradedQuantityOfOrder) {
    StringUtils stringUtils;

    sequentialOrderNumber = _sequentialOrderNumber;
    secondaryOrderID = _secondaryOrderID;
    priorityTime = _priorityTime;
    priorityTimeInteger = stringUtils.dateToInt(_priorityTime);
    orderPrice = stoi(_orderPrice);
    totalQuantityOfOrder = stoi(_totalQuantityOfOrder);
    tradedQuantityOfOrder = stoi(_tradedQuantityOfOrder);
}

string SaleOrder::getSequentialOrderNumber() {
    return sequentialOrderNumber;
}

void SaleOrder::setSequentialOrderNumber(string _sequentialOrderNumber) {
    sequentialOrderNumber = _sequentialOrderNumber;
}

string SaleOrder::getSecondaryOrderID() {
    return secondaryOrderID;
}

void SaleOrder::setSecondaryOrderID(string _secondaryOrderID) {
    secondaryOrderID = _secondaryOrderID;
}

string SaleOrder::getPriorityTime() {
    return priorityTime;
}

void SaleOrder::setPriorityTime(string _priorityTime) {
    priorityTime = _priorityTime;
}

double SaleOrder::getOrderPrice() const {
    return orderPrice;
}

void SaleOrder::setOrderPrice(string _orderPrice) {
    orderPrice = stod(_orderPrice);
}

long long SaleOrder::getPriorityTimeInteger() const {
    return priorityTimeInteger;
}

void SaleOrder::setPriorityTimeInteger(long long _priorityTimeInteger) {
    priorityTimeInteger = _priorityTimeInteger;
}

int SaleOrder::getTotalQuantityOfOrder() {
    return totalQuantityOfOrder;
}

void SaleOrder::setTotalQuantityOfOrder(int _totalQuantityOfOrder) {
    totalQuantityOfOrder = _totalQuantityOfOrder;
}

int SaleOrder::getTradedQuantityOfOrder() {
    return tradedQuantityOfOrder;
}

void SaleOrder::setTradedQuantityOfOrder(int _tradedQuantityOfOrder) {
    tradedQuantityOfOrder = _tradedQuantityOfOrder;
}