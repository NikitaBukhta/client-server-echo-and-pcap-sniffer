#include "posixError.h"
#include "server.h"
#include "posix_wrapping.h"

#include <iostream>
#include <string.h>
#include <vector>

int main(int argc, char **argv)
{
    if (argc < 2)
        throw std::invalid_argument("You must pass the port number when starting "
            "the server!");

    int port = atoi(argv[1]);

    cs::Server server(port, 1);
    server.acceptClientConnection();

    std::vector<int> clientSockets;
    server.getClientSockets(clientSockets);

    for (auto& socket : clientSockets)
        std::cout << "Socket: " << socket << std::endl;


    //std::string msg = "";
    char msg[1024];
    server.readMessage(msg, clientSockets[0]);
    std::cout << "Read message: " << msg << std::endl;

    

    return 0;
}