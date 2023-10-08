#include "Semaphore.h"
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
	LogService();
	void startLogSystem(map<string, StockInfo> *offersBook, Semaphore *semaphore);
	void sendDataOnTick(map<string, StockInfo>* offersBook, Semaphore* semaphore, ServerResponseSender* responseSender);
};

#endif