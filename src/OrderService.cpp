#include "OrderService.h"
#include "Order.h"
#include "StringUtils.h"
#include "ArrayUtils.h"
#include "StockInfo.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <atomic>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <map>
#include <algorithm>

using namespace std;

// Aux methods
bool isTargetSymbol(vector<string> targetsStocks, string stock) {
    StringUtils stringUtils;
    return stringUtils.include(targetsStocks, stock) != -1;
}

vector<string> removeWithSpacesFromSplitedString(vector<string> splitedString, StringUtils stringUtils)
{
    for (int i = 0; i < splitedString.size(); i++) {
        splitedString[i] = stringUtils.removeWhiteSpaces(splitedString[i]);
    }
    return splitedString;
}

Order processOrder(string order, StringUtils stringUtils)
{
    vector<string> splitedString;
    char const* delimiter = ";";
    int stringSize;
    string type;
    splitedString = removeWithSpacesFromSplitedString(stringUtils.split(order, (*delimiter)), stringUtils);

    stringSize = splitedString.size();
    Order* orderBuffer = new Order();

    // stringSize <= 2 => Data header
    if (stringSize > 2) {
        type = splitedString[stringSize - 1];
        if (type == "VDA" || type == "CPA") {
            orderBuffer->setSessionDate(splitedString[0]);
            orderBuffer->setInstrumentSymbol(splitedString[1]);
            orderBuffer->setOrderSide(splitedString[2]);
            orderBuffer->setSequentialOrderNumber(splitedString[3]);
            orderBuffer->setSecondaryOrderID(splitedString[4]);
            orderBuffer->setExecutionType(splitedString[5]);
            orderBuffer->setPriorityTime(splitedString[6]);
            orderBuffer->setPriorityIndicator(splitedString[7]);
            orderBuffer->setOrderPrice(splitedString[8]);
            orderBuffer->setTotalQuantityOfOrder(splitedString[9]);
            orderBuffer->setTradedQuantityOfOrder(splitedString[10]);
            orderBuffer->setOrderDate(splitedString[11]);
            orderBuffer->setOrderDatetimeEntry(splitedString[12]);
            orderBuffer->setOrderStatus(splitedString[13]);
            orderBuffer->setAggressorIndicator(splitedString[14]);
            orderBuffer->setMember(splitedString[15]);
            orderBuffer->setType(type);
            orderBuffer->setArrayOfCalculatedFields(splitedString);
            return (*orderBuffer);
        }
        else if (type == "NEG") {
            orderBuffer->setSessionDate(splitedString[1]);
            orderBuffer->setInstrumentSymbol(splitedString[2]);
            orderBuffer->setIradeNumber(splitedString[3]);
            orderBuffer->setIradePrice(splitedString[4]);
            orderBuffer->setIradedQuantity(splitedString[5]);
            orderBuffer->setIradeTime(splitedString[6]);
            orderBuffer->setIradeIndicator(splitedString[7]);
            orderBuffer->setBuyOrderDate(splitedString[8]);
            orderBuffer->setSequentialBuyOrderNumber(splitedString[9]);
            orderBuffer->setSecondaryOrderIDBuyOrder(splitedString[10]);
            orderBuffer->setAggressorBuyOrderIndicator(splitedString[11]);
            orderBuffer->setSellOrderDate(splitedString[12]);
            orderBuffer->setSequentialSellOrderNumber(splitedString[13]);
            orderBuffer->setSecondaryOrderIDSellOrder(splitedString[14]);
            orderBuffer->setAggressorSellOrderIndicator(splitedString[15]);
            orderBuffer->setCrossTradeIndicator(splitedString[16]);
            orderBuffer->setBuyMember(splitedString[17]);
            orderBuffer->setSellMember(splitedString[18]);
            orderBuffer->setType(type);
            orderBuffer->setArrayOfCalculatedFields(splitedString);
            return (*orderBuffer);
        }
    }

    return (*orderBuffer);
}

// Class methods

OrderService::OrderService(vector<string> _targetStocks) {
    targetStocks = _targetStocks;
}

void OrderService::startProcessOrders(vector<string>* rawOrdersQueue, map<string, StockInfo>* offersBook, Semaphore* semaphore)
{
    StringUtils stringUtils;
    Order orderBuffer;
    ArrayUtils arrayUtils;
    chrono::nanoseconds timespan(1);
    string symbol = "";
    string currOrder;
    PurchaseOrder purchaseOrderBuffer;
    SaleOrder saleOrderBuffer;

    while (true) {
        semaphore->acquire();

        if (rawOrdersQueue->size() == 0) {
            semaphore->release();
            continue;
        }
        currOrder = rawOrdersQueue->front();
        symbol = stringUtils.removeWhiteSpaces(stringUtils.split(currOrder, ';')[1]);
        rawOrdersQueue->erase(rawOrdersQueue->begin());
     
        if (symbol != "" && isTargetSymbol(targetStocks, symbol)) {
            orderBuffer = processOrder(currOrder, stringUtils);
            if (orderBuffer.getOrderSide() == "1") {
                purchaseOrderBuffer = *(new PurchaseOrder(orderBuffer.getSequentialOrderNumber(), orderBuffer.getSecondaryOrderID(), orderBuffer.getPriorityTime(), orderBuffer.getOrderPrice(), orderBuffer.getTotalQuantityOfOrder(), orderBuffer.getTradedQuantityOfOrder()));
                if ((*offersBook)[symbol].purchaseOrders.size() == 0 || purchaseOrderBuffer.getOrderPrice() > (*offersBook)[symbol].purchaseOrders[0].getOrderPrice()) {
                    (*offersBook)[symbol].bid = purchaseOrderBuffer.getOrderPrice();
                }
                arrayUtils.insertPurchaseOrder((*offersBook)[symbol].purchaseOrders, purchaseOrderBuffer);
            }
            else if (orderBuffer.getOrderSide() == "2") {
                saleOrderBuffer = *(new SaleOrder(orderBuffer.getSequentialOrderNumber(), orderBuffer.getSecondaryOrderID(), orderBuffer.getPriorityTime(), orderBuffer.getOrderPrice(), orderBuffer.getTotalQuantityOfOrder(), orderBuffer.getTradedQuantityOfOrder()));
                if ((*offersBook)[symbol].saleOrders.size() == 0 || saleOrderBuffer.getOrderPrice() < (*offersBook)[symbol].saleOrders[0].getOrderPrice()) {
                    (*offersBook)[symbol].ask = saleOrderBuffer.getOrderPrice();
                }
                arrayUtils.insertSaleOrder((*offersBook)[symbol].saleOrders, saleOrderBuffer);
            }
        }
        else {
            symbol = "";
        }
        
  
        semaphore->release();
        this_thread::sleep_for(timespan);
        
    }
}