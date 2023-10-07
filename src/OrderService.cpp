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
bool isTargetSymbol(vector<string> targetsStocks, string stock)
{
    StringUtils stringUtils;
    return stringUtils.include(targetsStocks, stock) != -1;
}

string generateFilename()
{
    string timestamp = to_string(chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count());
    string file_name = "trade_history_" + timestamp + ".txt";
    return file_name;
}

bool updateBidPrice(PurchaseOrder purchaseOrder, string symbol, map<string, StockInfo> *offersBook)
{
    StockInfo stockInfo = (*offersBook)[symbol];

    if (stockInfo.bid == 0 || stockInfo.bid < purchaseOrder.getOrderPrice())
    {
        (*offersBook)[symbol].bid = purchaseOrder.getOrderPrice();
        return true;
    }
    return false;
}

bool updateAskPrice(SaleOrder saleOrder, string symbol, map<string, StockInfo> *offersBook)
{
    StockInfo stockInfo = (*offersBook)[symbol];

    if (stockInfo.ask == 0.0 || stockInfo.ask > saleOrder.getOrderPrice())
    {
        (*offersBook)[symbol].ask = saleOrder.getOrderPrice();
        return true;
    }
    return false;
}

// Class methods

OrderService::OrderService(vector<string> _targetStocks)
{
    targetStocks = _targetStocks;
}

void OrderService::startProcessOrders(vector<string> *rawOrdersQueue, map<string, StockInfo> *offersBook, Semaphore *semaphore, ServerResponseSender *responseSender)
{
    StringUtils stringUtils;
    OrderUtils orderUtils;
    Order order;
    ArrayUtils arrayUtils;
    chrono::nanoseconds timespan(1);
    string symbol = "";
    string rawCurrOrder;

    filesystem::path pwd = filesystem::current_path();
    string sysFileChar = "/"; //(_WIN64 || _WIN32) ? "\\" : "/";
    string pwdString = stringUtils.pathToString(pwd);
    string file_name = generateFilename();
    string fullPath = "data" + sysFileChar + "history" + sysFileChar + file_name;

    ofstream tradeHistoryFile;
    tradeHistoryFile.open(fullPath);

    PurchaseOrder purchaseOrderBuffer;
    SaleOrder saleOrderBuffer;

    while (true)
    {
        semaphore->acquire();

        if (rawOrdersQueue->size() == 0)
        {
            semaphore->release();
            this_thread::sleep_for(timespan);
            continue;
        }

        rawCurrOrder = rawOrdersQueue->front();
        symbol = stringUtils.removeWhiteSpaces(stringUtils.split(rawCurrOrder, ';')[1]);
        rawOrdersQueue->erase(rawOrdersQueue->begin());

        if (symbol != "" && isTargetSymbol(targetStocks, symbol))
        {
            order = orderUtils.parseOrder(rawCurrOrder, stringUtils);

            bool isBuyOrder = order.getOrderSide() == "1";

            if (isBuyOrder)
            {
                PurchaseOrder purchaseOrderBuffer;
                purchaseOrderBuffer = *(new PurchaseOrder(order.getSequentialOrderNumber(), order.getSecondaryOrderID(), order.getPriorityTime(), order.getOrderPrice(), order.getTotalQuantityOfOrder(), order.getTradedQuantityOfOrder(), order.getOrderSource()));

                // Insert new offer on purchases orders queue
                arrayUtils.insertPurchaseOrder((*offersBook)[symbol].purchaseOrders, purchaseOrderBuffer);

                if (updateBidPrice(purchaseOrderBuffer, symbol, offersBook))
                {
                    json jsonObject = {
                        {"event", "UPDATE_BOOK"},
                        {"symbol", symbol},
                        {"traded_volume", (*offersBook)[symbol].totalTradedQuantity},
                        {"buy_offers", (*offersBook)[symbol].purchaseOrders.size()},
                        {"sell_offers", (*offersBook)[symbol].saleOrders.size()},
                        {"bid", (*offersBook)[symbol].bid},
                        {"ask", (*offersBook)[symbol].ask},
                        {"last_trade_price", (*offersBook)[symbol].lastTradePrice}};
                    responseSender->sendResponse(jsonObject);
                    orderUtils.executePossibleTrades(symbol, offersBook, 1, tradeHistoryFile, responseSender);
                }
            }
            else
            {
                SaleOrder saleOrderBuffer;
                saleOrderBuffer = *(new SaleOrder(order.getSequentialOrderNumber(), order.getSecondaryOrderID(), order.getPriorityTime(), order.getOrderPrice(), order.getTotalQuantityOfOrder(), order.getTradedQuantityOfOrder(), order.getOrderSource()));

                // Insert new offer on sales orders queue
                arrayUtils.insertSaleOrder((*offersBook)[symbol].saleOrders, saleOrderBuffer);

                if (updateAskPrice(saleOrderBuffer, symbol, offersBook))
                {
                    json jsonObject = {
                        {"event", "UPDATE_BOOK"},
                        {"symbol", symbol},
                        {"traded_volume", (*offersBook)[symbol].totalTradedQuantity},
                        {"buy_offers", (*offersBook)[symbol].purchaseOrders.size()},
                        {"sell_offers", (*offersBook)[symbol].saleOrders.size()},
                        {"bid", (*offersBook)[symbol].bid},
                        {"ask", (*offersBook)[symbol].ask},
                        {"last_trade_price", (*offersBook)[symbol].lastTradePrice}};
                    responseSender->sendResponse(jsonObject);
                    orderUtils.executePossibleTrades(symbol, offersBook, 2, tradeHistoryFile, responseSender);
                }
            }
        }
        else
        {
            symbol = "";
        }

        semaphore->release();
        this_thread::sleep_for(timespan);
    }

    tradeHistoryFile.close();
}