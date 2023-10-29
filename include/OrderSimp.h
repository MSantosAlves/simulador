#include <string>

#ifndef ORDER_SIMPLIFIED_H
#define ORDER_SIMPLIFIED_H

using namespace std;

struct OrderSimp
{
    string sequentialOrderNumber;
    string priorityTime;
    double orderPrice = -1;
    long long priorityTimeInteger;
    int totalQuantityOfOrder = 0;
    int tradedQuantityOfOrder = 0;
    int orderSource = 0;
};

#endif