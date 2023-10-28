#include <string>
#include <math.h>
#include <Context.h>

using namespace std;

Context::Context(){}
float Context::getSimulationExecuted(){
    return simulationExecuted;
}

void Context::setSimulationExecuted(float _simulationExecuted){
    simulationExecuted = _simulationExecuted;
}
string Context::getSimulationExecutedHumandReadable(){
    int fileReadInPercent = ceil(simulationExecuted * 100);
    return to_string(fileReadInPercent);
}