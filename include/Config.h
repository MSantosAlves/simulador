#include <string>
#include <vector>
#include <map>
#include <StockDataInfo.h>
using namespace std;

#ifndef __CONFIG_H__
#define __CONFIG_H__

class Config
{
private:
	string date;
	string dataPath;
	string simulationSpeed;
	vector<string> targetStocks;
	map<string, StockDataInfo> targetStocksDataInfo;

public:
	Config();
	void setDate(string date);
	string getDate();
	void setDataPath(string dataPath);
	string getDataPath();
	void setSimulationSpeed(string simulationSpeed);
	string getSimulationSpeed();
	void setTargetStocks(vector<string> targetStocks);
	vector<string> getTargetStocks();
	map<string, StockDataInfo> getTargetStocksDataInfo();
	void setTargetStocksDataInfo(map<string, StockDataInfo> targetStocksDataInfo);
};

#endif