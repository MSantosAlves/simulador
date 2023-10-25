#include "Clock.h"
#include "DataService.h"
#include "OrderService.h"
#include "LogService.h"
#include "Semaphore.h"
#include "Config.h"
#include "StockInfo.h"
#include "Trader.h"
#include "StockDataInfo.h"
#include "Server.h"
#include "ServerResponseSender.h"

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
    Clock* clock = new Clock();

    // Get custom configs
    Config *config = new Config();
    string dataTargetDate = config->getDate();
    string dataPath = config->getDataPath();
    string simulationSpeed = config->getSimulationSpeed();
    vector<string> targetStocks = config->getTargetStocks();
    map<string, StockDataInfo> targetStocksDataInfo = config->getTargetStocksDataInfo();
   
    // Instantiate services
    DataService *dataService = new DataService(dataTargetDate, dataPath, targetStocksDataInfo, targetStocks, simulationSpeed, clock);
    OrderService *orderService = new OrderService(targetStocks, clock);
    LogService *logService = new LogService();

    // Setup socket
    Server *server = new Server(PORT);
    ServerResponseSender *responseSender = new ServerResponseSender(server->getClientSocketAddress());

    thread readOrdersThread(&DataService::startAcquisition, dataService, &rawOrdersQueue, semaphore, "BOTH");
    thread ordersProcessorThread(&OrderService::startProcessOrders, orderService, &rawOrdersQueue, &offersBook, semaphore, responseSender);
    thread sendDataOnTickThread(&LogService::sendDataOnTick, logService, &offersBook, semaphore, responseSender);
    thread marketServerThread(&Server::acceptConnections, server, &rawOrdersQueue, semaphore);

    marketServerThread.join();
    readOrdersThread.join();
    ordersProcessorThread.join();
    sendDataOnTickThread.join();

    return 0;
}