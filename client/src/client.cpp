#include "client.h"
#include "posix_wrapping.h"

#include <iostream>         // std::cerr
#include <sys/socket.h>     // struct sockaddr
#include <unistd.h>         // close()
#include <netinet/tcp.h>    // TCP_KEEPINTVL
#include <string>

using namespace client;

Client::Client(int port, char *IPv4)
{
    address = {0};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    try
    {
        POSIX::_inetPton(address.sin_family, IPv4, &address.sin_addr);
        
        serverSocket = POSIX::_socket(address.sin_family, SOCK_STREAM, IPPROTO_TCP);
        enableKeepalive(serverSocket, 10);

        POSIX::_connect(serverSocket, (struct sockaddr*)(&address), sizeof(address));
    }
    catch(const POSIX::PosixError& e)
    {
        std::cerr << e.what() << '\n';
    }
}

Client::~Client(void)
{
    close(serverSocket);
}

ssize_t Client::readMessage(std::string& buffer) const
{
    buffer.clear();         // clear buffer from old information;

    const unsigned short bufferSize = 1024;
    // !!! WARNING, can be not empty fully after the last calling !!!
    char temp[bufferSize];      // place for writting off client's message;
    ssize_t nread;      // count of chars read;
    
    try
    {
        nread = POSIX::_read(serverSocket, temp, bufferSize);
    }
    catch(const POSIX::PosixError& e)
    {
        buffer = "";   // add new info;

        return 0;
    }

    buffer.append(temp, nread);    // add new info;

    return nread;
}

void Client::sendMessage(const std::string& msg) const
{
    auto a = msg.c_str();
    auto count = msg.size() * sizeof(char);
    try
    {
        // +1 is '\0'
        POSIX::_send(serverSocket, msg.c_str(), msg.size() + 1, MSG_OOB);
    }
    catch(const POSIX::PosixError& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Client::enableKeepalive(int serverSocket, int interval)
{
    int keepaliveOpt = true;
    try
    {
        // turn on keepalive;
        POSIX::_setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, &keepaliveOpt, sizeof(keepaliveOpt));
        // set interval between checking;
        POSIX::_setsockopt(serverSocket, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}