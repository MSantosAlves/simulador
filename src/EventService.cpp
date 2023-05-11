#include "EventService.h"
#include "Event.h"
#include "StringUtils.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <atomic>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <map>

using namespace std;

// Aux methods
bool isTargetSymbol(vector<string> targetsStocks, string stock) {
    StringUtils stringUtils;
    return stringUtils.include(targetsStocks, stock) != -1;
}

vector<string> removeWithSpacesFromSplitedString(vector<string> splitedString)
{
    StringUtils stringUtils;
    for (int i = 0; i < splitedString.size(); i++) {
        splitedString[i] = stringUtils.removeWhiteSpaces(splitedString[i]);
    }
    return splitedString;
}

Event processEvent(string event)
{
    StringUtils stringUtils;
    vector<string> splitedString;
    char const* delimiter = ";";
    int stringSize;
    string type;
    splitedString = removeWithSpacesFromSplitedString(stringUtils.split(event, (*delimiter)));

    stringSize = splitedString.size();
    Event* eventBuffer = new Event();

    // stringSize <= 2 => Data header
    if (stringSize > 2) {
        type = splitedString[stringSize - 1];
        if (type == "VDA" || type == "CPA") {
            eventBuffer->setSessionDate(splitedString[0]);
            eventBuffer->setInstrumentSymbol(splitedString[1]);
            eventBuffer->setOrderSide(splitedString[2]);
            eventBuffer->setSequentialOrderNumber(splitedString[3]);
            eventBuffer->setSecondaryOrderID(splitedString[4]);
            eventBuffer->setExecutionType(splitedString[5]);
            eventBuffer->setPriorityTime(splitedString[6]);
            eventBuffer->setPriorityIndicator(splitedString[7]);
            eventBuffer->setOrderPrice(splitedString[8]);
            eventBuffer->setTotalQuantityOfOrder(splitedString[9]);
            eventBuffer->setTradedQuantityOfOrder(splitedString[10]);
            eventBuffer->setOrderDate(splitedString[11]);
            eventBuffer->setOrderDatetimeEntry(splitedString[12]);
            eventBuffer->setOrderStatus(splitedString[13]);
            eventBuffer->setAggressorIndicator(splitedString[14]);
            eventBuffer->setMember(splitedString[15]);
            eventBuffer->setType(type);
            eventBuffer->setArrayOfCalculatedFields(splitedString);
            return (*eventBuffer);
        }
        else if (type == "NEG") {
            eventBuffer->setSessionDate(splitedString[1]);
            eventBuffer->setInstrumentSymbol(splitedString[2]);
            eventBuffer->setIradeNumber(splitedString[3]);
            eventBuffer->setIradePrice(splitedString[4]);
            eventBuffer->setIradedQuantity(splitedString[5]);
            eventBuffer->setIradeTime(splitedString[6]);
            eventBuffer->setIradeIndicator(splitedString[7]);
            eventBuffer->setBuyOrderDate(splitedString[8]);
            eventBuffer->setSequentialBuyOrderNumber(splitedString[9]);
            eventBuffer->setSecondaryOrderIDBuyOrder(splitedString[10]);
            eventBuffer->setAggressorBuyOrderIndicator(splitedString[11]);
            eventBuffer->setSellOrderDate(splitedString[12]);
            eventBuffer->setSequentialSellOrderNumber(splitedString[13]);
            eventBuffer->setSecondaryOrderIDSellOrder(splitedString[14]);
            eventBuffer->setAggressorSellOrderIndicator(splitedString[15]);
            eventBuffer->setCrossTradeIndicator(splitedString[16]);
            eventBuffer->setBuyMember(splitedString[17]);
            eventBuffer->setSellMember(splitedString[18]);
            eventBuffer->setType(type);
            eventBuffer->setArrayOfCalculatedFields(splitedString);
            return (*eventBuffer);
        }
    }

    return (*eventBuffer);
}


// Class methods

EventService::EventService(vector<string> _targetStocks) {
    targetStocks = _targetStocks;
}

void EventService::startProcessEvents(vector<string>* eventsToBeProcessed, vector<string>* offerBook, Semaphore* semaphore, map<string, vector<PurchaseOffer>>* purchasesOffers, map<string, vector<SaleOffer>>* salesOffers)
{
    Event eventBuffer;
    chrono::milliseconds timespan(1000);
    string symbol;
    string currEvent;
    PurchaseOffer purchaseOfferBuffer;
    SaleOffer saleOfferBuffer;

    while (true) {
        semaphore->acquire();

        if (eventsToBeProcessed->size() == 0) {
			semaphore->release();
			continue;
		}
        currEvent = eventsToBeProcessed->front();
        eventBuffer = processEvent(currEvent);
        eventsToBeProcessed->erase(eventsToBeProcessed->begin());
        symbol = eventBuffer.getInstrumentSymbol();
        
        if (symbol != "" && isTargetSymbol(targetStocks, symbol)) {
            
            //TODO: Order each symbol offers by price and priority time
            if (eventBuffer.getOrderSide() == "1") {
                purchaseOfferBuffer = *(new PurchaseOffer(eventBuffer.getSequentialOrderNumber(), eventBuffer.getSecondaryOrderID(), eventBuffer.getPriorityTime(), eventBuffer.getOrderPrice(), eventBuffer.getTotalQuantityOfOrder(), eventBuffer.getTradedQuantityOfOrder()));
                (*purchasesOffers)[symbol].push_back(purchaseOfferBuffer);
            }
            else if (eventBuffer.getOrderSide() == "2") {
                saleOfferBuffer = *(new SaleOffer(eventBuffer.getSequentialOrderNumber(), eventBuffer.getSecondaryOrderID(), eventBuffer.getPriorityTime(), eventBuffer.getOrderPrice(), eventBuffer.getTotalQuantityOfOrder(), eventBuffer.getTradedQuantityOfOrder()));
                (*salesOffers)[symbol].push_back(saleOfferBuffer);
            }
        }

        semaphore->release();
        
    }
}