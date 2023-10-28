#include <string>

using namespace std;

#ifndef __Context_H__
#define __Context_H__

class Context
{
private:

    float simulationExecuted = 0;

public:
	Context();

    float getSimulationExecuted();
    void setSimulationExecuted(float simulationExecuted);
    string getSimulationExecutedHumandReadable();

};

#endif