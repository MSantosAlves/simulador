#include <string>
#include <vector>

#include "Semaphore.h"

#ifndef __DATA_SERVICE_H__
#define __DATA_SERVICE_H__

class DataService
{
public:
    DataService();
    void startAcquisition(vector<string>* events, Semaphore* semaphore);
};

#endif