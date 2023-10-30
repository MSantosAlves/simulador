#include "MetricsSystem.h"

#include <filesystem>
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <fstream>
#include "unistd.h"

using namespace std;

MetricsSystem::MetricsSystem(Clock *_clock, Context *_context, map<string, StockInfo> *_offersBook, queue<string> *_rawOrdersQueue)
{
    clock = _clock;
    context = _context;
    offersBook = _offersBook;
    rawOrdersQueue = _rawOrdersQueue;
}

void MetricsSystem::startMetricsSystem(Semaphore *semaphore)
{
    int lastOrdersRead, currOrdersRead, throughput, purchases, sales, ordersQueueSize, bookSize, queueBookRatio = 0;
    int tickSeconds = 10;
    chrono::milliseconds tick(tickSeconds * 1000);

    string targetStock = context->getTargetStock();
    string timeString = clock->getRealTimeHumanReadable();

    filesystem::path basePath = filesystem::current_path().parent_path();
    string metricsFilename = targetStock + "_" + timeString + ".csv";
    string metricsFilePath = string(basePath) + "/metrics/" + metricsFilename;
    string fileHeader = "Symbol,BookSize,QueueSize,Throughput,Time\n";
    string lineBuffer = "";

    ofstream metricsFile(metricsFilePath);
     metricsFile.flush();

    if (!metricsFile.is_open()) {
        cerr << "Failed to create metrics file." << std::endl;
        return;
    }

    metricsFile << fileHeader;

    sleep(5);

    while (context->simulationShouldContinue())
    {
        purchases = (*offersBook)[targetStock].purchaseOrders.size();
        sales = (*offersBook)[targetStock].saleOrders.size();
        currOrdersRead = context->getOrdersRead();
        lastOrdersRead = context->getLastOrdersReadValue();
        ordersQueueSize = rawOrdersQueue->size();
        bookSize = purchases + sales;
        throughput = floor((currOrdersRead - lastOrdersRead) / tickSeconds);

        cout << endl;
        cout << "Orders processed: " << context->getSimulationExecutedHumandReadable() << "% " << "(" << context->getOrdersRead() << " orders)" << endl;
        cout << "Book Size: " << bookSize << endl;
        cout << "Throughput: " << throughput << endl;

        lineBuffer = targetStock + "," + to_string(bookSize) + "," + to_string(ordersQueueSize) + "," + to_string(throughput) + "," + clock->getRealTimeHumanReadable() + "\n";

        metricsFile << lineBuffer;
        metricsFile.flush();

        context->setLastOrdersReadValue(currOrdersRead);

        this_thread::sleep_for(tick);
        
    }

    metricsFile.close();

    return;
}