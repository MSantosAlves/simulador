#include "DataUtils.h"
#include "StringUtils.h"

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <fstream>

using namespace std;

DataUtils::DataUtils() {}

bool compareDates(string i1, string i2)
{
	StringUtils stringUtils;
	long long date1 = stringUtils.dateToInt(stringUtils.split(i1, ';')[6]);
	long long date2 = stringUtils.dateToInt(stringUtils.split(i2, ';')[6]);
	return (date1 < date2);
}

vector<string> DataUtils::readToMemory(vector<string> sourcePaths) {
	vector<string> data;
	for (int i = 0; i < sourcePaths.size(); i++)
	{
		ifstream file(sourcePaths[i]);
		string line;
		while (getline(file, line))
		{
			data.push_back(line);
		}
	}
	return data;
}

vector<string> DataUtils::sortByDate(vector<string> data) {
	StringUtils stringUtils;
	sort(data.begin(), data.end(), compareDates);
	return data;
}

void DataUtils::writeToFile(vector<string> data, string destinationPath) {}