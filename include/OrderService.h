#include "Semaphore.h"
#include "Order.h"
#include "StockInfo.h"
#include "ServerResponseSender.h"
#include "Server.h"
#include "Clock.h"
#include "Context.h"

#include <vector>
#include <string>
#include <map>
#include <queue>

using namespace std;

#ifndef __ORDER_SERVICE_H__
#define __ORDER_SERVICE_H__

class OrderService
{
private:
	Clock *clock;
	Context *context;

public:
	OrderService(Clock *clock, Context *context);
	void startProcessOrders(queue<string> *rawOrdersQueue, map<string, StockInfo> *offersBook, Semaphore *semaphore, ServerResponseSender *responseSender, Server *server);
};

#endif