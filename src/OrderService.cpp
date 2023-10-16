#include "OrderService.h"
#include "Order.h"
#include "StringUtils.h"
#include "ArrayUtils.h"
#include "OrderUtils.h"
#include "StockInfo.h"
#include "OrderStatuses.h"

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

void handleNewOrder(string symbol, Order order, map<string, StockInfo> *offersBook, ArrayUtils arrayUtils, OrderUtils orderUtils, ServerResponseSender *responseSender)
{
    bool isBuyOrder = order.getOrderSide() == "1";

    if (isBuyOrder)
    {
        PurchaseOrder purchaseOrderBuffer;
        purchaseOrderBuffer = *(new PurchaseOrder(order.getSequentialOrderNumber(), order.getSecondaryOrderID(), order.getPriorityTime(), order.getOrderPrice(), order.getTotalQuantityOfOrder(), order.getTradedQuantityOfOrder(), order.getOrderSource()));

        // Market offers
        if(purchaseOrderBuffer.getOrderPrice() == 0){
            purchaseOrderBuffer.setOrderPrice(to_string((*offersBook)[symbol].ask));
        }

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
            orderUtils.executePossibleTrades(symbol, offersBook, 1, responseSender);
        }
    }
    else
    {
        SaleOrder saleOrderBuffer;
        saleOrderBuffer = *(new SaleOrder(order.getSequentialOrderNumber(), order.getSecondaryOrderID(), order.getPriorityTime(), order.getOrderPrice(), order.getTotalQuantityOfOrder(), order.getTradedQuantityOfOrder(), order.getOrderSource()));

        // Market offers
        if(saleOrderBuffer.getOrderPrice() == 0){
            saleOrderBuffer.setOrderPrice(to_string((*offersBook)[symbol].bid));
        }

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
            orderUtils.executePossibleTrades(symbol, offersBook, 2, responseSender);
        }
    }
}

void handleReplacedOrder(string symbol, Order order, map<string, StockInfo> *offersBook, ArrayUtils arrayUtils, OrderUtils orderUtils, ServerResponseSender *responseSender)
{
    string sequentialOrderNumber = order.getSequentialOrderNumber();
    int targetIdx = -1;

    if (order.getOrderSide() == "1")
    {
        for (int i = 0; i < (*offersBook)[symbol].purchaseOrders.size(); i++)
        {
            if ((*offersBook)[symbol].purchaseOrders[i].getSequentialOrderNumber() == sequentialOrderNumber)
            {
                targetIdx = i;
                break;
            }
        }

        if (targetIdx < 0)
        {
            return;
        }

        PurchaseOrder updatedOrder = *(new PurchaseOrder(order.getSequentialOrderNumber(), order.getSecondaryOrderID(), order.getPriorityTime(), order.getOrderPrice(), order.getTotalQuantityOfOrder(), order.getTradedQuantityOfOrder(), order.getOrderSource()));

        // Market offers
        if(updatedOrder.getOrderPrice() == 0){
            updatedOrder.setOrderPrice(to_string((*offersBook)[symbol].ask));
        }

        if ((*offersBook)[symbol].purchaseOrders[targetIdx].getOrderPrice() != updatedOrder.getOrderPrice() || (*offersBook)[symbol].purchaseOrders[targetIdx].getTotalQuantityOfOrder() < updatedOrder.getTotalQuantityOfOrder())
        {
            // Price changed or order quantity increased
            // Priority loss => Update order position in queue
            (*offersBook)[symbol].purchaseOrders.erase((*offersBook)[symbol].purchaseOrders.begin() + targetIdx);
            int priceLastIndex = arrayUtils.searchPurchaseOrdersPriceLastIndex((*offersBook)[symbol].purchaseOrders, updatedOrder.getOrderPrice());

            if (priceLastIndex >= 0)
            {

                int newPriority = (*offersBook)[symbol].purchaseOrders[priceLastIndex].getPriorityTimeInteger() + 1;
                updatedOrder.setPriorityTimeInteger(newPriority);
                updatedOrder.setPriorityTime(to_string(newPriority));

                (*offersBook)[symbol].purchaseOrders.insert((*offersBook)[symbol].purchaseOrders.begin() + priceLastIndex, updatedOrder);
            }
            else
            {
                arrayUtils.insertPurchaseOrder((*offersBook)[symbol].purchaseOrders, updatedOrder);


                if (updateBidPrice(updatedOrder, symbol, offersBook))
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
                    orderUtils.executePossibleTrades(symbol, offersBook, 1, responseSender);
                }
            }
        }
        else if ((*offersBook)[symbol].purchaseOrders[targetIdx].getTotalQuantityOfOrder() > updatedOrder.getTotalQuantityOfOrder())
        {
            // Order quantity decreased
            // Keep priority
            (*offersBook)[symbol].purchaseOrders[targetIdx].setTotalQuantityOfOrder(updatedOrder.getTotalQuantityOfOrder());
        }
    }
    else
    {
        for (int i = 0; i < (*offersBook)[symbol].saleOrders.size(); i++)
        {
            if ((*offersBook)[symbol].saleOrders[i].getSequentialOrderNumber() == sequentialOrderNumber)
            {
                targetIdx = i;
                break;
            }
        }

        if (targetIdx < 0)
        {
            return;
        }

        SaleOrder updatedOrder = *(new SaleOrder(order.getSequentialOrderNumber(), order.getSecondaryOrderID(), order.getPriorityTime(), order.getOrderPrice(), order.getTotalQuantityOfOrder(), order.getTradedQuantityOfOrder(), order.getOrderSource()));

        // Market offers
        if(updatedOrder.getOrderPrice() == 0){
            updatedOrder.setOrderPrice(to_string((*offersBook)[symbol].ask));
        }

        if ((*offersBook)[symbol].saleOrders[targetIdx].getOrderPrice() != updatedOrder.getOrderPrice() || (*offersBook)[symbol].saleOrders[targetIdx].getTotalQuantityOfOrder() < updatedOrder.getTotalQuantityOfOrder())
        {
            // Price changed or order quantity increased
            // Priority loss => Update order position in queue
            (*offersBook)[symbol].saleOrders.erase((*offersBook)[symbol].saleOrders.begin() + targetIdx);
            int priceLastIndex = arrayUtils.searchSaleOrdersPriceLastIndex((*offersBook)[symbol].saleOrders, updatedOrder.getOrderPrice());

            if (priceLastIndex >= 0)
            {

                int newPriority = (*offersBook)[symbol].saleOrders[priceLastIndex].getPriorityTimeInteger() + 1;
                updatedOrder.setPriorityTimeInteger(newPriority);
                updatedOrder.setPriorityTime(to_string(newPriority));

                (*offersBook)[symbol].saleOrders.insert((*offersBook)[symbol].saleOrders.begin() + priceLastIndex, updatedOrder);
            }
            else
            {
                arrayUtils.insertSaleOrder((*offersBook)[symbol].saleOrders, updatedOrder);

                if (updateAskPrice(updatedOrder, symbol, offersBook))
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
                    orderUtils.executePossibleTrades(symbol, offersBook, 2, responseSender);
                }
            }
        }
        else if ((*offersBook)[symbol].saleOrders[targetIdx].getTotalQuantityOfOrder() > updatedOrder.getTotalQuantityOfOrder())
        {
            // Order quantity decreased
            // Keep priority
            (*offersBook)[symbol].saleOrders[targetIdx].setTotalQuantityOfOrder(updatedOrder.getTotalQuantityOfOrder());
        }
    }
}

void handleCancelOrExpiredOrder(string symbol, Order order, map<string, StockInfo> *offersBook)
{
    string sequentialOrderNumber = order.getSequentialOrderNumber();
    int targetIdx = -1;

    if (order.getOrderSide() == "1")
    {
        for (int i = 0; i < (*offersBook)[symbol].purchaseOrders.size(); i++)
        {
            if ((*offersBook)[symbol].purchaseOrders[i].getSequentialOrderNumber() == sequentialOrderNumber)
            {
                targetIdx = i;
                break;
            }
        }

        if (targetIdx < 0)
        {
            return;
        }

        (*offersBook)[symbol].purchaseOrders.erase((*offersBook)[symbol].purchaseOrders.begin() + targetIdx);

        if (targetIdx == 0)
        {
            // Update BID price
            (*offersBook)[symbol].bid = (*offersBook)[symbol].purchaseOrders[0].getOrderPrice();
        }
    }
    else
    {
        for (int i = 0; i < (*offersBook)[symbol].saleOrders.size(); i++)
        {
            if ((*offersBook)[symbol].saleOrders[i].getSequentialOrderNumber() == sequentialOrderNumber)
            {
                targetIdx = i;
                break;
            }
        }

        if (targetIdx < 0)
        {
            return;
        }

        (*offersBook)[symbol].saleOrders.erase((*offersBook)[symbol].saleOrders.begin() + targetIdx);

        if (targetIdx == 0)
        {
            // Update ASK price
            (*offersBook)[symbol].ask = (*offersBook)[symbol].saleOrders[0].getOrderPrice();
        }
    }
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

        order = orderUtils.parseOrder(rawCurrOrder, stringUtils);

        string delimiter = ":";
        int hourOfTheDay = stoi(stringUtils.split(order.getPriorityTime(), delimiter)[0]);
        long long sequentialOrderNumber = stoll(order.getSequentialOrderNumber());

        //sequentialOrderNumber < 725370158444
        if (hourOfTheDay < 9 || hourOfTheDay > 18)
        {
            semaphore->release();
            this_thread::sleep_for(timespan);
            continue;
        }

        int orderStatus = order.getOrderStatus() == "C" ? 9 : stoi(order.getOrderStatus());

        if (orderStatus == OrderStatuses::Status::REJECTED || orderStatus == OrderStatuses::Status::PARTIALLY_FILLED || orderStatus == OrderStatuses::Status::FILLED)
        {
            // Just ignore
        }
        else if (orderStatus == OrderStatuses::Status::CANCELLED || orderStatus == OrderStatuses::Status::EXPIRED)
        {
            handleCancelOrExpiredOrder(symbol, order, offersBook);
        }
        else if (orderStatus == OrderStatuses::Status::REPLACED)
        {
            handleReplacedOrder(symbol, order, offersBook, arrayUtils, orderUtils, responseSender);
        }
        else if (orderStatus == OrderStatuses::Status::NEW)
        {
            handleNewOrder(symbol, order, offersBook, arrayUtils, orderUtils, responseSender);
        }
        else
        {
            cout << "[ERROR] Unmaped Order Status: " << order.getOrderStatus() << endl;
        }
        semaphore->release();
        this_thread::sleep_for(timespan);
    }

    tradeHistoryFile.close();
}