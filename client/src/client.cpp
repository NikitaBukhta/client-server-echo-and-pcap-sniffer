#include "client.h"
#include "posix_wrapping.h"

#include <iostream>         // std::cerr
#include <sys/socket.h>     // struct sockaddr
#include <unistd.h>         // close()

cs::Client::Client(int port, char *IPv4)
{
    address = {0};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    try
    {
        POSIX::_inetPton(address.sin_family, IPv4, &address.sin_addr);
        
        serverSocket = POSIX::_socket(address.sin_family, SOCK_STREAM, IPPROTO_TCP);
        POSIX::_connect(serverSocket, (struct sockaddr*)(&address), sizeof(address));
    }
    catch(const POSIX::PosixError& e)
    {
        std::cerr << e.what() << '\n';
    }
}

cs::Client::~Client(void)
{
    close(serverSocket);
}

ssize_t cs::Client::readMessage(char *buffer)
{
    const unsigned short bufferSize = 1024;
    char buf[bufferSize];      // place for writting off client's message;
    ssize_t nread;      // count of chars read;
    
    try
    {
        nread = POSIX::_read(serverSocket, &buf, bufferSize);
    }
    catch(const POSIX::PosixError& e)
    {
        std::cerr << e.what() << std::endl;

        strcpy(buf, "");    // fill buffer with empty string, if error is happened;

        return 0;
    }

    strcpy(buffer, buf);

    return nread;
}

void cs::Client::sendMessage(char *message)
{
    try
    {
        POSIX::_write(serverSocket, message, strlen(message));
    }
    catch(const POSIX::PosixError& e)
    {
        std::cerr << e.what() << std::endl;
    }
}