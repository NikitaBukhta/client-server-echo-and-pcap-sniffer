#include "server.h"
#include "posix_wrapping.h"
#include "connectionError.h"

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
    int clientSocket;

    try
    {
        clientSocket = POSIX::Accept(serversSocket, (sockaddr*)(&clientAddress), 
            &clientAddressLength);
        
        // if server is full, throw the error;
        if (maxClientsCount <= clients.size())
        {
            char errorMessage[] = "Server is full. Try to connect letter!\n";
            throw cs::ConnectionError(errorMessage);
        }

        clients.emplace(clientSocket, clientAddress);
        char *clientIP = GetClientIP(clientSocket);

        std::cout << "Client (" << clientIP << ") connected to the server!" 
        << std::endl;
    }
    catch(const POSIX::PosixError& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch(const cs::ConnectionError& e )
    {
        // send the error message and close connection with the client;
        const char* errorMessage = e.what();
        write(clientSocket, errorMessage, strlen(errorMessage));
        close(clientSocket);
    }
}

char* cs::Server::GetClientIP(int clientSocket)
{
    auto client = clients.find(clientSocket);  
    
    try
    {
        return POSIX::InetNtoa(client->second.sin_addr);
    }
    catch(const POSIX::PosixError& e)
    {
        std::cerr << e.what() << '\n';
        return nullptr;
    }
}