#include "DataService.h"
#include "Semaphore.h"
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

using namespace std;

DataService::DataService(string _date, string _dataPath, map<string, StockDataInfo> _targetStocksDataInfo, vector<string> _targetStocks, string _simulationSpeed)
{
    date = _date;
    dataPath = _dataPath;
    targetStocks = _targetStocks;
    targetStocksDataInfo = _targetStocksDataInfo;
    simulationSpeed = _simulationSpeed;
}

void DataService::startAcquisition(vector<string> *rawOrdersQueue, Semaphore *semaphore, string orderType)
{
    string filePath = dataPath + "/" + date +  "/raw/";
    int timespanInNs = 100000000; // Default = 100ms

    if (simulationSpeed == "FAST") {
        timespanInNs = 1000000;
    }
    else if (simulationSpeed == "NORMAL") {
        timespanInNs = 100000000; // 100ms
    }else if(simulationSpeed == "SLOW"){
        timespanInNs = 1000000000; // 1s
    }

    chrono::nanoseconds timespan(timespanInNs);

    const int nbOfOfferBytes = 230;
    int fileCurrentLine = 0, dataOffset = 0;
    string currentStock = "", currentFile = "", currentFilePath = "", orderBuffer = "";
    FileDataStockInfo fileInfo;
    vector<string> filesToRead = {};

    filePath += orderType == "SALES" ? "VDA" : "CPA";
    string orderSufix = orderType == "SALES" ? ";VDA" : ";CPA";

    for (int i = 0; i < targetStocks.size(); i++)
    {
        currentStock = targetStocks[i];

        filesToRead = orderType == "SALES" ? targetStocksDataInfo[currentStock].vdaFiles : targetStocksDataInfo[currentStock].cpaFiles;

        // For each stock, loop through files that contains stock orders
        for (int j = 0; j < filesToRead.size(); j++)
        {
            // First and Last lines where the current stock orders appears in the current file
            fileInfo = orderType == "SALES" ? targetStocksDataInfo[currentStock].vdaFilesInfo[filesToRead[j]] : targetStocksDataInfo[currentStock].cpaFilesInfo[filesToRead[j]];

            currentFile = filesToRead[j];
            currentFilePath = filePath + "/" + currentFile;
            ifstream dataFile(currentFilePath);

            // Where stock orders starts
            dataOffset = (fileInfo.firstLineIndex - 1) * nbOfOfferBytes;
            dataFile.ignore(dataOffset);
            fileCurrentLine = fileInfo.firstLineIndex;

            // Loop until file ends or get the last line for the current stock in the current file
            while (getline(dataFile, orderBuffer) && fileCurrentLine <= fileInfo.lastLineIndex)
            {

                semaphore->acquire();
                rawOrdersQueue->push_back(orderBuffer + orderSufix);
                semaphore->release();

                orderBuffer = "";

                this_thread::sleep_for(timespan);
                fileCurrentLine++;

                // File end was reached
                if (!dataFile)
                {
                    dataFile.close();
                    break;
                }

                // All offers from currentStock in this file where processed and file end was not reached yet
                if (fileCurrentLine > fileInfo.lastLineIndex)
                {
                    dataFile.close();
                }
            }
        }
    }

    return;
}