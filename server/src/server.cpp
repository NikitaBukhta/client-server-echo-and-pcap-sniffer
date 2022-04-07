#include "server.h"
#include "posix_wrapping.h"

#include <unistd.h>     // close();
#include <iostream>     // std::cerr

Server::Server(int port, unsigned short maxClientsCount)
{
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    this->maxClientsCount = maxClientsCount;

    MakeServerSocket();
}

Server::Server(int port) : Server(port, 1)
{
}

Server::~Server(void)
{
    // close all clients;
    for (auto client : clients)
    {
        close(client.first);
    }
    
    // close server;
    close(serversSocket);
}

int Server::MakeServerSocket(void)
{
    try
    {
        serversSocket = Socket(address.sin_family, SOCK_STREAM, IPPROTO_TCP);
        Bind(serversSocket, (struct sockaddr*)(&address), sizeof(address));
        Listen(serversSocket, maxClientsCount);
    }
    catch(const PosixError& e)
    {
        std::cerr << e.what() << std::endl;
        
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}