#include <vector>
#include <map>
#include <string>

#include "FileDataStockInfo.h"

#ifndef STOCK_DATA_INFO_H
#define STOCK_DATA_INFO_H

using namespace std;

struct StockDataInfo
{
    vector<string> vdaFiles;
    vector<string> cpaFiles;
    map<string, FileDataStockInfo> vdaFilesInfo;
    map <string, FileDataStockInfo> cpaFilesInfo;
};

#endif