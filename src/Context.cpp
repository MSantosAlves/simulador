#include <string>
#include <math.h>
#include <Context.h>

using namespace std;

Context::Context() {}

void Context::setTargetStock(string _targetStock)
{
    targetStock = _targetStock;
}

string Context::getTargetStock()
{
    return targetStock;
}

void Context::setTotalOrdersSize(int _totalOrdersSize)
{
    totalOrdersSize = _totalOrdersSize;
}

int Context::getTotalOrdersSize()
{
    return totalOrdersSize;
}

string Context::getSimulationExecutedHumandReadable()
{
    float simulationExecuted = ordersRead / float(totalOrdersSize);
    int fileReadInPercent = ceil(simulationExecuted * 100);
    return to_string(fileReadInPercent);
}

void Context::setOrdersRead(int _ordersRead)
{
    ordersRead = _ordersRead;
}

int Context::getOrdersRead()
{
    return ordersRead;
}

void Context::setLastOrdersReadValue(int _lastOrdersReadValue)
{
    lastOrdersReadValue = _lastOrdersReadValue;
}

int Context::getLastOrdersReadValue()
{
    return lastOrdersReadValue;
}