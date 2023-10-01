#include "ClientResponseSender.h"

#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <arpa/inet.h>

using namespace std;

ClientResponseSender::ClientResponseSender(int *_clientSocketPtr)
{
    clientSocketPtr = _clientSocketPtr;
}

void ClientResponseSender::sendResponse(string data)
{
    if (clientSocketPtr)
    {
        send(*clientSocketPtr, data.c_str(), data.size(), 0);
        close(*clientSocketPtr);
    }
}
