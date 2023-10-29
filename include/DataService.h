#include <string>
#include <vector>
#include <map>
#include<queue>

#include "Semaphore.h"
#include "Clock.h"
#include "Context.h"
#include "StockDataInfo.h"

using namespace std;

#ifndef __DATA_SERVICE_H__
#define __DATA_SERVICE_H__

class DataService
{
private:
    string date;
    string dataPath;
    StockDataInfo targetStockDataInfo;
    string targetStock;
    string simulationSpeed;
    Clock *clock;
    Context *context;

public:
    DataService(string date, string dataPath, StockDataInfo targetStockDataInfo, string targetStock, string simulationSpeed, Clock *clock, Context *context);
    void startAcquisition(queue<string> *rawOrdersQueue, Semaphore *semaphore, string offerType);
};

#endif