#include "Config.h"
#include "StringUtils.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

Config::Config() {
    filesystem::path pwd = filesystem::current_path();
    StringUtils stringUtils;
    string sysFileChar = (_WIN64 || _WIN32) ? "\\" : "/";
    string fullPath = stringUtils.pathToString(pwd) + sysFileChar + "config.json";
   
    ifstream jsonFile(fullPath);
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
