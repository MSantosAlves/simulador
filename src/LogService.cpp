
#include "LogService.h"
#include "EventFieldsEnum.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <atomic>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

void printTable(const vector<string>& headers, const vector<vector<string>>& data) {
    if (_WIN64 || _WIN32) {
        system("cls");
    }else {
        system("clear");
    }
    
    // Find the maximum string length for each column
    vector<size_t> columnWidths(headers.size());
    for (size_t i = 0; i < headers.size(); i++) {
        columnWidths[i] = headers[i].length();
    }
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); i++) {
            columnWidths[i] = max(columnWidths[i], row[i].length());
        }
    }

    cout << "* ";

    for (size_t i = 0; i < headers.size(); i++) {
        for (size_t j = 0; j < columnWidths[i]; j++) {
            cout << "-";
        }
        cout << " * ";
    }
    cout << "\n";

    cout << "| ";

    // Print the headers
    for (size_t i = 0; i < headers.size(); i++) {
        cout << headers[i];
        for (size_t j = 0; j < columnWidths[i] - headers[i].length(); j++) {
            cout << " ";
        }
        cout << " | ";
    }
    cout << "\n";

    cout << "* ";

    // Print the separator
    for (size_t i = 0; i < headers.size(); i++) {
        for (size_t j = 0; j < columnWidths[i]; j++) {
            cout << "-";
        }
        cout << " * ";
    }
    cout << "\n";

    // Print the data
    for (const auto& row : data) {
        cout << "| ";
        for (size_t i = 0; i < row.size(); i++) {
            cout << row[i];
            for (size_t j = 0; j < columnWidths[i] - row[i].length(); j++) {
                cout << " ";
            }
            cout << " | ";
        }
        cout << "\n";
    }

    cout << "* ";

    for (size_t i = 0; i < headers.size(); i++) {
        for (size_t j = 0; j < columnWidths[i]; j++) {
            cout << "-";
        }
        cout << " * ";
    }
    cout << "\n";
}

void printEvents(vector<Event>* processedEvents)
{
    EventFields* eventFields = new EventFields();
    vector<string> headers = { "Symbol", "Price", "Total Qty.", "Member", "Type" };
    vector<vector<string>> data;
    vector<int> desiredFields = {
        EventFields::Enum::INSTRUMENT_SYMBOL,
        EventFields::Enum::ORDER_PRICE,
        EventFields::Enum::TOTAL_QUANTITY_OF_ORDER,
        EventFields::Enum::MEMBER,
        EventFields::Enum::TYPE_ORDER
    };

    for (int i = 0; i < processedEvents->size(); i++) {
        data.push_back(processedEvents->at(i).getArrayOfCalculatedFields(false, desiredFields));
    }

    if (data.size() > 0) {
        printTable(headers, data);
    }

}


LogService::LogService() {}

void LogService::startLogSystem(vector<string>* events, vector<string>* book, Semaphore* semaphore, vector<Event>* processedEvents)
{
    chrono::milliseconds timespan(3000);
    while (true) {
        semaphore->acquire();
        printEvents(processedEvents);
        semaphore->release();
        this_thread::sleep_for(timespan);
    }
}