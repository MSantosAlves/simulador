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

using namespace std;

DataService::DataService(string _date, string _dataPath, map<string, StockDataInfo> _targetStocksDataInfo, vector<string> _targetStocks, string _simulationSpeed, Clock *_clock)
{
    date = _date;
    dataPath = _dataPath;
    targetStocks = _targetStocks;
    targetStocksDataInfo = _targetStocksDataInfo;
    simulationSpeed = _simulationSpeed;
    clock = _clock;
}

void DataService::startAcquisition(vector<string> *rawOrdersQueue, Semaphore *semaphore, string orderType)
{
    string filePath = dataPath + "/" + date + "/raw/";
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
    int fileCurrentLine = 0, dataOffset = 0;
    string currentStock = "", currentFile = "", currentFilePath = "", orderBuffer = "";
    FileDataStockInfo fileInfo;
    vector<string> filesToRead = {};
    int isFirstOrder = true;
    string lastPriorityTime;

    if (orderType == "BOTH")
    {
        filePath += targetStocks[0] + "_SORTED.txt";

        ifstream dataFile(filePath);

        while (getline(dataFile, orderBuffer))
        {

            vector<string> splitedLine = stringUtils.split(orderBuffer, ';');
            string priorityTime = splitedLine[6];
            string orderSide = splitedLine[2];
            string orderSufix = orderSide == "1" ? ";CPA" : ";VDA";

            semaphore->acquire();

            if (isFirstOrder)
            {
                clock->setSimulationTime(priorityTime);
                lastPriorityTime = priorityTime;
                cout << "Reading file: " << filePath << endl;
                cout << "Initial Simulation Time: " << clock->getSimulationTimeHumanReadable() << endl;
                isFirstOrder = false;
            }

            rawOrdersQueue->push_back(orderBuffer + orderSufix);

            semaphore->release();

            orderBuffer = "";

            int64_t timeBetweenOrdersInMs = clock->getTimeBetweenOrdersInMicroseconds(lastPriorityTime, priorityTime);
            int64_t sleepTimeInMs = simulationSpeed != "NORMAL" ? floor(timeBetweenOrdersInMs / timespanDownfactor) : timeBetweenOrdersInMs;

            if (sleepTimeInMs > 0)
            {
                this_thread::sleep_for(chrono::milliseconds(sleepTimeInMs));
            }

            clock->setSimulationTime(priorityTime);

            lastPriorityTime = priorityTime;

            // File end was reached
            if (!dataFile)
            {
                cout << filePath << " end reached.";
                dataFile.close();
                break;
            }
        }

        cout << filePath << " end reached.";
        dataFile.close();
        return;
    }

    return;
}