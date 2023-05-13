#include "Semaphore.h"
#include "Order.h"
#include "PurchaseOrder.h"
#include "SaleOrder.h"

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
	void startProcessOrders(vector<string>* ordersToBeProcessed, vector<string>* offersBook, Semaphore* semaphore, map<string, vector<PurchaseOrder>>* purchases, map<string, vector<SaleOrder>>* sales);
};

#endif