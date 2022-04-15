#include "server.h"
#include "posix_wrapping.h"

#include <stdexcept>
#include <vector>
#include <unistd.h>         // STDOUT_FILENO
#include <algorithm>        // std::find;
#include <string>

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
void makeDisconnect(int client, cs::Server& server, std::map<int, unsigned short>& clientList)
{
    server.disconnectClient(client);
    clientList.erase(client);
}

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
    unsigned short iterationForCheck = 40;

    while (true)
    {  

        if(server.acceptClientConnection())
        {
            connectionMade = true;
            server.getClientSockets(clients);
        }

        // check every client
        for (auto& client : clients)
        {
            // reinitialized the variables;
            FD_ZERO(&read_fds);
            FD_SET(client.first, &read_fds);
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;

            try
            {
                ++(client.second);

                // check if client can send message;
                if(POSIX::_select(client.first + 1, &read_fds, NULL, NULL, &timeout))
                {
                    // if nothing is read, return 0;
                    if (server.readMessage(msg, client.first))
                    {
                        POSIX::_write(STDOUT_FILENO, (msg + "\n").c_str(), (sizeof(msg.c_str()) + 2 * sizeof(char)));
                        msg = "Server echo " + msg + "\n";
                        server.sendMessage(msg, client.first);

                        // if we have got the message, reset the number of iterations to zero;
                        client.second = 0;
                    }
                    // if connection is lost, make disconnect the client from the server;
                    else
                        makeDisconnect(client.first, server, clients);
                }
                // if client is passive;
                else if (client.second == iterationForCheck)
                {
                    char clientInfo[70];
                    sprintf(clientInfo, "Client %d (%s) disconnected for passive reason!", 
                        client.first, server.getClientIP(client.first));
                    server.sendMessage("You have been disconnected for passive reason!\n");
                    POSIX::_write(STDOUT_FILENO, clientInfo, strlen(clientInfo));
                    makeDisconnect(client.first, server, clients);
                }
            }
            // the same, if we cannot select the socket;
            catch(const std::exception& e)
            {
                makeDisconnect(client.first, server, clients);
            }
        }

        // if some connections have been made and the server is empty now;
        if ((clients.size() == 0) && connectionMade) break;
    }

    return 0;
}