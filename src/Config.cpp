#include <iostream>
#include <fstream>

#include "Config.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

Config::Config() {
    ifstream jsonFile("C:\\Users\\Matheus Alves\\Documents\\UnB\\TG\\code\\simulador\\config.json");
    json jsonData = json::parse(jsonFile);

    string date = jsonData["date"];
    string dataPath = jsonData["dataPath"];
    vector<string> stocks = jsonData["targetStocks"];
    int size = stocks.size();
    vector<string> targetStocks = {};

    for (int i = 0; i < size; ++i) {
        targetStocks.push_back(stocks[i]);
    }

    setDate(date);
    setDataPath(dataPath);
    setTargetStocks(targetStocks);

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

void Config::setTargetStocks(vector<string> _targetStocks)
{
    targetStocks = _targetStocks;
}

vector<string> Config::getTargetStocks()
{
    return targetStocks;
}
