#include "Server.h"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

Server::Server(int _port)
{
    port = _port;
    // Create a socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Set server address parameters
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
}

int *Server::getClientSocketAddress()
{
    return &clientSocket;
}

int waitForConnectionsLoop(int serverSocket, struct sockaddr_in clientAddress, int addrlen)
{
    bool conected = false;
    int clientSocketConnection;

    while (!conected)
    {
        if ((clientSocketConnection = accept(serverSocket, (struct sockaddr *)&clientAddress, (socklen_t *)&addrlen)) < 0)
        {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        conected = true;

        std::cout << "New client connected. Socket FD: " << clientSocketConnection << std::endl;
    }

    return clientSocketConnection;
}

void Server::acceptConnections(queue<string> *rawOrdersQueue, Semaphore *semaphore)
{
    int addrlen = sizeof(serverAddress);
    struct sockaddr_in clientAddress;
    chrono::nanoseconds timespan(1);
    string orderBuffer = "";
    json jsonData = {};

    semaphore->acquire();
    clientSocket = waitForConnectionsLoop(serverSocket, clientAddress, addrlen);
    semaphore->release();

    while (true)
    {
        char buffer[1024] = {0};

        read(clientSocket, buffer, 1024);
        if (buffer != "")
        {
            jsonData = json::parse(buffer);
            orderBuffer = jsonData["offer"];
            semaphore->acquire();
            rawOrdersQueue->push(orderBuffer);
            semaphore->release();
        }
    }

    return;
}

void Server::closeServer(){
    ::close(port);
    _exit(0);
}