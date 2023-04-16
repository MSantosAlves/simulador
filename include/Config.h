#include <string>
#include <vector>
using namespace std;

#ifndef __CONFIG_H__
#define __CONFIG_H__

class Config
{
private:
	string date;
	string dataPath;
	vector<string> targetStocks;

public:
	Config();
	void setDate(string date);
	string getDate();
	void setDataPath(string dataPath);
	string getDataPath();
	void setTargetStocks(vector<string> targetStocks);
	vector<string> getTargetStocks();
};

#endif