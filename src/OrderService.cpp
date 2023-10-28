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

void handleNewOrder(string symbol, Order order, map<string, StockInfo> *offersBook, ArrayUtils arrayUtils, OrderUtils *orderUtils, ServerResponseSender *responseSender, Clock *clock)
{
    bool isBuyOrder = order.getOrderSide() == "1";

    if (isBuyOrder)
    {
        PurchaseOrder purchaseOrderBuffer;
        purchaseOrderBuffer = *(new PurchaseOrder(order.getSequentialOrderNumber(), order.getSecondaryOrderID(), order.getPriorityTime(), order.getPriorityIndicator(), order.getOrderPrice(), order.getTotalQuantityOfOrder(), order.getTradedQuantityOfOrder(), order.getOrderSource(), order.getAggressorIndicator()));

        // Market offers
        if (purchaseOrderBuffer.getOrderPrice() == 0)
        {
            purchaseOrderBuffer.setOrderPrice(to_string((*offersBook)[symbol].ask));
        }

        // Insert new offer on purchases orders queue
        arrayUtils.insertPurchaseOrder((*offersBook)[symbol].purchaseOrders, purchaseOrderBuffer);

        if (updateBidPrice(purchaseOrderBuffer, symbol, offersBook))
        {
            // If order price is higher than ask price, offer will be immediately executed, and bid price shouldn't be updated
            if (purchaseOrderBuffer.getOrderPrice() < (*offersBook)[symbol].ask)
            {
                json jsonObject = {
                    {"event", "UPDATE_BOOK"},
                    {"symbol", symbol},
                    {"direction", "BID"},
                    {"time", clock->getSimulationTimeHumanReadable()},
                    {"traded_volume", (*offersBook)[symbol].totalTradedQuantity},
                    {"buy_offers", (*offersBook)[symbol].purchaseOrders.size()},
                    {"sell_offers", (*offersBook)[symbol].saleOrders.size()},
                    {"bid", (*offersBook)[symbol].bid},
                    {"ask", (*offersBook)[symbol].ask},
                    {"last_trade_price", (*offersBook)[symbol].lastTradePrice}};
                responseSender->sendResponse(jsonObject);
            }

            orderUtils->executePossibleTrades(symbol, offersBook, 1, responseSender);
        }
    }
    else
    {
        SaleOrder saleOrderBuffer;
        saleOrderBuffer = *(new SaleOrder(order.getSequentialOrderNumber(), order.getSecondaryOrderID(), order.getPriorityTime(), order.getPriorityIndicator(), order.getOrderPrice(), order.getTotalQuantityOfOrder(), order.getTradedQuantityOfOrder(), order.getOrderSource(), order.getAggressorIndicator()));

        // Market offers
        if (saleOrderBuffer.getOrderPrice() == 0)
        {
            saleOrderBuffer.setOrderPrice(to_string((*offersBook)[symbol].bid));
        }

        // Insert new offer on sales orders queue
        arrayUtils.insertSaleOrder((*offersBook)[symbol].saleOrders, saleOrderBuffer);

        if (updateAskPrice(saleOrderBuffer, symbol, offersBook))
        {
            // If order price is lower than bid price, offer will be immediately executed, and ask price shouldn't be updated
            if (saleOrderBuffer.getOrderPrice() > (*offersBook)[symbol].bid)
            {
                json jsonObject = {
                    {"event", "UPDATE_BOOK"},
                    {"symbol", symbol},
                    {"direction", "ASK"},
                    {"time", clock->getSimulationTimeHumanReadable()},
                    {"traded_volume", (*offersBook)[symbol].totalTradedQuantity},
                    {"buy_offers", (*offersBook)[symbol].purchaseOrders.size()},
                    {"sell_offers", (*offersBook)[symbol].saleOrders.size()},
                    {"bid", (*offersBook)[symbol].bid},
                    {"ask", (*offersBook)[symbol].ask},
                    {"last_trade_price", (*offersBook)[symbol].lastTradePrice}};
                responseSender->sendResponse(jsonObject);
            }

            orderUtils->executePossibleTrades(symbol, offersBook, 2, responseSender);
        }
    }
}

void handleReplacedOrder(string symbol, Order order, map<string, StockInfo> *offersBook, ArrayUtils arrayUtils, OrderUtils *orderUtils, ServerResponseSender *responseSender, Clock *clock)
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

        PurchaseOrder updatedOrder = *(new PurchaseOrder(order.getSequentialOrderNumber(), order.getSecondaryOrderID(), order.getPriorityTime(), order.getPriorityIndicator(), order.getOrderPrice(), order.getTotalQuantityOfOrder(), order.getTradedQuantityOfOrder(), order.getOrderSource(), order.getAggressorIndicator()));

        // Market offers
        if (updatedOrder.getOrderPrice() == 0)
        {
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
                updatedOrder.setPriorityIndicator(newPriority);

                (*offersBook)[symbol].purchaseOrders.insert((*offersBook)[symbol].purchaseOrders.begin() + priceLastIndex, updatedOrder);
            }
            else
            {
                arrayUtils.insertPurchaseOrder((*offersBook)[symbol].purchaseOrders, updatedOrder);

                if (updateBidPrice(updatedOrder, symbol, offersBook))
                {
                    // If order price is higher than ask price, offer will be immediately executed, and bid price shouldn't be updated
                    if (updatedOrder.getOrderPrice() < (*offersBook)[symbol].ask)
                    {
                        json jsonObject = {
                            {"event", "UPDATE_BOOK"},
                            {"symbol", symbol},
                            {"direction", "BID"},
                            {"time", clock->getSimulationTimeHumanReadable()},
                            {"traded_volume", (*offersBook)[symbol].totalTradedQuantity},
                            {"buy_offers", (*offersBook)[symbol].purchaseOrders.size()},
                            {"sell_offers", (*offersBook)[symbol].saleOrders.size()},
                            {"bid", (*offersBook)[symbol].bid},
                            {"ask", (*offersBook)[symbol].ask},
                            {"last_trade_price", (*offersBook)[symbol].lastTradePrice}};
                        responseSender->sendResponse(jsonObject);
                    }

                    orderUtils->executePossibleTrades(symbol, offersBook, 1, responseSender);
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

        SaleOrder updatedOrder = *(new SaleOrder(order.getSequentialOrderNumber(), order.getSecondaryOrderID(), order.getPriorityTime(), order.getPriorityIndicator(), order.getOrderPrice(), order.getTotalQuantityOfOrder(), order.getTradedQuantityOfOrder(), order.getOrderSource(), order.getAggressorIndicator()));

        // Market offers
        if (updatedOrder.getOrderPrice() == 0)
        {
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

                long long newPriority = (*offersBook)[symbol].saleOrders[priceLastIndex].getPriorityIndicator() + 1;
                updatedOrder.setPriorityIndicator(newPriority);

                (*offersBook)[symbol].saleOrders.insert((*offersBook)[symbol].saleOrders.begin() + priceLastIndex, updatedOrder);
            }
            else
            {
                arrayUtils.insertSaleOrder((*offersBook)[symbol].saleOrders, updatedOrder);

                if (updateAskPrice(updatedOrder, symbol, offersBook))
                {
                    // If order price is lower than bid price, offer will be immediately executed, and ask price shouldn't be updated
                    if (updatedOrder.getOrderPrice() > (*offersBook)[symbol].bid)
                    {
                        json jsonObject = {
                            {"event", "UPDATE_BOOK"},
                            {"symbol", symbol},
                            {"direction", "ASK"},
                            {"time", clock->getSimulationTimeHumanReadable()},
                            {"traded_volume", (*offersBook)[symbol].totalTradedQuantity},
                            {"buy_offers", (*offersBook)[symbol].purchaseOrders.size()},
                            {"sell_offers", (*offersBook)[symbol].saleOrders.size()},
                            {"bid", (*offersBook)[symbol].bid},
                            {"ask", (*offersBook)[symbol].ask},
                            {"last_trade_price", (*offersBook)[symbol].lastTradePrice}};
                        responseSender->sendResponse(jsonObject);
                    }

                    orderUtils->executePossibleTrades(symbol, offersBook, 2, responseSender);
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

OrderService::OrderService(Clock *_clock)
{
    clock = _clock;
}

void OrderService::startProcessOrders(vector<string> *rawOrdersQueue, map<string, StockInfo> *offersBook, Semaphore *semaphore, ServerResponseSender *responseSender)
{
    StringUtils stringUtils;
    OrderUtils *orderUtils = new OrderUtils(clock);
    Order order;
    ArrayUtils arrayUtils;
    chrono::nanoseconds timespan(1);
    string symbol = "";
    string rawCurrOrder;

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

        order = orderUtils->parseOrder(rawCurrOrder, stringUtils);

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
            handleReplacedOrder(symbol, order, offersBook, arrayUtils, orderUtils, responseSender, clock);
        }
        else if (orderStatus == OrderStatuses::Status::NEW)
        {
            handleNewOrder(symbol, order, offersBook, arrayUtils, orderUtils, responseSender, clock);
        }
        else
        {
            cout << "[ERROR] Unmaped Order Status: " << order.getOrderStatus() << endl;
        }
        semaphore->release();
        this_thread::sleep_for(timespan);
    }
}