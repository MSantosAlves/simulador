#include "OrderUtils.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace std;

OrderUtils::OrderUtils(Clock *_clock)
{
    clock = _clock;
}

Order OrderUtils::parseOrder(string order, StringUtils stringUtils, Order *orderBuffer)
{
    vector<string> splitedString;
    char const *delimiter = ";";
    int stringSize;
    string type;
    splitedString = stringUtils.removeWhiteSpacesFromSplitedString(stringUtils.split(order, (*delimiter)));

    stringSize = splitedString.size();
    // Order *orderBuffer = new Order();

    // stringSize <= 2 => Data header
    if (stringSize > 2)
    {
        type = splitedString[stringSize - 1];
        if (type == "VDA" || type == "CPA" || type == "CLIENT")
        {
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
        }
        else if (type == "NEG")
        {
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
        }

        int orderSource = type == "CLIENT" ? 1 : 0;
        orderBuffer->setOrderSource(orderSource);
        return (*orderBuffer);
    }

    return (*orderBuffer);
}

void sendUpdateBookEvent(string symbol, map<string, StockInfo> *offersBook, ServerResponseSender *responseSender, string direction, string time)
{
    json jsonObject = {
        {"event", "UPDATE_BOOK"},
        {"symbol", symbol},
        {"direction", direction},
        {"time", time},
        {"traded_volume", (*offersBook)[symbol].totalTradedQuantity},
        {"buy_offers", (*offersBook)[symbol].purchaseOrders.size()},
        {"sell_offers", (*offersBook)[symbol].saleOrders.size()},
        {"bid", (*offersBook)[symbol].bid},
        {"ask", (*offersBook)[symbol].ask},
        {"last_trade_price", (*offersBook)[symbol].lastTradePrice}};
    responseSender->sendResponse(jsonObject);
}

void OrderUtils::executePossibleTrades(string symbol, map<string, StockInfo> *offersBook, int bookUpdateDirection, ServerResponseSender *responseSender)
{
    PurchaseOrder currPurchaseOrder;
    SaleOrder currSaleOrder;
    int tradedQty = 0;

    // BID changed, but there is no sale order to be processed OR ASK changed, but there is no purchase order to be processed.
    if ((bookUpdateDirection == 1 && (*offersBook)[symbol].saleOrders.size() == 0) || (bookUpdateDirection == 2 && (*offersBook)[symbol].purchaseOrders.size() == 0))
    {
        return;
    }

    currSaleOrder = (*offersBook)[symbol].saleOrders[0];
    currPurchaseOrder = (*offersBook)[symbol].purchaseOrders[0];

    // Set negotiations loop stop condition
    bool bidIsGreaterThanOrEqToAsk = currPurchaseOrder.getOrderPrice() >= currSaleOrder.getOrderPrice();

    while (bidIsGreaterThanOrEqToAsk)
    {
        /**
         * Three possible scenarios for the orders beeing traded:
         *
         * I)   Both orders have the same totalQuantity available for trade
         *
         *  1 - Execute trade
         *  2 - Remove both orders from book
         *  3 - Update currSale and currPurchase references
         *  4 - Update BID and ASK price
         *
         * II)  PurchaseTotalQuantity is greater than SaleTotalQuantity
         *
         *  1 - Execute trade
         *  2 - Remove sale order from book
         *  3 - Update currPurchase reference
         *  4 - Update ASK price
         *
         * III) SaleTotalQuantity is greater than PurchaseTotalQuantity
         *
         *  1 - Execute trade
         *  2 - Remove purchase order from book
         *  3 - Update currSale reference
         *  4 - Update BID price
         */

        bool bothOffersCanBeEntirelyFilled = currPurchaseOrder.getTotalQuantityOfOrder() == currSaleOrder.getTotalQuantityOfOrder();
        bool currSaleCanBeEntirelyFilled = currPurchaseOrder.getTotalQuantityOfOrder() > currSaleOrder.getTotalQuantityOfOrder();

        double tradePrice = 0.0;

        // TODO: Review price model
        if ((*offersBook)[symbol].lastTradePrice != 0)
        {
            tradePrice = (currPurchaseOrder.getOrderPrice() + currSaleOrder.getOrderPrice() + (*offersBook)[symbol].lastTradePrice) / 3.0;
        }
        else
        {
            tradePrice = (currPurchaseOrder.getOrderPrice() + currSaleOrder.getOrderPrice()) / 2.0;
        }

        if (bothOffersCanBeEntirelyFilled)
        {

            // Execute the trade
            tradedQty = currSaleOrder.getTotalQuantityOfOrder();
            currPurchaseOrder.setTotalQuantityOfOrder(0);
            currSaleOrder.setTotalQuantityOfOrder(0);
            currPurchaseOrder.setTradedQuantityOfOrder(currPurchaseOrder.getTradedQuantityOfOrder() + tradedQty);
            currSaleOrder.setTradedQuantityOfOrder(currSaleOrder.getTradedQuantityOfOrder() + tradedQty);

            (*offersBook)[symbol].totalTradedQuantity += tradedQty;
            (*offersBook)[symbol].lastTradePrice = tradePrice;

            // Remove both orders from book, given that both were entirely filled
            (*offersBook)[symbol].saleOrders.erase((*offersBook)[symbol].saleOrders.begin());
            (*offersBook)[symbol].purchaseOrders.erase((*offersBook)[symbol].purchaseOrders.begin());

            if (currSaleOrder.getOrderSource() == 1)
            {
                json jsonObject = {
                    {"event", "SALE_OFFER_ENTIRELY_FILLED"},
                    {"qty", tradedQty},
                    {"id", currSaleOrder.getSequentialOrderNumber()},
                    {"price", currSaleOrder.getOrderPrice()},
                    {"symbol", symbol}};
                responseSender->sendResponse(jsonObject);
            }
            else if (currPurchaseOrder.getOrderSource() == 1)
            {
                json jsonObject = {
                    {"event", "PURCHASE_OFFER_ENTIRELY_FILLED"},
                    {"qty", tradedQty},
                    {"id", currPurchaseOrder.getSequentialOrderNumber()},
                    {"price", currSaleOrder.getOrderPrice()},
                    {"symbol", symbol}};
                responseSender->sendResponse(jsonObject);
            }

            // Update ASK value and currSale reference
            if ((*offersBook)[symbol].saleOrders.size() > 0)
            {
                currSaleOrder = (*offersBook)[symbol].saleOrders[0];
                if ((*offersBook)[symbol].ask != currSaleOrder.getOrderPrice())
                {
                    (*offersBook)[symbol].ask = currSaleOrder.getOrderPrice();
                    if ((*offersBook)[symbol].ask > (*offersBook)[symbol].bid)
                    {
                        sendUpdateBookEvent(symbol, offersBook, responseSender, "ASK", clock->getSimulationTimeHumanReadable());
                    }
                }
            }

            // Update BID value and currPurchase reference
            if ((*offersBook)[symbol].purchaseOrders.size() > 0)
            {
                currPurchaseOrder = (*offersBook)[symbol].purchaseOrders[0];
                if ((*offersBook)[symbol].bid != currPurchaseOrder.getOrderPrice())
                {
                    (*offersBook)[symbol].bid = currPurchaseOrder.getOrderPrice();
                    if ((*offersBook)[symbol].bid < (*offersBook)[symbol].ask)
                    {
                        sendUpdateBookEvent(symbol, offersBook, responseSender, "BID", clock->getSimulationTimeHumanReadable());
                    }
                }
            }

            // Break loop all sales or all offers were already processed
            if ((*offersBook)[symbol].saleOrders.size() == 0 || (*offersBook)[symbol].purchaseOrders.size() == 0)
            {
                break;
            }
        }
        else if (currSaleCanBeEntirelyFilled)
        {
            // Execute the trade
            tradedQty = currSaleOrder.getTotalQuantityOfOrder();
            currPurchaseOrder.setTotalQuantityOfOrder(currPurchaseOrder.getTotalQuantityOfOrder() - tradedQty);
            currPurchaseOrder.setTradedQuantityOfOrder(currPurchaseOrder.getTradedQuantityOfOrder() + tradedQty);
            currSaleOrder.setTotalQuantityOfOrder(0);
            currSaleOrder.setTradedQuantityOfOrder(currSaleOrder.getTradedQuantityOfOrder() + tradedQty);

            (*offersBook)[symbol].totalTradedQuantity += tradedQty;
            (*offersBook)[symbol].lastTradePrice = tradePrice;

            if (currSaleOrder.getOrderSource() == 1)
            {
                json jsonObject = {
                    {"event", "SALE_OFFER_ENTIRELY_FILLED"},
                    {"qty", tradedQty},
                    {"id", currSaleOrder.getSequentialOrderNumber()},
                    {"price", currSaleOrder.getOrderPrice()},
                    {"symbol", symbol}};
                responseSender->sendResponse(jsonObject);
            }
            else if (currPurchaseOrder.getOrderSource() == 1)
            {
                json jsonObject = {
                    {"event", "PURCHASE_OFFER_PARTIALLY_FILLED"},
                    {"qty", tradedQty},
                    {"id", currPurchaseOrder.getSequentialOrderNumber()},
                    {"price", currSaleOrder.getOrderPrice()},
                    {"symbol", symbol}};
                responseSender->sendResponse(jsonObject);
            }

            // Update currPurchase value given that it was not entirely filled yet
            (*offersBook)[symbol].purchaseOrders[0] = currPurchaseOrder;

            // Remove sale order from book, given that it was entirely filled
            (*offersBook)[symbol].saleOrders.erase((*offersBook)[symbol].saleOrders.begin());

            // Update ASK value and currSale reference OR break loop if all sales where already processed
            if ((*offersBook)[symbol].saleOrders.size() > 0)
            {
                currSaleOrder = (*offersBook)[symbol].saleOrders[0];
                if ((*offersBook)[symbol].ask != currSaleOrder.getOrderPrice())
                {
                    (*offersBook)[symbol].ask = currSaleOrder.getOrderPrice();
                    if ((*offersBook)[symbol].ask > (*offersBook)[symbol].bid)
                    {
                        sendUpdateBookEvent(symbol, offersBook, responseSender, "ASK", clock->getSimulationTimeHumanReadable());
                    }
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            // currPurchaseCanBeEntirelyFilled = true

            // Execute the trade
            tradedQty = currPurchaseOrder.getTotalQuantityOfOrder();
            currPurchaseOrder.setTotalQuantityOfOrder(0);
            currPurchaseOrder.setTradedQuantityOfOrder(currPurchaseOrder.getTradedQuantityOfOrder() + tradedQty);
            currSaleOrder.setTotalQuantityOfOrder(currSaleOrder.getTotalQuantityOfOrder() - tradedQty);
            currSaleOrder.setTradedQuantityOfOrder(currSaleOrder.getTradedQuantityOfOrder() + tradedQty);

            (*offersBook)[symbol].totalTradedQuantity += tradedQty;
            (*offersBook)[symbol].lastTradePrice = tradePrice;

            if (currSaleOrder.getOrderSource() == 1)
            {
                json jsonObject = {
                    {"event", "SALE_OFFER_PARTIALLY_FILLED"},
                    {"qty", tradedQty},
                    {"id", currSaleOrder.getSequentialOrderNumber()},
                    {"price", currSaleOrder.getOrderPrice()},
                    {"symbol", symbol}};
                responseSender->sendResponse(jsonObject);
            }
            else if (currPurchaseOrder.getOrderSource() == 1)
            {
                json jsonObject = {
                    {"event", "PURCHASE_OFFER_ENTIRELY_FILLED"},
                    {"qty", tradedQty},
                    {"id", currPurchaseOrder.getSequentialOrderNumber()},
                    {"price", currSaleOrder.getOrderPrice()},
                    {"symbol", symbol}};
                responseSender->sendResponse(jsonObject);
            }

            // Update currSale value given that it was not entirely filled yet
            (*offersBook)[symbol].saleOrders[0] = currSaleOrder;

            // Remove purchase order from book, given that it was entirely filled
            (*offersBook)[symbol].purchaseOrders.erase((*offersBook)[symbol].purchaseOrders.begin());

            // Update BID value and currPurchase reference OR break loop if all sales where already processed
            if ((*offersBook)[symbol].purchaseOrders.size() > 0)
            {
                currPurchaseOrder = (*offersBook)[symbol].purchaseOrders[0];
                if ((*offersBook)[symbol].bid != currPurchaseOrder.getOrderPrice())
                {
                    (*offersBook)[symbol].bid = currPurchaseOrder.getOrderPrice();
                    if ((*offersBook)[symbol].bid < (*offersBook)[symbol].ask)
                    {
                        sendUpdateBookEvent(symbol, offersBook, responseSender, "BID", clock->getSimulationTimeHumanReadable());
                    }
                }
            }
            else
            {
                break;
            }
        }

        bidIsGreaterThanOrEqToAsk = currPurchaseOrder.getOrderPrice() >= currSaleOrder.getOrderPrice();
    }

    return;
}