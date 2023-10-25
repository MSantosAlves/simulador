#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

#ifndef __CLOCK_H__
#define __CLOCK_H__

class Clock
{
private:

    high_resolution_clock::time_point realStartTime;
    high_resolution_clock::time_point realTimeLastSimulationTimeUpdate;
    high_resolution_clock::time_point simulationTime;

public:
	Clock();
	void setRealStartTime();
	high_resolution_clock::time_point getRealStartTime();

    void setSimulationTime(string timeString);
	high_resolution_clock::time_point getSimulationTime();

    high_resolution_clock::time_point getUpdatedSimulationTime();

    string getSimulationTimeHumanReadable();

    int64_t getTimeBetweenOrdersInMicroseconds(string lastOrderPriorityTime, string currOrderPriorityTime);

};

#endif