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

    makeServerSocket();
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

int cs::Server::makeServerSocket(void)
{
    try
    {
        serversSocket = POSIX::_socket(address.sin_family, SOCK_STREAM, IPPROTO_TCP);
        POSIX::_bind(serversSocket, (struct sockaddr*)(&address), sizeof(address));
        POSIX::_listen(serversSocket, maxClientsCount);
    }
    catch(const POSIX::PosixError& e)
    {
        std::cerr << e.what() << std::endl;
        
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void cs::Server::acceptClientConnection(void)
{
    sockaddr_in clientAddress = {0};
    clientAddress.sin_family = address.sin_family;
    clientAddress.sin_port = address.sin_port;
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket;

    try
    {
        clientSocket = POSIX::_accept(serversSocket, (sockaddr*)(&clientAddress), 
            &clientAddressLength);
        
        // if server is full, throw the error;
        if (maxClientsCount <= clients.size())
        {
            char errorMessage[] = "Server is full. Try to connect letter!\n";
            throw cs::ConnectionError(errorMessage);
        }

        clients.emplace(clientSocket, clientAddress);
        char *clientIP = getClientIP(clientSocket);
        
        std::cout << "Client (" << clientIP << ") connected to the server!"; 
        sendMessage("Connected\n", clientSocket);
    }
    catch(const POSIX::PosixError& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch(const cs::ConnectionError& e )
    {
        // send the error message and close connection with the client;
        const char* errorMessage = e.what();
        sendMessage(errorMessage, clientSocket);
        //write(clientSocket, errorMessage, strlen(errorMessage));
        close(clientSocket);
    }
}

char* cs::Server::getClientIP(int clientSocket)
{
    auto client = clients.find(clientSocket);  
    
    try
    {
        return POSIX::_inetNtoa(client->second.sin_addr);
    }
    catch(const POSIX::PosixError& e)
    {
        std::cerr << e.what() << '\n';
        return nullptr;
    }
}

ssize_t cs::Server::readMessage(char *buffer, int clientSocket)
{
    const unsigned short bufferSize = 1024;
    char buf[bufferSize];      // place for writting off client's message;
    ssize_t nread;      // count of chars read;
    
    try
    {
        nread = POSIX::_read(clientSocket, &buf, bufferSize);
    }
    catch(const POSIX::PosixError& e)
    {
        std::cerr << e.what() << std::endl;

        strcpy(buf, "");    // fill buffer with empty string, if error is happened;
    }

    strcpy(buffer, buf);

    return nread;
}

ssize_t cs::Server::readMessage(char *buffer)
{
    ssize_t nread = 0;
    do
    {
        for (auto& client : clients)
        {
            nread = readMessage(buffer, client.first);

            // if message was found, left the function;
            if (strcmp(buffer, "") != 0)
                return nread;
        }
    } while (strcmp(buffer, "") == 0);

    return nread;
}

void cs::Server::getClientSockets(std::vector<int>& clientSockets)
{
    for (auto& client : clients)
    {
        clientSockets.push_back(client.first);
    }
}

void cs::Server::sendMessage(const char *message, int clientSocket)
{
    try
    {
        POSIX::_write(clientSocket, message, strlen(message));
    }
    catch(const POSIX::PosixError& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void cs::Server::sendMessage(const char *message)
{
    for (auto client : clients)
    {
        try
        {
            POSIX::_write(client.first, message, strlen(message));
        }
        catch(const POSIX::PosixError& e)
        {
            std::cerr << "Client socket " << client.first << ": " << e.what() 
                << std::endl;
        }
    }
}

void cs::Server::modifyMessage(char *message, char *prefix, bool modifyAtStart)
{
    if (modifyAtStart)
    {
        int bufSize = strlen(message) + strlen(prefix) + 1;
        char buf[bufSize];
        strcpy(buf, prefix);
        strcat(buf, message);
        buf[bufSize - 1] = '\0';
        strcpy(message, buf);
    }
    else
    {
        strcat(message, prefix);
    }
}