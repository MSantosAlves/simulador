#include "OrderService.h"
#include "Order.h"
#include "StringUtils.h"
#include "ArrayUtils.h"
#include "OrderUtils.h"
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

// Class methods

OrderService::OrderService(vector<string> _targetStocks) {
    targetStocks = _targetStocks;
}

void OrderService::startProcessOrders(vector<string>* rawOrdersQueue, map<string, StockInfo>* offersBook, Semaphore* semaphore, Trader* traderAccount)
{
    StringUtils stringUtils;
    OrderUtils orderUtils;
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
            orderBuffer = orderUtils.parseOrder(currOrder, stringUtils);
            orderUtils.orderMatching(symbol, orderBuffer, offersBook, arrayUtils, traderAccount);
        }else {
            symbol = "";
        }
        
        semaphore->release();
        this_thread::sleep_for(timespan);

    }
}