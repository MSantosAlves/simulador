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
