#include <string>

using namespace std;

#ifndef __Context_H__
#define __Context_H__

class Context
{
private:

    string targetStock;
    int totalOrdersSize = 0;
    int ordersRead = 0;
    int lastOrdersReadValue = 0;

public:
	Context();

    void setTargetStock(string targetStock);
    string getTargetStock();
    void setTotalOrdersSize(int totalOrdersSize);
    int getTotalOrdersSize();
    string getSimulationExecutedHumandReadable();
    void setOrdersRead(int ordersRead);
    int getOrdersRead();
    void setLastOrdersReadValue(int lastOrdersReadValue);
    int getLastOrdersReadValue();

};

#endif