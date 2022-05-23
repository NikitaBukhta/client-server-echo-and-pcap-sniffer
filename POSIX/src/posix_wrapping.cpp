#include "posix_wrapping.h"
#include "posixError.h"

#include <sys/socket.h>
#include <errno.h>
#include <string>
#include <unistd.h> 

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

ssize_t POSIX::_read(int socketFD, void *buf, size_t nbyte)
{
    ssize_t nread;
    nread = read(socketFD, buf, nbyte);

    if (nread == -1)
    {
        std::string error("read error: ");
        error += strerror(errno);
        
        throw PosixError(error.c_str());
    }

    return nread;
}

ssize_t POSIX::_write(int socketFD, const void *buf, size_t nbyte)
{
    ssize_t nwrite;
    nwrite = write(socketFD, buf, nbyte);

    if (nwrite == -1)
    {
        std::string error("write error: ");
        error += strerror(errno);
        
        throw PosixError(error.c_str());
    }

    return nwrite;
}

bool POSIX::_select(int socketFD, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout)
{
    int ret = select(socketFD, readfds, writefds, errorfds, timeout);

    if (ret == -1)
    {
        std::string error("select error: ");
        error += strerror(errno);
        
        throw PosixError(error.c_str());
    }

    return ret;
}

ssize_t POSIX::_send(int socket, const void *buffer, size_t length, int flags)
{
    ssize_t ret = send(socket, buffer, length, flags);

    if (ret == -1)
    {
        std::string error("send error: ");
        error += strerror(errno);
        
        throw PosixError(error.c_str());
    }
    
    return ret;
}

int POSIX::_fcntl(int fildes, int cmd, ...)
{
    va_list valist;

    int ret = fcntl(fildes, cmd, valist);

    if (ret == -1)
    {
        std::string error("send error: ");
        error += strerror(errno);
        
        throw PosixError(error.c_str());
    }

    va_end(valist);

    return ret;
}