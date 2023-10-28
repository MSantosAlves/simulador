#include <vector>
#include <map>
#include <string>

#include "FileDataStockInfo.h"

#ifndef STOCK_DATA_INFO_H
#define STOCK_DATA_INFO_H

using namespace std;

struct StockDataInfo
{
    string filename;
    int nbOfLines;
};

#endif