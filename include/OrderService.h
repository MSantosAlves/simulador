#include "Semaphore.h"
#include "Order.h"
#include "StockInfo.h"
#include "ServerResponseSender.h"

#include <vector>
#include <string>
#include <map>

using namespace std;

#ifndef __ORDER_SERVICE_H__
#define __ORDER_SERVICE_H__

class OrderService
{
private:
	vector<string> targetStocks;

public:
	OrderService(vector<string> targetStocks);
	void startProcessOrders(vector<string> *rawOrdersQueue, map<string, StockInfo> *offersBook, Semaphore *semaphore, ServerResponseSender *responseSender);
};

#endif