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

string generateFilename() {
    string timestamp = to_string(chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count());
    string file_name = "trade_history_" + timestamp + ".txt";
    return file_name;
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

   
    filesystem::path pwd = filesystem::current_path();
    string sysFileChar = (_WIN64 || _WIN32) ? "\\" : "/";
    string pwdString = stringUtils.pathToString(pwd);
    string file_name = generateFilename();
    string fullPath = pwdString + sysFileChar + "data" + sysFileChar + "history" + sysFileChar + file_name;

    ofstream tradesHistoryFile;
    tradesHistoryFile.open(fullPath);
    
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
            orderUtils.orderMatching(symbol, orderBuffer, offersBook, arrayUtils, traderAccount, tradesHistoryFile);
        }else {
            symbol = "";
        }
        
        semaphore->release();
        this_thread::sleep_for(timespan);

    }

    tradesHistoryFile.close();
}