#ifndef POSIX_WRAPPING_H
#define POSIX_WRAPPING_H

#include "posixError.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int Socket(int domain, int type, int protocol);

int Bind(int socketFD, const struct sockaddr* address, socklen_t addressLength);

int Listen(int socketFD, int backlog);

int Accept(int socketFD, sockaddr* address, socklen_t* addressLength);

#endif // !POSIX_WRAPPING_H