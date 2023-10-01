
#include "LogService.h"
#include "OrderFieldsEnum.h"
#include "StockInfo.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <atomic>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>

using namespace std;

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

LogService::LogService() {}

void LogService::startLogSystem(map<string, StockInfo> *offersBook, Semaphore *semaphore)
{
    chrono::milliseconds timespan(3000);
    while (true)
    {
        semaphore->acquire();
        printOffersBook(offersBook);
        semaphore->release();
        this_thread::sleep_for(timespan);
    }

    return;
}