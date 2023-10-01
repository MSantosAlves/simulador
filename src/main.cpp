#include "DataService.h"
#include "OrderService.h"
#include "LogService.h"
#include "Semaphore.h"
#include "Config.h"
#include "StockInfo.h"
#include "Trader.h"
#include "StockDataInfo.h"
#include "Server.h"
#include "ClientResponseSender.h"

#include <thread>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <atomic>
#include <string>
#include <vector>
#include <map>

#define PORT 8080

int main(int argc, char *argv[])
{
    // Instantiate application semaphore
    Semaphore *semaphore = new Semaphore();
    semaphore->release();

    // Trader class (strategy)
    Trader *traderAccount = new Trader(5000.00);

    // Instantiate shared variables
    vector<string> rawOrdersQueue;
    map<string, StockInfo> offersBook;

    // Get custom configs
    Config *config = new Config();
    string dataTargetDate = config->getDate();
    string dataPath = config->getDataPath();
    vector<string> targetStocks = config->getTargetStocks();
    map<string, StockDataInfo> targetStocksDataInfo = config->getTargetStocksDataInfo();

    // Instantiate services
    DataService *dataService = new DataService(dataTargetDate, dataPath, targetStocksDataInfo, targetStocks);
    OrderService *orderService = new OrderService(targetStocks);
    LogService *logService = new LogService();

    // Setup socket
    Server *server = new Server(PORT);
    ClientResponseSender *responseSender = new ClientResponseSender(server->getClientSocketAddress());

    thread readPurchasesThread(&DataService::startAcquisition, dataService, &rawOrdersQueue, semaphore, "PURCHASES");
    thread readSalesThread(&DataService::startAcquisition, dataService, &rawOrdersQueue, semaphore, "SALES");
    thread ordersProcessorThread(&OrderService::startProcessOrders, orderService, &rawOrdersQueue, &offersBook, semaphore, responseSender);
    thread logSystemThread(&LogService::startLogSystem, logService, &offersBook, semaphore);
    thread marketServerThread(&Server::acceptConnections, server, &rawOrdersQueue, semaphore);

    readPurchasesThread.join();
    readSalesThread.join();
    ordersProcessorThread.join();
    logSystemThread.join();
    marketServerThread.join();

    return 0;
}