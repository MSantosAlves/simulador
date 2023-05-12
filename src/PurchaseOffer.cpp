#include "PurchaseOffer.h"
#include "StringUtils.h"
#include <string>

using namespace std;

PurchaseOffer::PurchaseOffer() {}

PurchaseOffer::PurchaseOffer(string _sequentialOrderNumber, string _secondaryOrderID, string _priorityTime, string _orderPrice, string _totalQuantityOfOrder, string _tradedQuantityOfOrder) {
    StringUtils stringUtils;
    sequentialOrderNumber = _sequentialOrderNumber;
    secondaryOrderID = _secondaryOrderID;
    priorityTime = _priorityTime;
    priorityTimeInteger = stringUtils.dateToInt(_priorityTime);
    orderPrice = stoi(_orderPrice);
    totalQuantityOfOrder = stoi(_totalQuantityOfOrder);
    tradedQuantityOfOrder = stoi(_tradedQuantityOfOrder);
};

string PurchaseOffer::getSequentialOrderNumber(){
    return sequentialOrderNumber;
}

void PurchaseOffer::setSequentialOrderNumber(string _sequentialOrderNumber){
    sequentialOrderNumber = _sequentialOrderNumber;
}

string PurchaseOffer::getSecondaryOrderID() {
    return secondaryOrderID;
}

void PurchaseOffer::setSecondaryOrderID(string _secondaryOrderID){
    secondaryOrderID = _secondaryOrderID;
}

string PurchaseOffer::getPriorityTime() {
    return priorityTime;
}

void PurchaseOffer::setPriorityTime(string _priorityTime){
    priorityTime = _priorityTime;
}

double PurchaseOffer::getOrderPrice() const {
    return orderPrice;
}

void PurchaseOffer::setOrderPrice(string _orderPrice){
    orderPrice = stod(_orderPrice);
}

long long PurchaseOffer::getPriorityTimeInteger() const {
    return priorityTimeInteger;
}

void PurchaseOffer::setPriorityTimeInteger(long long _priorityTimeInteger){
    priorityTimeInteger = _priorityTimeInteger;
}

int PurchaseOffer::getTotalQuantityOfOrder() {
    return totalQuantityOfOrder;
}

void PurchaseOffer::setTotalQuantityOfOrder(int _totalQuantityOfOrder){
    totalQuantityOfOrder = _totalQuantityOfOrder;
}

int PurchaseOffer::getTradedQuantityOfOrder() {
    return tradedQuantityOfOrder;
}

void PurchaseOffer::setTradedQuantityOfOrder(int _tradedQuantityOfOrder){
    tradedQuantityOfOrder = _tradedQuantityOfOrder;
}
