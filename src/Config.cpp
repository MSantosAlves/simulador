#include "Config.h"
#include "StringUtils.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

Config::Config()
{
    StringUtils stringUtils;
    filesystem::path pwd = filesystem::current_path().parent_path();

    string pwdString = stringUtils.pathToString(pwd);
    string fullPath = pwdString +"/config.json";

    ifstream jsonFile(fullPath);
    json jsonData = json::parse(jsonFile);

    string date = jsonData["date"];
    string dataPath = jsonData["dataPath"];

    string fullDataPath =  pwdString + dataPath;
    string indexStocksPath = pwdString + "/data/b3/" + date + "/compiled/INDEX_SORTED_STOCK_FILES.json";
    ifstream indexStocksFile(indexStocksPath);
    json indexStocksJson = json::parse(indexStocksFile);

    string targetStock = jsonData["targetStock"];
    string simulationSpeed = jsonData["simulationSpeed"];

    StockDataInfo targetStockDataInfo = {};
    targetStockDataInfo.filename = indexStocksJson[targetStock]["filename"];
    targetStockDataInfo.nbOfLines = indexStocksJson[targetStock]["number_of_lines"];

    setDate(date);
    setDataPath(fullDataPath);
    setTargetStock(targetStock);
    setTargetStockDataInfo(targetStockDataInfo);
    setSimulationSpeed(simulationSpeed);
}

void Config::setDate(string _date)
{
    date = _date;
}

string Config::getDate()
{
    return date;
}

void Config::setDataPath(string _dataPath)
{
    dataPath = _dataPath;
}

string Config::getDataPath()
{
    return dataPath;
}

void Config::setSimulationSpeed(string _simulationSpeed)
{
    simulationSpeed = _simulationSpeed;
}

string Config::getSimulationSpeed()
{
    return simulationSpeed;
}

void Config::setTargetStock(string _targetStock)
{
    targetStock = _targetStock;
}

string Config::getTargetStock()
{
    return targetStock;
}

void Config::setTargetStockDataInfo(StockDataInfo _targetStockDataInfo)
{
    targetStockDataInfo = _targetStockDataInfo;
}

StockDataInfo Config::getTargetStockDataInfo()
{
    return targetStockDataInfo;
}
