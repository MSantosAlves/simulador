#include "DataService.h"
#include "Semaphore.h"
#include "Clock.h"
#include "Config.h"
#include "StringUtils.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <atomic>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <cmath>
#include <queue>

using namespace std;

DataService::DataService(string _date, string _dataPath, StockDataInfo _targetStockDataInfo, string _targetStock, string _simulationSpeed, Clock *_clock, Context *_context)
{
    date = _date;
    dataPath = _dataPath;
    targetStock = _targetStock;
    targetStockDataInfo = _targetStockDataInfo;
    simulationSpeed = _simulationSpeed;
    clock = _clock;
    context = _context;
}

void DataService::startAcquisition(queue<string> *rawOrdersQueue, Semaphore *semaphore, string orderType)
{
    string filePath = dataPath + "/" + date + "/sorted/";
    StringUtils stringUtils;
    int extraOffset = 0;
    float timespanDownfactor = 1;

    if (simulationSpeed == "FAST")
    {
        timespanDownfactor = 100;
    }
    else if (simulationSpeed == "SLOW")
    {
        timespanDownfactor = 0.1;
    }

    const int nbOfOfferBytes = 230;
    bool isFirstOrder = true;
    string orderBuffer = "";
    string priorityTime, lastPriorityTime, orderSide, orderSufix;
    vector<string> splitedLine = {};
    int64_t timeBetweenOrdersInMs;
    int64_t sleepTimeInMs;
    int lineCounter = 0;
    int fileEndLine = context->getTotalOrdersSize();

    if (orderType == "BOTH")
    {
        filePath += targetStock + "_SORTED.txt";

        ifstream dataFile(filePath);

        while (getline(dataFile, orderBuffer))
        {

            splitedLine = stringUtils.split(orderBuffer, ';');
            priorityTime = splitedLine[6];
            orderSide = splitedLine[2];
            orderSufix = orderSide == "1" ? ";CPA" : ";VDA";

            semaphore->acquire();

            if (isFirstOrder)
            {
                clock->setSimulationTime(priorityTime);
                lastPriorityTime = priorityTime;
                cout << "Simulation started at: " << clock->getRealTimeHumanReadable() << endl;
                cout << "Ficticious Start Time: " << clock->getSimulationTimeHumanReadable() << endl;
                cout << "Reading file: " << filePath << endl;
                cout << "File length: " << fileEndLine << endl;
                isFirstOrder = false;
            }

            rawOrdersQueue->push(orderBuffer + orderSufix);

            orderBuffer = "";

            timeBetweenOrdersInMs = clock->getTimeBetweenOrdersInMicroseconds(lastPriorityTime, priorityTime);
            sleepTimeInMs = simulationSpeed != "NORMAL" ? floor(timeBetweenOrdersInMs / timespanDownfactor) : timeBetweenOrdersInMs;

            if (sleepTimeInMs > 0)
            {
                this_thread::sleep_for(chrono::milliseconds(sleepTimeInMs));
            }

            clock->setSimulationTime(priorityTime);

            semaphore->release();

            lastPriorityTime = priorityTime;

            lineCounter++;

            // File end was reached
            if (!dataFile || lineCounter == fileEndLine - 1)
            {
                cout << "File " << filePath << " fully consumed." << endl;
                dataFile.close();
                break;
            }

        }

        return;
    }

    return;
}