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
	string targetStock;
	StockDataInfo targetStockDataInfo;

public:
	Config();
	void setDate(string date);
	string getDate();
	void setDataPath(string dataPath);
	string getDataPath();
	void setSimulationSpeed(string simulationSpeed);
	string getSimulationSpeed();
	void setTargetStock(string targetStock);
	string getTargetStock();
	StockDataInfo getTargetStockDataInfo();
	void setTargetStockDataInfo(StockDataInfo targetStockDataInfo);
};

#endif