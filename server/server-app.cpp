#include "server.h"

#include <stdexcept>
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

    char message[256];
    server.readMessage(message);
    char prefix[] = "Server echo: ";
    server.modifyMessage(message, prefix, true);
    server.sendMessage(message, clientSockets[0]);

    return 0;
}