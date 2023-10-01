#include <iostream>
#include <unistd.h>
#include <string>

using namespace std;

#ifndef __CLIENT_RESPONSE_SENDER__
#define __CLIENT_RESPONSE_SENDER__

class ClientResponseSender
{
private:
    int *clientSocketPtr;

public:
    ClientResponseSender(int *clientSocketPtr);
    void sendResponse(string responseMessage);
};

#endif