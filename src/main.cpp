#include "DataService.h"
#include "OrderService.h"
#include "LogService.h"
#include "Semaphore.h"
#include "Config.h"

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
    vector<string> ordersToBeProcessed;
    vector<string> offersBook;
    vector<Order> processedOrders;
    map<string, vector<PurchaseOrder>> purchasesOrders;
    map<string, vector<SaleOrder>> salesOrders;

    // Get custom configs
    Config* config = new Config();
    string date = config->getDate();
    string dataPath = config->getDataPath();
    vector<string> stocks = config->getTargetStocks();

    // Instantiate services
    DataService* dataService = new DataService(date, dataPath);
    OrderService* orderService = new OrderService(stocks);
    LogService* logService = new LogService();
    
    thread dataAcquisitionThread(&DataService::startAcquisition, dataService, &ordersToBeProcessed, semaphore);
    thread ordersProcessorThread(&OrderService::startProcessOrders, orderService, &ordersToBeProcessed, &offersBook, semaphore, &purchasesOrders, &salesOrders);
    thread logSystemThread(&LogService::startLogSystem, logService, &ordersToBeProcessed, &offersBook, semaphore, &processedOrders);

    dataAcquisitionThread.join();
    ordersProcessorThread.join();
    //logSystemThread.join();

    return 0;
}