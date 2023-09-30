#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

#ifndef __SERVER_H__
#define __SERVER_H__

class Server
{
private:
    int serverSocket;
    int port;
    struct sockaddr_in serverAddress;

public:
    Server(int port);
    void acceptConnections();
};

#endif