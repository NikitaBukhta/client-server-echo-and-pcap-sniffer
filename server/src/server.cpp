#include "server.h"
#include "posix_wrapping.h"

#include <unistd.h>     // close();
#include <iostream>     // std::cerr

cs::Server::Server(int port, unsigned short maxClientsCount)
{
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    this->maxClientsCount = maxClientsCount;

    MakeServerSocket();
}

cs::Server::Server(int port) : Server(port, 1)
{
}

cs::Server::~Server(void)
{
    // close all clients;
    for (auto client : clients)
    {
        close(client.first);
    }
    
    // close server;
    close(serversSocket);
}

int cs::Server::MakeServerSocket(void)
{
    try
    {
        serversSocket = POSIX::Socket(address.sin_family, SOCK_STREAM, IPPROTO_TCP);
        POSIX::Bind(serversSocket, (struct sockaddr*)(&address), sizeof(address));
        POSIX::Listen(serversSocket, maxClientsCount);
    }
    catch(const POSIX::PosixError& e)
    {
        std::cerr << e.what() << std::endl;
        
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void cs::Server::AcceptClientConnection(void)
{
    sockaddr_in clientAddress = {0};
    clientAddress.sin_family = address.sin_family;
    clientAddress.sin_port = address.sin_port;
    socklen_t clientAddressLength = sizeof(clientAddress);

    try
    {
        int clientSocket = POSIX::Accept(serversSocket, (sockaddr*)(&clientAddress), 
            &clientAddressLength);
        char *clientIP = POSIX::InetNtoa(clientAddress.sin_addr);

        std::cout << "Client (" << clientIP << ") connected to the server!" 
        << std::endl;
    }
    catch(const POSIX::PosixError& e)
    {
        std::cerr << e.what() << std::endl;
    }
}