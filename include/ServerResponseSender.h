#include <iostream>
#include <unistd.h>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

#ifndef __SERVER_RESPONSE_SENDER__
#define __SERVER_RESPONSE_SENDER__

class ServerResponseSender
{
private:
    int *clientSocketPtr;

public:
    ServerResponseSender(int *clientSocketPtr);
    void sendResponse(const json &jsonRespons);
};

#endif