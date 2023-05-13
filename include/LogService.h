#include "Semaphore.h"
#include "Order.h"

#include <string>
#include <atomic>
#include <vector>

using namespace std;

#ifndef __LOG_SERVICE_H__
#define __LOG_SERVICE_H__

class LogService
{
public:
	LogService();
	void startLogSystem(vector<string>* order, vector<string>* book, Semaphore* semaphore, vector<Order>* processedOrders);
};

#endif