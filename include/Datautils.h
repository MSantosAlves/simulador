#include <vector>
#include <string>
#include "filesystem"

using namespace std;

#ifndef __DATA_UTILS_H__
#define __DATA_UTILS_H__

class DataUtils
{
public:
    DataUtils();
    vector<string> readToMemory(vector<string> sourcePaths);
    vector<string> sortByDate(vector<string> data);
    void writeToFile(vector<string> data, string destinationPath);
};

#endif