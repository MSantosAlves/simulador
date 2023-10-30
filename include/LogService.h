#include "Semaphore.h"
#include "Clock.h"
#include "Context.h"
#include "StockInfo.h"
#include "ServerResponseSender.h"

#include <string>
#include <map>
#include<queue>

using namespace std;

#ifndef __LOG_SERVICE_H__
#define __LOG_SERVICE_H__

class LogService
{
public:
	LogService(Clock* clock, Context* context);
	void startLogSystem(map<string, StockInfo> *offersBook, Semaphore *semaphore);
	void sendMarkedDataOnTick(map<string, StockInfo>* offersBook, Semaphore* semaphore, ServerResponseSender* responseSender);
	Clock* clock;
	Context* context;
};

#endif