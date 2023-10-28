#include "Semaphore.h"
#include "Order.h"
#include "StockInfo.h"
#include "ServerResponseSender.h"
#include "Clock.h"

#include <vector>
#include <string>
#include <map>

using namespace std;

#ifndef __ORDER_SERVICE_H__
#define __ORDER_SERVICE_H__

class OrderService
{
private:
	Clock *clock;

public:
	OrderService(Clock *clock);
	void startProcessOrders(vector<string> *rawOrdersQueue, map<string, StockInfo> *offersBook, Semaphore *semaphore, ServerResponseSender *responseSender);
};

#endif