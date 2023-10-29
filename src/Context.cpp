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

float Context::getSimulationExecuted()
{
    return simulationExecuted;
}

void Context::setSimulationExecuted(float _simulationExecuted)
{
    simulationExecuted = _simulationExecuted;
}
string Context::getSimulationExecutedHumandReadable()
{
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