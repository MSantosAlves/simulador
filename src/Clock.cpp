#include "Clock.h"

#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <vector>

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <ctime>
#include <iomanip>

using namespace std;
using namespace chrono;

high_resolution_clock::time_point timeStringToTimePoint(string timeString)
{
    vector<int> timeComponents;
    stringstream ss(timeString);
    string item;

    while (getline(ss, item, ':'))
    {
        timeComponents.push_back(stoi(item));
    }

    stringstream msstream(item);
    string microsecondsString;

    while (getline(msstream, microsecondsString, '.'))
    {
        timeComponents.push_back(stoi(microsecondsString));
    }

    // Set the hours, minutes, and seconds
    auto _hours = hours(timeComponents[0]);
    auto _minutes = minutes(timeComponents[1]);
    auto _seconds = seconds(timeComponents[2]);
    int _microseconds = stoi(microsecondsString);

    // Get the current date and time
    system_clock::time_point now = system_clock::now();

    // Convert the current time to a time_t value
    std::time_t now_time_t = system_clock::to_time_t(now);

    // Get the date part (year, month, and day)
    std::tm now_tm = *std::localtime(&now_time_t);

    // Set the time part (hours, minutes, seconds, and microseconds)
    now_tm.tm_hour = duration_cast<hours>(_hours).count();
    now_tm.tm_min = duration_cast<minutes>(_minutes).count();
    now_tm.tm_sec = duration_cast<seconds>(_seconds).count();

    high_resolution_clock::time_point tp = system_clock::from_time_t(std::mktime(&now_tm)) + microseconds(_microseconds);
    
    return tp;
}

Clock::Clock()
{
}

void Clock::setSimulationTime(string timeString)
{
    simulationTime = timeStringToTimePoint(timeString);
    realTimeLastSimulationTimeUpdate = high_resolution_clock::now();
}

high_resolution_clock::time_point Clock::getSimulationTime()
{
    return simulationTime;
}

high_resolution_clock::time_point Clock::getUpdatedSimulationTime()
{
    microseconds timeElapsedInMicroseconds = duration_cast<microseconds>(high_resolution_clock::now() - realTimeLastSimulationTimeUpdate);

    simulationTime = simulationTime + timeElapsedInMicroseconds;
    
    realTimeLastSimulationTimeUpdate = high_resolution_clock::now();
    
    return simulationTime;
}

string Clock::getSimulationTimeHumanReadable()
{

    high_resolution_clock::time_point myTimePoint = getUpdatedSimulationTime();

    time_t time_t_value = high_resolution_clock::to_time_t(myTimePoint);

    // Extract hours, minutes, seconds, and microseconds
    tm timeinfo = *localtime(&time_t_value);
    long long microseconds = chrono::duration_cast<chrono::microseconds>(myTimePoint.time_since_epoch()).count() % 1000000;

    // Format the time as "HH.MM.SS.mmmmmm"
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeinfo);
    string formattedTime = string(buffer) + "." + to_string(microseconds);

    return formattedTime;
}

int64_t Clock::getTimeBetweenOrdersInMicroseconds(string lastOrderPriorityTime, string currOrderPriorityTime)
{
    high_resolution_clock::time_point lastTimePoint = timeStringToTimePoint(lastOrderPriorityTime);
    high_resolution_clock::time_point currTimePoint = timeStringToTimePoint(currOrderPriorityTime);

    auto timeDifference = chrono::duration_cast<std::chrono::milliseconds>(currTimePoint - lastTimePoint);
    int64_t milliseconds = timeDifference.count();

    return milliseconds;
}