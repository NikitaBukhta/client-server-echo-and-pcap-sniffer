#include "posix_wrapping.h"
#include "posixError.h"

#include <sys/socket.h>     // socket(), bind()
#include <errno.h>
#include <string>

int POSIX::_socket(int domain, int type, int protocol)
{
    int ret = socket(domain, type, protocol);

    if (ret == -1)
    {
        std::string error("Socket error: ");
        error += strerror(errno);
        
        throw PosixError(error.c_str());
    }

    return ret;
}

int POSIX::_bind(int socketFD, const struct sockaddr* address, socklen_t addressLength)
{
    int ret = bind(socketFD, address, addressLength);
    if (ret == -1)
    {
        std::string error("Bind error: ");
        error += strerror(errno);
        
        throw PosixError(error.c_str());
    }

    return ret;
}

int POSIX::_listen(int socketFD, int backlog)
{
    int ret = listen(socketFD, backlog);
    if (ret == -1)
    {
        std::string error("Listen error: ");
        error += strerror(errno);
        
        throw PosixError(error.c_str());
    }

    return ret;
}

int POSIX::_accept(int socketFD, struct sockaddr* address, socklen_t* addressLength)
{
    int ret = accept(socketFD, address, addressLength);
    if (ret == -1)
    {
        std::string error("Accept error: ");
        error += strerror(errno);
        
        throw PosixError(error.c_str());
    }

    return ret;
}

int POSIX::_inetPton(int family, const char* src, void* destination)
{
    int res = inet_pton(family, src, destination);

    if (res == 0)
    {

        std::string error("int_pton failed: src doesn't contain a charapter"
            " string representing a valid network address in the specified"
            " address family.");

        throw PosixError(error.c_str());
    }
    else if (res == -1)
    {
        std::string error("Accept error: ");
        error += strerror(errno);
        
        throw PosixError(error.c_str());  
    }

    return res;
}

int POSIX::_connect(int socketFD, const struct sockaddr* address, socklen_t addressLength)
{
    int ret = connect(socketFD, address, addressLength);

    if (ret == -1)
    {
        std::string error("Connect error: ");
        error += strerror(errno);
        
        throw PosixError(error.c_str());
    }

    return ret;
}

int POSIX::_setsockopt(int socketFD, int level, int optionName, const void *optionValue, socklen_t optionLength)
{
    int ret = setsockopt(socketFD, level, optionName, optionValue, optionLength);

    if (ret == -1)
    {
        std::string error("Setsockopt error: ");
        error += strerror(errno);
        
        throw PosixError(error.c_str());
    }

    return ret;
}

char* POSIX::_inetNtoa(const struct in_addr& in)
{
    char *ret = inet_ntoa(in);

    // I don't know, will this function throw error or not,
    // but it returns NULL if some errors happened;
    if (ret == NULL)
    {
        std::string error("inet_ntoa error: ");
        error += strerror(errno);
        
        throw PosixError(error.c_str());
    }

    return ret;
}