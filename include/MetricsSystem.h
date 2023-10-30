#include "Semaphore.h"
#include "Clock.h"
#include "Context.h"
#include "StockInfo.h"

#include <string>
#include <map>
#include<queue>

using namespace std;

#ifndef __METRICS_SYSTEM_H__
#define __METRICS_SYSTEM_H__

class MetricsSystem
{
    private:
    Clock* clock;
	Context* context;
	map<string, StockInfo> *offersBook; 
	queue<string> *rawOrdersQueue;
	
public:
	MetricsSystem(Clock* clock, Context* context, map<string, StockInfo> *offersBook, queue<string> *rawOrdersQueue);
	void startMetricsSystem(Semaphore *semaphore);
};

#endif