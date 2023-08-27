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

DataService::DataService(string _date, string _dataPath) {
    date = _date;
    dataPath = _dataPath;
}

void DataService::startAcquisition(vector<string>* rawOrdersQueue, Semaphore* semaphore, string orderType)
{
    string sysFileChar = (_WIN64 || _WIN32) ? "\\" : "/";
    string filePath = dataPath + sysFileChar + date + sysFileChar;

    const int nbOfChunkOffers = 1000;
    const int nbOfOfferBytes = 230;

    const size_t chunkSize = nbOfOfferBytes * nbOfChunkOffers;
    char chunkBuffer[chunkSize];
    string orderBuffer;
    string orderSufix = ";VDA";
    chrono::nanoseconds timespan(1);

    if (orderType == "SALES") {
        filePath += "VDA" + sysFileChar + "OFER_VDA_BMF_20191220_00.txt";
    }
    else if (orderType == "PURCHASES") {
        filePath += "CPA" + sysFileChar + "OFER_CPA_BMF_20191220_00.txt";
        orderSufix = ";CPA";
    }

    ifstream dataFile(filePath);

    while (dataFile)
    {
        dataFile.read(chunkBuffer, chunkSize);
        
        for (int i = 0;  i < nbOfChunkOffers; i++) {

            orderBuffer = string(chunkBuffer).substr(i * nbOfOfferBytes, nbOfOfferBytes) + orderSufix;
            semaphore->acquire();
            rawOrdersQueue->push_back(orderBuffer);
            semaphore->release();

            this_thread::sleep_for(timespan);
        }

    }

    dataFile.close();


}