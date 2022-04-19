#include "server.h"
#include "posix_wrapping.h"

#include <stdexcept>        // default exceptions;
#include <unistd.h>         // STDOUT_FILENO;
#include <string>
#include <iostream>         // cout, endl;

#define PREFIX "Server echo: "

/* Description:
 * Desconnect the client from the server and rewrite content from
 * vector (remove disconnected client);
 * 
 * ARGS:
 * client - client socket;
 * server - server where we remove the client;
 * clientList - vector with client sockets;
 */
void makeDisconnect(int client, cs::Server& server, std::map<int, unsigned short>& clientList);

int main(int argc, char **argv)
{
    if (argc < 2)
        throw std::invalid_argument("You must pass the port number when starting "
            "the server!");

    int port = atoi(argv[1]);

    cs::Server server(port, 10);
    // key - clientSocket, value - count of iteration;
    std::map<int, unsigned short> clients;
    bool connectionMade = false;    /* this var is need in order to 
                                     * do not close the server, if
                                     * any connections have't made
                                     * earlier;
                                     */
    std::string msg;

    fd_set read_fds;                // read fields for check an client performance;
    struct timeval timeout = {0};   // checking interval;
    
    // every iteration n, we make disconnect all passive connections;
    unsigned short iterationForCheck = 20;

    while (true)
    {  

        if(server.acceptClientConnection())
        {
            connectionMade = true;
            server.getClientSockets(clients);
        }

        // check every client
        for (unsigned short i = 0; i < clients.size(); ++i)
        {
            auto client = clients.begin();
            std::advance(client, i);

            ++(client->second);

            // if nothing is read, return 0;
            if (server.readMessage(msg, client->first))
            {
                std::cout << "Message from socket " << client->first << " (" 
                    << server.getClientIP(client->first) << " ): " << msg << std::endl;
                
                msg = PREFIX + msg + '\0';
                server.sendMessage(msg, client->first);

                // if we have got the message, reset the number of iterations to zero;
                client->second = 0;
            }
            else if (client->second >= iterationForCheck)
            {
                makeDisconnect(client->first, server, clients);
                --i;
            }
            sleep(1);
        }

        // if some connections have been made and the server is empty now;
        if ((clients.size() == 0) && connectionMade) break;
    }

    return 0;
}

void makeDisconnect(int client, cs::Server& server, std::map<int, unsigned short>& clientList)
{
    std::cout << "Disconnect socket " << client << " (" << server.getClientIP(client) << " ): " << std::endl;

    server.disconnectClient(client);
    clientList.erase(client);
}