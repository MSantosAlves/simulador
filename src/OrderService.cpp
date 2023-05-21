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

Order parseOrder(string order, StringUtils stringUtils)
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

void processNewOrder(string symbol, Order order, map<string, StockInfo>* offersBook, ArrayUtils arrayUtils, Trader* traderAccount) {
    const string PURCHASE_ORDER = "1";
    const string SALE_ORDER = "2";
    if (order.getOrderSide() == PURCHASE_ORDER) {
        PurchaseOrder purchaseOrderBuffer;
        purchaseOrderBuffer = *(new PurchaseOrder(order.getSequentialOrderNumber(), order.getSecondaryOrderID(), order.getPriorityTime(), order.getOrderPrice(), order.getTotalQuantityOfOrder(), order.getTradedQuantityOfOrder()));
        
        if (purchaseOrderBuffer.getOrderPrice() >= 0 && purchaseOrderBuffer.getOrderPrice() <= (*offersBook)[symbol].ask) {

            SaleOrder currSaleOrder;
            int tradedQty;
            int i = 0;
            do {
                currSaleOrder = (*offersBook)[symbol].saleOrders[i];

                if (purchaseOrderBuffer.getTotalQuantityOfOrder() >= currSaleOrder.getTotalQuantityOfOrder()) {
                    tradedQty = currSaleOrder.getTotalQuantityOfOrder();
                    purchaseOrderBuffer.setTotalQuantityOfOrder(purchaseOrderBuffer.getTotalQuantityOfOrder() - tradedQty);
                    purchaseOrderBuffer.setTradedQuantityOfOrder(purchaseOrderBuffer.getTradedQuantityOfOrder() + tradedQty);
                    currSaleOrder.setTotalQuantityOfOrder(0);
                }else {
                    tradedQty = purchaseOrderBuffer.getTotalQuantityOfOrder();
                    purchaseOrderBuffer.setTotalQuantityOfOrder(0);
                    purchaseOrderBuffer.setTradedQuantityOfOrder(purchaseOrderBuffer.getTradedQuantityOfOrder() + tradedQty);
                    currSaleOrder.setTotalQuantityOfOrder(currSaleOrder.getTotalQuantityOfOrder() - tradedQty);
                }

                (*offersBook)[symbol].saleOrders[i] = currSaleOrder;
                (*offersBook)[symbol].totalTradedQuantity += tradedQty;
                (*offersBook)[symbol].historicalPrices.push_back(purchaseOrderBuffer.getOrderPrice());

                i++;
            } while (purchaseOrderBuffer.getTotalQuantityOfOrder() > 0 && currSaleOrder.getOrderPrice() <= purchaseOrderBuffer.getOrderPrice());

            // Update purchase orders
            if (purchaseOrderBuffer.getTotalQuantityOfOrder() > 0) {
                (*offersBook)[symbol].purchaseOrders.insert((*offersBook)[symbol].purchaseOrders.begin(), purchaseOrderBuffer);
            }
            // Update sale orders (erase all sales where totalQuantityOfOrder = 0)
			(*offersBook)[symbol].saleOrders.erase(remove_if((*offersBook)[symbol].saleOrders.begin(), (*offersBook)[symbol].saleOrders.end(), [](SaleOrder& saleOrder) { return saleOrder.getTotalQuantityOfOrder() == 0; }), (*offersBook)[symbol].saleOrders.end());

            // Update bid & ask prices
            if ((*offersBook)[symbol].purchaseOrders.size() > 0) {
                (*offersBook)[symbol].bid = (*offersBook)[symbol].purchaseOrders[0].getOrderPrice();
                traderAccount->onTick(symbol, (*offersBook)[symbol].bid, "BID", offersBook);
            }else {
                (*offersBook)[symbol].bid = 0;
            }

            if ((*offersBook)[symbol].saleOrders.size() > 0) {
                (*offersBook)[symbol].ask = (*offersBook)[symbol].saleOrders[0].getOrderPrice();
                traderAccount->onTick(symbol, (*offersBook)[symbol].ask, "ASK", offersBook);
            }else {
                (*offersBook)[symbol].ask = 0;
            }
        }else {
            // Add purchase order to symbol queue
            arrayUtils.insertPurchaseOrder((*offersBook)[symbol].purchaseOrders, purchaseOrderBuffer);
            (*offersBook)[symbol].bid = purchaseOrderBuffer.getOrderPrice();
            traderAccount->onTick(symbol, (*offersBook)[symbol].bid, "BID", offersBook);
        }

    }else if (order.getOrderSide() == SALE_ORDER) {
        SaleOrder saleOrderBuffer;
        saleOrderBuffer = *(new SaleOrder(order.getSequentialOrderNumber(), order.getSecondaryOrderID(), order.getPriorityTime(), order.getOrderPrice(), order.getTotalQuantityOfOrder(), order.getTradedQuantityOfOrder()));
        
        if (saleOrderBuffer.getOrderPrice() >= 0 && saleOrderBuffer.getOrderPrice() <= (*offersBook)[symbol].bid) {

			PurchaseOrder currPurchaseOrder;
			int tradedQty;
			int i = 0;
            do {
				currPurchaseOrder = (*offersBook)[symbol].purchaseOrders[i];

                if (saleOrderBuffer.getTotalQuantityOfOrder() >= currPurchaseOrder.getTotalQuantityOfOrder()) {
					tradedQty = currPurchaseOrder.getTotalQuantityOfOrder();
					saleOrderBuffer.setTotalQuantityOfOrder(saleOrderBuffer.getTotalQuantityOfOrder() - tradedQty);
					saleOrderBuffer.setTradedQuantityOfOrder(saleOrderBuffer.getTradedQuantityOfOrder() + tradedQty);
					currPurchaseOrder.setTotalQuantityOfOrder(0);
				}else {
					tradedQty = saleOrderBuffer.getTotalQuantityOfOrder();
					saleOrderBuffer.setTotalQuantityOfOrder(0);
					saleOrderBuffer.setTradedQuantityOfOrder(saleOrderBuffer.getTradedQuantityOfOrder() + tradedQty);
					currPurchaseOrder.setTotalQuantityOfOrder(currPurchaseOrder.getTotalQuantityOfOrder() - tradedQty);
				}

                (*offersBook)[symbol].purchaseOrders[i] = currPurchaseOrder;
                (*offersBook)[symbol].totalTradedQuantity += tradedQty;
                (*offersBook)[symbol].historicalPrices.push_back(saleOrderBuffer.getOrderPrice());

				i++;
			} while (saleOrderBuffer.getTotalQuantityOfOrder() > 0 && currPurchaseOrder.getOrderPrice() >= saleOrderBuffer.getOrderPrice());

			// Update sale orders
            if (saleOrderBuffer.getTotalQuantityOfOrder() > 0) {
				(*offersBook)[symbol].saleOrders.insert((*offersBook)[symbol].saleOrders.begin(), saleOrderBuffer);
			}
			// Update purchase orders (erase all orders where totalQuantityOfOrder = 0)
            (*offersBook)[symbol].purchaseOrders.erase(remove_if((*offersBook)[symbol].purchaseOrders.begin(), (*offersBook)[symbol].purchaseOrders.end(), [](PurchaseOrder& purchaseOrder) { return purchaseOrder.getTotalQuantityOfOrder() == 0; }), (*offersBook)[symbol].purchaseOrders.end());

            // Update bid & ask prices
            if ((*offersBook)[symbol].purchaseOrders.size() > 0) {
                (*offersBook)[symbol].bid = (*offersBook)[symbol].purchaseOrders[0].getOrderPrice();
                traderAccount->onTick(symbol, (*offersBook)[symbol].bid, "BID", offersBook);
            }else {
                (*offersBook)[symbol].bid = 0;
            }

            if ((*offersBook)[symbol].saleOrders.size() > 0) {
                (*offersBook)[symbol].ask = (*offersBook)[symbol].saleOrders[0].getOrderPrice();
                traderAccount->onTick(symbol, (*offersBook)[symbol].ask, "ASK", offersBook);
            }else {
                (*offersBook)[symbol].ask = 0;
            }
        }else {
            arrayUtils.insertSaleOrder((*offersBook)[symbol].saleOrders, saleOrderBuffer);
            (*offersBook)[symbol].ask = saleOrderBuffer.getOrderPrice();
            traderAccount->onTick(symbol, (*offersBook)[symbol].ask, "ASK", offersBook);
        }
    }
}

// Class methods

OrderService::OrderService(vector<string> _targetStocks) {
    targetStocks = _targetStocks;
}

void OrderService::startProcessOrders(vector<string>* rawOrdersQueue, map<string, StockInfo>* offersBook, Semaphore* semaphore, Trader* traderAccount)
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
            orderBuffer = parseOrder(currOrder, stringUtils);
            processNewOrder(symbol, orderBuffer, offersBook, arrayUtils, traderAccount);
        }else {
            symbol = "";
        }
        
        semaphore->release();
        this_thread::sleep_for(timespan);

    }
}