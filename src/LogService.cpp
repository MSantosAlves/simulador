
#include "LogService.h"
#include "OrderFieldsEnum.h"
#include "StockInfo.h"
#include "StockMarketVolume.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <atomic>
#include <string>
#include <vector>
#include <deque>
#include <chrono>
#include <thread>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

void printTable(const vector<string> &headers, const vector<vector<string>> &data)
{
    if (false)
    { //_WIN64 || _WIN32) {
        system("cls");
    }
    else
    {
        system("clear");
    }

    // Find the maximum string length for each column
    vector<size_t> columnWidths(headers.size());
    for (size_t i = 0; i < headers.size(); i++)
    {
        columnWidths[i] = headers[i].length();
    }
    for (const auto &row : data)
    {
        for (size_t i = 0; i < row.size(); i++)
        {
            columnWidths[i] = max(columnWidths[i], row[i].length());
        }
    }

    cout << "* ";

    for (size_t i = 0; i < headers.size(); i++)
    {
        for (size_t j = 0; j < columnWidths[i]; j++)
        {
            cout << "-";
        }
        cout << " * ";
    }
    cout << "\n";

    cout << "| ";

    // Print the headers
    for (size_t i = 0; i < headers.size(); i++)
    {
        cout << headers[i];
        for (size_t j = 0; j < columnWidths[i] - headers[i].length(); j++)
        {
            cout << " ";
        }
        cout << " | ";
    }
    cout << "\n";

    cout << "* ";

    // Print the separator
    for (size_t i = 0; i < headers.size(); i++)
    {
        for (size_t j = 0; j < columnWidths[i]; j++)
        {
            cout << "-";
        }
        cout << " * ";
    }
    cout << "\n";

    // Print the data
    for (const auto &row : data)
    {
        cout << "| ";
        for (size_t i = 0; i < row.size(); i++)
        {
            cout << row[i];
            for (size_t j = 0; j < columnWidths[i] - row[i].length(); j++)
            {
                cout << " ";
            }
            cout << " | ";
        }
        cout << "\n";
    }

    cout << "* ";

    for (size_t i = 0; i < headers.size(); i++)
    {
        for (size_t j = 0; j < columnWidths[i]; j++)
        {
            cout << "-";
        }
        cout << " * ";
    }
    cout << "\n";
}

// void printOrders(vector<Order>* processedOrders)
//{
//     OrderFields* orderFields = new OrderFields();
//     vector<string> headers = { "Symbol", "Price", "Total Qty.", "Member", "Type" };
//     vector<vector<string>> data;
//     vector<int> desiredFields = {
//         OrderFields::Enum::INSTRUMENT_SYMBOL,
//         OrderFields::Enum::ORDER_PRICE,
//         OrderFields::Enum::TOTAL_QUANTITY_OF_ORDER,
//         OrderFields::Enum::MEMBER,
//         OrderFields::Enum::TYPE_ORDER
//     };
//
//     for (int i = 0; i < processedOrders->size(); i++) {
//         data.push_back(processedOrders->at(i).getArrayOfCalculatedFields(false, desiredFields));
//     }
//
//     if (data.size() > 0) {
//         printTable(headers, data);
//     }
//
// }

void printOffersBook(map<string, StockInfo> *offersBook)
{
    OrderFields *orderFields = new OrderFields();
    vector<string> headers = {"Symbol", "Bid Price", "Ask Price", "Total orders", "Total traded"};
    vector<string> lineBuffer;
    vector<vector<string>> data;

    for (auto it = offersBook->begin(); it != offersBook->end(); it++)
    {
        lineBuffer.push_back(it->first);
        lineBuffer.push_back(to_string(it->second.bid));
        lineBuffer.push_back(to_string(it->second.ask));
        lineBuffer.push_back(to_string(it->second.purchaseOrders.size() + it->second.saleOrders.size()));
        lineBuffer.push_back(to_string(it->second.totalTradedQuantity));
        data.push_back(lineBuffer);
        lineBuffer.clear();
    }

    if (data.size() > 0)
    {
        printTable(headers, data);
    }
}

LogService::LogService(Clock *_clock, Context *_context)
{
    clock = _clock;
    context = _context;
}

void LogService::startLogSystem(map<string, StockInfo> *offersBook, Semaphore *semaphore)
{
    chrono::milliseconds timespan(3000);
    while (context->simulationShouldContinue())
    {
        semaphore->acquire();
        printOffersBook(offersBook);
        semaphore->release();
        this_thread::sleep_for(timespan);
    }

    return;
}

map<string, vector<StockMarketVolume>> getMarketVolume(map<string, StockInfo> *offersBook, Context * context)
{

    vector<string> symbols;
    map<string, vector<StockMarketVolume>> marketVolume = {};

    if (offersBook->empty() || context->getSimulationExecutedHumandReadable() == "100")
    {
        return marketVolume;
    }

    for (const auto &pair : *offersBook)
    {
        symbols.push_back(pair.first);
    }

    string symbol = symbols[0];

    if (symbols.size() == 0)
    {
        return marketVolume;
    }

    deque<PurchaseOrder> purchaseOrders = (*offersBook)[symbol].purchaseOrders;
    deque<SaleOrder> saleOrders = (*offersBook)[symbol].saleOrders;

    if (purchaseOrders.size() == 0 || saleOrders.size() == 0)
    {
        return marketVolume;
    }

    StockMarketVolume stockMarketVolume;
    int lastIdx = 0;

    for (int i = purchaseOrders.size() - 1; i >= 0; i--)
    {

        if (i == purchaseOrders.size() - 1)
        {
            stockMarketVolume.price = purchaseOrders[i].getOrderPrice();
            stockMarketVolume.quantity = purchaseOrders[i].getTotalQuantityOfOrder();
            stockMarketVolume.direction = "BUY";
            marketVolume[symbol].push_back(stockMarketVolume);
        }
        else
        {
            lastIdx = marketVolume[symbol].size() - 1;
            if (marketVolume[symbol][lastIdx].price == purchaseOrders[i].getOrderPrice())
            {
                marketVolume[symbol][lastIdx].quantity += purchaseOrders[i].getTotalQuantityOfOrder();
            }
            else
            {
                stockMarketVolume.price = purchaseOrders[i].getOrderPrice();
                stockMarketVolume.quantity = purchaseOrders[i].getTotalQuantityOfOrder();
                stockMarketVolume.direction = "BUY";
                marketVolume[symbol].push_back(stockMarketVolume);
            }
        }
    }

    for (int i = 0; i < saleOrders.size(); i++)
    {

        if (i == 0)
        {
            stockMarketVolume.price = saleOrders[i].getOrderPrice();
            stockMarketVolume.quantity = saleOrders[i].getTotalQuantityOfOrder();
            stockMarketVolume.direction = "SALE";
            marketVolume[symbol].push_back(stockMarketVolume);
        }
        else
        {
            lastIdx = marketVolume[symbol].size() - 1;
            if (marketVolume[symbol][lastIdx].price == saleOrders[i].getOrderPrice())
            {
                marketVolume[symbol][lastIdx].quantity += saleOrders[i].getTotalQuantityOfOrder();
            }
            else
            {
                stockMarketVolume.price = saleOrders[i].getOrderPrice();
                stockMarketVolume.quantity = saleOrders[i].getTotalQuantityOfOrder();
                stockMarketVolume.direction = "SALE";
                marketVolume[symbol].push_back(stockMarketVolume);
            }
        }
    }

    return marketVolume;
}

void LogService::sendMarkedDataOnTick(map<string, StockInfo> *offersBook, Semaphore *semaphore, ServerResponseSender *responseSender)
{
    chrono::milliseconds tick(1000);
    map<string, vector<StockMarketVolume>> marketVolume;
    json jsonObject, volumeJson;

    while (context->simulationShouldContinue())
    {
        semaphore->acquire();

        marketVolume = getMarketVolume(offersBook, context);

        // Construct the JSON object manually
        jsonObject["event"] = "UPDATE_MARKET_VOLUME";
        jsonObject["market_volume"] = json::object();

        for (const auto &entry : marketVolume)
        {
            jsonObject["market_volume"][entry.first] = json::array();
            for (const auto &stockVolume : entry.second)
            {
                volumeJson["quantity"] = stockVolume.quantity;
                volumeJson["price"] = stockVolume.price;
                volumeJson["direction"] = stockVolume.direction;
                jsonObject["market_volume"][entry.first].push_back(volumeJson);
                jsonObject["time"] = clock->getSimulationTimeHumanReadable();
            }
        }

        responseSender->sendResponse(jsonObject);

        semaphore->release();
        this_thread::sleep_for(tick);
    }

    return;
}