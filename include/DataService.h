#include <string>
#include <vector>
#include <map>

#include "Semaphore.h"
#include "StockDataInfo.h"

using namespace std;

#ifndef __DATA_SERVICE_H__
#define __DATA_SERVICE_H__

class DataService
{
private:    
    string date;
    string dataPath;
    map<string, StockDataInfo> targetStocksDataInfo;
    vector<string> targetStocks;
    string simulationSpeed;
public:
    DataService(string date, string dataPath, map<string, StockDataInfo> targetStocksDataInfo, vector<string> targetStocks, string simulationSpeed);
    void startAcquisition(vector<string>* rawOrdersQueue, Semaphore* semaphore, string offerType);
};

#endif