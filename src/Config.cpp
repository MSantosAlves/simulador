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
    filesystem::path pwd = filesystem::current_path().parent_path();
    StringUtils stringUtils;
    string pwdString = stringUtils.pathToString(pwd);
    string fullPath = pwdString +"/config.json";

    ifstream jsonFile(fullPath);
    json jsonData = json::parse(jsonFile);

    string date = jsonData["date"];

    string indexStocksPath = pwdString + "/data/b3/compiled/INDEX_STOCK_FILES.json";
    ifstream indexStocksFile(indexStocksPath);
    json indexStocksJson = json::parse(indexStocksFile);

    vector<string> stocks = jsonData["targetStocks"];
    string simulationSpeed = jsonData["simulationSpeed"];

    int size = stocks.size();
    vector<string> targetStocks = {};

    vector<string> cpaFiles = {};
    vector<string> vdaFiles = {};

    vector<string> auxVector = {};

    string filename = "";

    for (int i = 0; i < stocks.size(); i++)
    {
        targetStocks.push_back(stocks[i]);

        targetStocksDataInfo[stocks[i]] = {};
        targetStocksDataInfo[stocks[i]].cpaFiles = indexStocksJson[stocks[i]]["buy_files"];
        targetStocksDataInfo[stocks[i]].vdaFiles = indexStocksJson[stocks[i]]["sell_files"];

        auto auxJson = indexStocksJson[stocks[i]]["buy_files_info"];
        for (auto it = auxJson.begin(); it != auxJson.end(); ++it)
        {
            filename = it.key();
            auto fileInfoValue = it.value();
            targetStocksDataInfo[stocks[i]].cpaFilesInfo[filename] = {};
            targetStocksDataInfo[stocks[i]].cpaFilesInfo[filename].firstLineIndex = fileInfoValue["first_line_index"];
            targetStocksDataInfo[stocks[i]].cpaFilesInfo[filename].lastLineIndex = fileInfoValue["last_line_index"];
        }

        auxJson = indexStocksJson[stocks[i]]["sell_files_info"];
        for (auto it = auxJson.begin(); it != auxJson.end(); ++it)
        {
            filename = it.key();
            auto fileInfoValue = it.value();
            targetStocksDataInfo[stocks[i]].vdaFilesInfo[filename] = {};
            targetStocksDataInfo[stocks[i]].vdaFilesInfo[filename].firstLineIndex = fileInfoValue["first_line_index"];
            targetStocksDataInfo[stocks[i]].vdaFilesInfo[filename].lastLineIndex = fileInfoValue["last_line_index"];
        }
    }

    setDate(date);
    setDataPath(dataPath);
    setTargetStocks(targetStocks);
    setTargetStocksDataInfo(targetStocksDataInfo);
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

void Config::setTargetStocks(vector<string> _targetStocks)
{
    targetStocks = _targetStocks;
}

vector<string> Config::getTargetStocks()
{
    return targetStocks;
}

void Config::setTargetStocksDataInfo(map<string, StockDataInfo> _targetStocksDataInfo)
{
    targetStocksDataInfo = _targetStocksDataInfo;
}

map<string, StockDataInfo> Config::getTargetStocksDataInfo()
{
    return targetStocksDataInfo;
}
