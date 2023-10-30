#include <string>
#include <chrono>

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
    bool shouldStopSimulation = false;
    chrono::high_resolution_clock::time_point startTimePoint;

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
    chrono::high_resolution_clock::time_point getStartTimePoint();
    void setStartTimePoint(chrono::high_resolution_clock::time_point startTimePoint);
    void setShouldStopSimulation(bool value);
    bool simulationShouldContinue();

};

#endif