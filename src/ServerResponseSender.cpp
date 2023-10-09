#include "ServerResponseSender.h"

#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace std;

ServerResponseSender::ServerResponseSender(int *_clientSocketPtr)
{
    clientSocketPtr = _clientSocketPtr;
}

void ServerResponseSender::sendResponse(const json &jsonResponse)
{
    string jsonString = jsonResponse.dump();
    if (clientSocketPtr)
    {
        send(*clientSocketPtr, jsonString.c_str(), jsonString.size(), 0);
    }
}
