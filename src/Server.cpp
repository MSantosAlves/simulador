#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "Server.h"

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

void Server::acceptConnections()
{
    int newSocket;
    int addrlen = sizeof(serverAddress);
    struct sockaddr_in clientAddress;

    while (true)
    {
        // Accept incoming connections
        if ((newSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, (socklen_t *)&addrlen)) < 0)
        {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        char buffer[1024] = {0};
        int valread;

        valread = read(newSocket, buffer, 1024);
        std::cout << "Received data: " << buffer << std::endl;

        send(newSocket, "Message received by the server", strlen("Message received by the server"), 0);

        close(newSocket);
    }
}
