#include "OrderUtils.h"

using namespace std;

OrderUtils::OrderUtils() {}

Order OrderUtils::parseOrder(string order, StringUtils stringUtils)
{
    vector<string> splitedString;
    char const* delimiter = ";";
    int stringSize;
    string type;
    splitedString = stringUtils.removeWitheSpacesFromSplitedString(stringUtils.split(order, (*delimiter)));

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

void OrderUtils::orderMatching(string symbol, Order order, map<string, StockInfo>* offersBook, ArrayUtils arrayUtils, Trader* traderAccount) {
    const string PURCHASE_ORDER = "1";
    const string SALE_ORDER = "2";

    // BID: highest price a buyer will pay to buy a stock
    // ASK: lowest price a seller watns to sell a stock

    if (order.getOrderSide() == PURCHASE_ORDER) {
        PurchaseOrder purchaseOrderBuffer;
        purchaseOrderBuffer = *(new PurchaseOrder(order.getSequentialOrderNumber(), order.getSecondaryOrderID(), order.getPriorityTime(), order.getOrderPrice(), order.getTotalQuantityOfOrder(), order.getTradedQuantityOfOrder()));

        // New purchase order has a price higher than stock current ASK price = New negotiation
        if ((*offersBook)[symbol].ask > 0 && purchaseOrderBuffer.getOrderPrice() >= 0 && purchaseOrderBuffer.getOrderPrice() >= (*offersBook)[symbol].ask) {

            SaleOrder currSaleOrder, nextSaleOrder;
            int tradedQty;
            
            // Get the matching sale offer, i.e. the first sales offer in the offersBook (*sales order are sorted from lowest to highest price)
            currSaleOrder = (*offersBook)[symbol].saleOrders[0];
            
            do {
                // If purchase order quantity is higher than the sale order quantity, buy all stocks. 
                if (purchaseOrderBuffer.getTotalQuantityOfOrder() >= currSaleOrder.getTotalQuantityOfOrder()) {
                    tradedQty = currSaleOrder.getTotalQuantityOfOrder();
                    purchaseOrderBuffer.setTotalQuantityOfOrder(purchaseOrderBuffer.getTotalQuantityOfOrder() - tradedQty);
                    purchaseOrderBuffer.setTradedQuantityOfOrder(purchaseOrderBuffer.getTradedQuantityOfOrder() + tradedQty);
                    currSaleOrder.setTotalQuantityOfOrder(0);
                    currSaleOrder.setTradedQuantityOfOrder(currSaleOrder.getTradedQuantityOfOrder() + tradedQty);
                }
                else {
                    tradedQty = purchaseOrderBuffer.getTotalQuantityOfOrder();
                    purchaseOrderBuffer.setTotalQuantityOfOrder(0);
                    purchaseOrderBuffer.setTradedQuantityOfOrder(purchaseOrderBuffer.getTradedQuantityOfOrder() + tradedQty);
                    currSaleOrder.setTotalQuantityOfOrder(currSaleOrder.getTotalQuantityOfOrder() - tradedQty);
                    currSaleOrder.setTradedQuantityOfOrder(currSaleOrder.getTradedQuantityOfOrder() + tradedQty);
                }

                (*offersBook)[symbol].totalTradedQuantity += tradedQty;
                (*offersBook)[symbol].historicalPrices.push_back(currSaleOrder.getOrderPrice());

                //Purchase order still have liquidity to be traded and there are other sale offers to be considered
                if (purchaseOrderBuffer.getTotalQuantityOfOrder() > 0 && (*offersBook)[symbol].saleOrders.size() > 1) {
                    // Update currSaleOrder reference
                    nextSaleOrder = (*offersBook)[symbol].saleOrders[1];
                }
                else {
                    // Avoid use the last reference from the loop
                    nextSaleOrder = currSaleOrder;
                }

                //Update OffersBook (erase or update sales that were completely processed)
                if (currSaleOrder.getTotalQuantityOfOrder() == 0) {
                    (*offersBook)[symbol].saleOrders.erase((*offersBook)[symbol].saleOrders.begin());
                }
                else {
                    (*offersBook)[symbol].saleOrders[0] = currSaleOrder;
                }

                currSaleOrder = nextSaleOrder;

            } while (purchaseOrderBuffer.getTotalQuantityOfOrder() > 0 && currSaleOrder.getTotalQuantityOfOrder() > 0 && currSaleOrder.getOrderPrice() <= purchaseOrderBuffer.getOrderPrice());
           
            if(purchaseOrderBuffer.getTotalQuantityOfOrder() > 0) {
                //Add purchase to OffersBook
                (*offersBook)[symbol].purchaseOrders.insert((*offersBook)[symbol].purchaseOrders.begin(), purchaseOrderBuffer);
            }
           
            // Update bid & ask prices
            if ((*offersBook)[symbol].purchaseOrders.size() > 0) {
                (*offersBook)[symbol].bid = (*offersBook)[symbol].purchaseOrders[0].getOrderPrice();
                //traderAccount->onTick(symbol, (*offersBook)[symbol].bid, "BID", offersBook);
            }
            else {
                (*offersBook)[symbol].bid = 0;
            }

            if ((*offersBook)[symbol].saleOrders.size() > 0) {
                (*offersBook)[symbol].ask = (*offersBook)[symbol].saleOrders[0].getOrderPrice();
                //traderAccount->onTick(symbol, (*offersBook)[symbol].ask, "ASK", offersBook);
            }
            else {
                (*offersBook)[symbol].ask = 0;
            }
        }
        else {
            // Add purchase order to symbol queue
            arrayUtils.insertPurchaseOrder((*offersBook)[symbol].purchaseOrders, purchaseOrderBuffer);
            (*offersBook)[symbol].bid = (*offersBook)[symbol].purchaseOrders[0].getOrderPrice();
            //traderAccount->onTick(symbol, (*offersBook)[symbol].bid, "BID", offersBook);
        }

    }
    else if (order.getOrderSide() == SALE_ORDER) {
        SaleOrder saleOrderBuffer;
        saleOrderBuffer = *(new SaleOrder(order.getSequentialOrderNumber(), order.getSecondaryOrderID(), order.getPriorityTime(), order.getOrderPrice(), order.getTotalQuantityOfOrder(), order.getTradedQuantityOfOrder()));

        if ((*offersBook)[symbol].bid > 0 && saleOrderBuffer.getOrderPrice() >= 0 && saleOrderBuffer.getOrderPrice() <= (*offersBook)[symbol].bid) {

            PurchaseOrder currPurchaseOrder, nextPurchaseOrder;
            int tradedQty;
            // Get the matching purchase offer, i.e. the first purchase offer in the offersBook (*purchases order are sorted from highest to lowest price)
            currPurchaseOrder = (*offersBook)[symbol].purchaseOrders[0];

            do {
                // If sale order quantity is higher than the purchase order quantity, sell all stocks. 
                if (saleOrderBuffer.getTotalQuantityOfOrder() >= currPurchaseOrder.getTotalQuantityOfOrder()) {
                    tradedQty = currPurchaseOrder.getTotalQuantityOfOrder();
                    saleOrderBuffer.setTotalQuantityOfOrder(saleOrderBuffer.getTotalQuantityOfOrder() - tradedQty);
                    saleOrderBuffer.setTradedQuantityOfOrder(saleOrderBuffer.getTradedQuantityOfOrder() + tradedQty);
                    currPurchaseOrder.setTotalQuantityOfOrder(0);
                    currPurchaseOrder.setTradedQuantityOfOrder(currPurchaseOrder.getTradedQuantityOfOrder() + tradedQty);
                }
                else {
                    tradedQty = saleOrderBuffer.getTotalQuantityOfOrder();
                    saleOrderBuffer.setTotalQuantityOfOrder(0);
                    saleOrderBuffer.setTradedQuantityOfOrder(saleOrderBuffer.getTradedQuantityOfOrder() + tradedQty);
                    currPurchaseOrder.setTotalQuantityOfOrder(currPurchaseOrder.getTotalQuantityOfOrder() - tradedQty);
                    currPurchaseOrder.setTradedQuantityOfOrder(currPurchaseOrder.getTradedQuantityOfOrder() + tradedQty);
                }

               
                (*offersBook)[symbol].totalTradedQuantity += tradedQty;
                (*offersBook)[symbol].historicalPrices.push_back(saleOrderBuffer.getOrderPrice());

                //Sale order still have liquidity to be traded and there are other purchase offers to be considered
                if(saleOrderBuffer.getTotalQuantityOfOrder() > 0 && (*offersBook)[symbol].purchaseOrders.size() > 1){
                    // Update currPurchaseOrder reference
                    nextPurchaseOrder = (*offersBook)[symbol].purchaseOrders[1];
                }
                else {
                    nextPurchaseOrder = currPurchaseOrder;
                }

                //Update OffersBook (erase or update purchases that were completely processed)
                if(currPurchaseOrder.getTotalQuantityOfOrder() == 0){
                     (*offersBook)[symbol].purchaseOrders.erase((*offersBook)[symbol].purchaseOrders.begin());
                }
                else {
                    (*offersBook)[symbol].purchaseOrders[0] = currPurchaseOrder;
                }

                currPurchaseOrder = nextPurchaseOrder;

            } while (saleOrderBuffer.getTotalQuantityOfOrder() > 0 && currPurchaseOrder.getTotalQuantityOfOrder() > 0 && currPurchaseOrder.getOrderPrice() >= saleOrderBuffer.getOrderPrice());

            if (saleOrderBuffer.getTotalQuantityOfOrder() > 0) {
                //Add sale to OffersBook
                (*offersBook)[symbol].saleOrders.insert((*offersBook)[symbol].saleOrders.begin(), saleOrderBuffer);
            }

            // Update bid & ask prices
            if ((*offersBook)[symbol].purchaseOrders.size() > 0) {
                (*offersBook)[symbol].bid = (*offersBook)[symbol].purchaseOrders[0].getOrderPrice();
                //traderAccount->onTick(symbol, (*offersBook)[symbol].bid, "BID", offersBook);
            }
            else {
                (*offersBook)[symbol].bid = 0;
            }

            if ((*offersBook)[symbol].saleOrders.size() > 0) {
                (*offersBook)[symbol].ask = (*offersBook)[symbol].saleOrders[0].getOrderPrice();
                //traderAccount->onTick(symbol, (*offersBook)[symbol].ask, "ASK", offersBook);
            }
            else {
                (*offersBook)[symbol].ask = 0;
            }
        }
        else {
            arrayUtils.insertSaleOrder((*offersBook)[symbol].saleOrders, saleOrderBuffer);
            (*offersBook)[symbol].ask = (*offersBook)[symbol].saleOrders[0].getOrderPrice();
            //traderAccount->onTick(symbol, (*offersBook)[symbol].ask, "ASK", offersBook);
        }
    }
}
