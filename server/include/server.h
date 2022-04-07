#ifndef SERVER_H
#define SERVER_H

#include "netinet/in.h"

#include <map>

class Server
{
private:
    struct sockaddr_in address;
    int serversSocket;
    std::map<int, struct sockaddr_in> client;
    
    Server(int port);
};

#endif // !SERVER_H