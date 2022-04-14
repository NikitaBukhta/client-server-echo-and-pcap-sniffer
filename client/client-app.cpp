#include "client.h"

#include <stdexcept>
#include <unistd.h>     // STDOUT_FINO
#include <string.h>
#include <iostream>

void singleMessageMode(cs::Client& client, const char *msg)
{
    client.sendMessage(msg);
    
    char buffer[strlen(msg) + 30];
    client.readMessage(buffer);

    printf("%s", buffer);
}

void interactiveMode(cs::Client& client)
{
    char buffer[1024];

    while (true)
    {
        printf("Waiting for your message: ");
        fgets(buffer, sizeof(buffer) * sizeof(char), stdin);
        if (strcmp(buffer, "\n") == 0) 
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
    char *buffer = new char[1024];
    client.readMessage(buffer);
    printf("%s\n", buffer);
    delete[] buffer;

    if (argc == 3)
        interactiveMode(client);
    else
    {
        singleMessageMode(client, argv[3]);
    }


    return 0;
}