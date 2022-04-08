#include "posixError.h"
#include "server.h"
#include "posix_wrapping.h"

#include <iostream>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc < 2)
        throw std::invalid_argument("You must pass the port number when starting "
            "the server!");

    int port = atoi(argv[1]);

    cs::Server server(port, 0);
    server.acceptClientConnection();

    std::cout << "Read message: " << server.readMessage() << std::endl;

    return 0;
}