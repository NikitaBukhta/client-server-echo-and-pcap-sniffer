#include "client.h"
#include "posix_wrapping.h"

#include <stdexcept>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc < 3)
        throw std::invalid_argument("You must pass the port number and then IP address when starting the server!");

    char* const IPv4 = argv[1];
    int port = atoi(argv[2]);

    cs::Client client(port, IPv4);
    char buf[1024];     // place where contains messages from the server;

    // read message about connection result;
    ssize_t nread = client.readMessage(buf);
    write(STDOUT_FILENO, buf, nread);

    //send message to the server;
    char msg[] = "This is test message must be sent to the server";
    client.sendMessage(msg);

    // get feedback;
    nread = client.readMessage(buf);
    write(STDOUT_FILENO, buf, nread);

    return 0;
}