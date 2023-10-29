#include "Order.h"
#include "StringUtils.h"
#include "StockInfo.h"
#include "ArrayUtils.h"
#include "ServerResponseSender.h"
#include "Clock.h"

#include <string>
#include <map>
#include <fstream>

using namespace std;

#ifndef __ORDER_UTILS_H__
#define __ORDER_UTILS_H__

class OrderUtils
{
private:
	Clock *clock;
public:
	OrderUtils(Clock *clock);
	Order parseOrder(string order, StringUtils stringUtils, Order *orderBuffer);
	void executePossibleTrades(string symbol, map<string, StockInfo> *offersBook, int bookUpdateDirection, ServerResponseSender *responseSender);
};

#endif