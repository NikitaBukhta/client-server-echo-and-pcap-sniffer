#include "client.h"
#include "posix_wrapping.h"

#include <stdexcept>
#include <unistd.h>     // STDOUT_FINO
#include <string.h>
#include <iostream>

void singleMessageMode(cs::Client& client, std::string& msg)
{
    client.sendMessage(msg);

    std::string buffer;
    client.readMessage(buffer);
    buffer.push_back('\n');

    POSIX::_write(STDOUT_FILENO, buffer.c_str(), buffer.size());
}

void interactiveMode(cs::Client& client)
{
    std::string buffer;

    while (true)
    {
        printf("Waiting for your message: ");
        std::getline(std::cin, buffer);     // read full line;
        if (buffer == "\n") 
            break;

        singleMessageMode(client, buffer);
    }
}

int main(int argc, char **argv)
{
    if (argc < 3)
        throw std::invalid_argument("You must pass the port number and then IP address when starting the server!");

    char* const IPv4 = argv[1];
    int port = atoi(argv[2]);

    cs::Client client(port, IPv4);

    // get accept connection message from server;
    std::string *acceptBuffer = new std::string;
    POSIX::_write(STDOUT_FILENO, acceptBuffer->c_str(), acceptBuffer->size());
    //std::cout << acceptBuffer << std::endl;
    client.readMessage(*acceptBuffer);
    delete acceptBuffer;

    if (argc == 3)
        interactiveMode(client);
    else
    {
        std::string msg = argv[3];
        singleMessageMode(client, msg);
    }


    return 0;
}