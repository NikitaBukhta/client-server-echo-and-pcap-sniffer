#include "client.h"
#include "posix_wrapping.h"

#include <stdexcept>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc < 3)
        throw std::invalid_argument("You must pass the port number and then IP address when starting the server!");

    int port = atoi(argv[1]);
    char* const IPv4 = argv[2];

    cs::Client client(port, argv[2]);

    char buf[1024];
    ssize_t nread = client.readMessage(buf);
    write(STDOUT_FILENO, buf, nread);
    strcpy(buf, "Hello, server!");
    client.sendMessage(buf);
    nread = client.readMessage(buf);
    write(STDOUT_FILENO, buf, nread);

    return 0;
}