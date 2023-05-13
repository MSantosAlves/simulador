#include <string>
#include <vector>

#include "Semaphore.h"

using namespace std;

#ifndef __DATA_SERVICE_H__
#define __DATA_SERVICE_H__

class DataService
{
private:    
    string date;
    string dataPath;   
public:
    DataService(string date, string dataPath);
    void startAcquisition(vector<string>* ordersToBeProcessed, Semaphore* semaphore);
};

#endif