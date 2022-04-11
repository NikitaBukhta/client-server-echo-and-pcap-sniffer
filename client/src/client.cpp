#include "client.h"
#include "posix_wrapping.h"

#include <iostream>         // std::cerr
#include <sys/socket.h>
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