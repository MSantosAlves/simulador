#include "SaleOffer.h"
#include "StringUtils.h"

#include <string>

using namespace std;

SaleOffer::SaleOffer() {}

SaleOffer::SaleOffer(string _sequentialOrderNumber, string _secondaryOrderID, string _priorityTime, string _orderPrice, string _totalQuantityOfOrder, string _tradedQuantityOfOrder) {
    StringUtils stringUtils;

    sequentialOrderNumber = _sequentialOrderNumber;
    secondaryOrderID = _secondaryOrderID;
    priorityTime = _priorityTime;
    priorityTimeInteger = stringUtils.dateToInt(_priorityTime);
    orderPrice = stoi(_orderPrice);
    totalQuantityOfOrder = stoi(_totalQuantityOfOrder);
    tradedQuantityOfOrder = stoi(_tradedQuantityOfOrder);
}

string SaleOffer::getSequentialOrderNumber() {
    return sequentialOrderNumber;
}

void SaleOffer::setSequentialOrderNumber(string _sequentialOrderNumber) {
    sequentialOrderNumber = _sequentialOrderNumber;
}

string SaleOffer::getSecondaryOrderID() {
    return secondaryOrderID;
}

void SaleOffer::setSecondaryOrderID(string _secondaryOrderID) {
    secondaryOrderID = _secondaryOrderID;
}

string SaleOffer::getPriorityTime() {
    return priorityTime;
}

void SaleOffer::setPriorityTime(string _priorityTime) {
    priorityTime = _priorityTime;
}

double SaleOffer::getOrderPrice() const {
    return orderPrice;
}

void SaleOffer::setOrderPrice(string _orderPrice) {
    orderPrice = stod(_orderPrice);
}

long long SaleOffer::getPriorityTimeInteger() const {
    return priorityTimeInteger;
}

void SaleOffer::setPriorityTimeInteger(long long _priorityTimeInteger) {
    priorityTimeInteger = _priorityTimeInteger;
}

int SaleOffer::getTotalQuantityOfOrder() {
    return totalQuantityOfOrder;
}

void SaleOffer::setTotalQuantityOfOrder(int _totalQuantityOfOrder) {
    totalQuantityOfOrder = _totalQuantityOfOrder;
}

int SaleOffer::getTradedQuantityOfOrder() {
    return tradedQuantityOfOrder;
}

void SaleOffer::setTradedQuantityOfOrder(int _tradedQuantityOfOrder) {
    tradedQuantityOfOrder = _tradedQuantityOfOrder;
}