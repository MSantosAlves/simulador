#include "Order.h"
#include "StringUtils.h"
#include "StockInfo.h"
#include "ArrayUtils.h"
#include "Trader.h"
#include "ServerResponseSender.h"

#include <string>
#include <map>
#include <fstream>

using namespace std;

#ifndef __ORDER_UTILS_H__
#define __ORDER_UTILS_H__

class OrderUtils
{
public:
	OrderUtils();
	Order parseOrder(string order, StringUtils stringUtils);
	void orderMatching(string symbol, Order order, map<string, StockInfo> *offersBook, ArrayUtils arrayUtils, Trader *traderAccount, ofstream &trade_history_file);
	void executePossibleTrades(string symbol, map<string, StockInfo> *offersBook, int bookUpdateDirection, ServerResponseSender *responseSender);
};

#endif