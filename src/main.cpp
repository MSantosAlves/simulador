#include "Clock.h"
#include "Context.h"
#include "DataService.h"
#include "OrderService.h"
#include "LogService.h"
#include "MetricsSystem.h"
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
#include <queue>

#define PORT 8080

int main(int argc, char *argv[])
{
    high_resolution_clock::time_point startPoint = high_resolution_clock::now();
    
    // Instantiate application semaphore
    Semaphore *semaphore = new Semaphore();
    semaphore->release();

    // Instantiate shared variables
    queue<string> rawOrdersQueue;
    map<string, StockInfo> offersBook;
    Clock *clock = new Clock();
    Context *ctx = new Context();
    ctx->setStartTimePoint(startPoint);

    // Get custom configs
    Config *config = new Config();
    string dataTargetDate = config->getDate();
    string dataPath = config->getDataPath();
    string simulationSpeed = config->getSimulationSpeed();
    string targetStock = config->getTargetStock();
    StockDataInfo targetStockDataInfo = config->getTargetStockDataInfo();
    ctx->setTargetStock(targetStock);
    ctx->setTotalOrdersSize(targetStockDataInfo.nbOfLines);

    // Instantiate services
    DataService *dataService = new DataService(dataTargetDate, dataPath, targetStockDataInfo, targetStock, simulationSpeed, clock, ctx);
    OrderService *orderService = new OrderService(clock, ctx);
    LogService *logService = new LogService(clock, ctx);
    MetricsSystem *metricsSystem = new MetricsSystem(clock, ctx, &offersBook, &rawOrdersQueue);

    // Setup socket
    Server *server = new Server(PORT);
    ServerResponseSender *responseSender = new ServerResponseSender(server->getClientSocketAddress());

    thread readOrdersThread(&DataService::startAcquisition, dataService, &rawOrdersQueue, semaphore, "BOTH");
    thread ordersProcessorThread(&OrderService::startProcessOrders, orderService, &rawOrdersQueue, &offersBook, semaphore, responseSender, server);
    thread sendMarketDataOnTickThread(&LogService::sendMarkedDataOnTick, logService, &offersBook, semaphore, responseSender);
    thread marketServerThread(&Server::acceptConnections, server, &rawOrdersQueue, semaphore, clock);
    thread metricsSystemThread(&MetricsSystem::startMetricsSystem, metricsSystem, semaphore);

    marketServerThread.join();
    readOrdersThread.join();
    ordersProcessorThread.join();
    sendMarketDataOnTickThread.join();
    metricsSystemThread.join();

    return 0;
}