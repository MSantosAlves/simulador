#include "Semaphore.h"
#include "Clock.h"
#include "Context.h"
#include "StockInfo.h"
#include "ServerResponseSender.h"

#include <string>
#include <map>

using namespace std;

#ifndef __LOG_SERVICE_H__
#define __LOG_SERVICE_H__

class LogService
{
public:
	LogService(Clock* clock, Context* context);
	void startLogSystem(map<string, StockInfo> *offersBook, Semaphore *semaphore);
	void sendDataOnTick(map<string, StockInfo>* offersBook, Semaphore* semaphore, ServerResponseSender* responseSender);
	void printContextOnTick(map<string, StockInfo> *offersBook);
	Clock* clock;
	Context* context;
};

#endif