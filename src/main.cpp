#include "Clock.h"
#include "Context.h"
#include "DataService.h"
#include "OrderService.h"
#include "LogService.h"
#include "Semaphore.h"
#include "Config.h"
#include "StockInfo.h"
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

    // Instantiate shared variables
    vector<string> rawOrdersQueue;
    map<string, StockInfo> offersBook;
    Clock *clock = new Clock();
    Context *ctx = new Context();

    // Get custom configs
    Config *config = new Config();
    string dataTargetDate = config->getDate();
    string dataPath = config->getDataPath();
    string simulationSpeed = config->getSimulationSpeed();
    string targetStock = config->getTargetStock();
    StockDataInfo targetStockDataInfo = config->getTargetStockDataInfo();
    ctx->setTargetStock(targetStock);

    // Instantiate services
    DataService *dataService = new DataService(dataTargetDate, dataPath, targetStockDataInfo, targetStock, simulationSpeed, clock, ctx);
    OrderService *orderService = new OrderService(clock);
    LogService *logService = new LogService(clock, ctx);

    // Setup socket
    Server *server = new Server(PORT);
    ServerResponseSender *responseSender = new ServerResponseSender(server->getClientSocketAddress());

    thread readOrdersThread(&DataService::startAcquisition, dataService, &rawOrdersQueue, semaphore, "BOTH");
    thread ordersProcessorThread(&OrderService::startProcessOrders, orderService, &rawOrdersQueue, &offersBook, semaphore, responseSender);
    // thread sendDataOnTickThread(&LogService::sendDataOnTick, logService, &offersBook, semaphore, responseSender);
    thread printContextThread(&LogService::printContextOnTick, logService, &offersBook, &rawOrdersQueue);
    // thread marketServerThread(&Server::acceptConnections, server, &rawOrdersQueue, semaphore);

    // marketServerThread.join();
    readOrdersThread.join();
    ordersProcessorThread.join();
    // sendDataOnTickThread.join();
    printContextThread.join();

    return 0;
}