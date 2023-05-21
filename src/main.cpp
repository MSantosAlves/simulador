#include "DataService.h"
#include "OrderService.h"
#include "LogService.h"
#include "Semaphore.h"
#include "Config.h"
#include "StockInfo.h"

#include <thread>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <atomic>
#include <string>
#include <vector>
#include <map>


int main(int argc, char* argv[])
{
    // Instantiate application semaphore
    Semaphore* semaphore = new Semaphore();
    semaphore->release();

    // Instantiate shared variables
    vector<string> rawOrdersQueue;
    map<string, StockInfo> offersBook;

    // Get custom configs
    Config* config = new Config();
    string dataTargetDate = config->getDate();
    string dataPath = config->getDataPath();
    vector<string> targetStocks = config->getTargetStocks();

    // Instantiate services
    DataService* dataService = new DataService(dataTargetDate, dataPath);
    OrderService* orderService = new OrderService(targetStocks);
    LogService* logService = new LogService();

    thread readSalesThread(&DataService::startAcquisition, dataService, &rawOrdersQueue, semaphore, "SALES");
    thread readPurchasesThread(&DataService::startAcquisition, dataService, &rawOrdersQueue, semaphore, "PURCHASES");
    thread ordersProcessorThread(&OrderService::startProcessOrders, orderService, &rawOrdersQueue, &offersBook, semaphore);
    thread logSystemThread(&LogService::startLogSystem, logService, &offersBook, semaphore);

    readSalesThread.join();
    readPurchasesThread.join();
    ordersProcessorThread.join();
    logSystemThread.join();

    return 0;
}