#include "client.h"
#include "posix_wrapping.h"

#include <stdexcept>    // dedault exceptions;
#include <unistd.h>     // STDOUT_FINO;
#include <string>
#include <iostream>     // cout, getline, cin, endl;

using namespace client;

void singleMessageMode(Client& client, std::string& msg)
{
    client.sendMessage(msg);

    std::string buffer;
    client.readMessage(buffer);
    buffer.push_back('\n');

    std::cout << buffer << std::endl;
}

void interactiveMode(Client& client)
{
    std::string buffer;

    while (true)
    {
        buffer.clear();
        printf("Waiting for your message: ");
        std::getline(std::cin, buffer);     // read full line;
        if (buffer == "\n" || buffer == "") 
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

    Client client(port, IPv4);

    // get accept connection message from server;
    std::string *acceptBuffer = new std::string;
    client.readMessage(*acceptBuffer);
    std::cout << *acceptBuffer << std::endl;
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