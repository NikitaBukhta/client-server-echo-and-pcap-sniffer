#include "server.h"
#include "posix_wrapping.h"

#include <stdexcept>
#include <vector>
#include <unistd.h>         // STDOUT_FILENO
#include <algorithm>        // std::find;

/* Description:
 * Desconnect the client from the server and rewrite content from
 * vector (remove disconnected client);
 * 
 * ARGS:
 * client - client socket;
 * server - server where we remove the client;
 * clientList - vector with client sockets;
 */
void makeDisconnect(int client, cs::Server& server, std::vector<int>& clientList)
{
    server.disconnectClient(client);
    clientList.erase(std::find(clientList.begin(), clientList.end(), client));
}

int main(int argc, char **argv)
{
    if (argc < 2)
        throw std::invalid_argument("You must pass the port number when starting "
            "the server!");

    int port = atoi(argv[1]);

    cs::Server server(port, 10);
    std::vector<int> clients;
    bool connectionMade = false;    /* this var is need in order to 
                                     * do not close the server, if
                                     * any connections have't made
                                     * earlier;
                                     */
    char msg[1024];
    char prefix[] = "Server echo: ";

    fd_set read_fds;                // read fields for check an client performance;
    struct timeval timeout = {0};   // checking interval;
    
    while (true)
    {   
        if(server.acceptClientConnection())
        {
            connectionMade = true;
            clients.clear();
            server.getClientSockets(clients);
        }

        // check every client
        for (auto& client : clients)
        {
            // reinitialized the variables;
            FD_ZERO(&read_fds);
            FD_SET(client, &read_fds);
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;

            try
            {
                // check if client can send message;
                if(POSIX::_select(client + 1, &read_fds, NULL, NULL, &timeout))
                {
                    // if nothing is read, return 0;
                    if (server.readMessage(msg, client))
                    {
                        server.modifyMessage(msg, "\n", false);
                        POSIX::_write(STDOUT_FILENO, msg, strlen(msg));
                        server.modifyMessage(msg, "Server echo: ", true);
                        server.sendMessage(msg, client);

                        strcpy(msg, "");    // clear msg data;
                    }
                    // if connection is lost, make disconnect the client from the server;
                    else
                        makeDisconnect(client, server, clients);
                }
            }
            // the same, if we cannot select the socket;
            catch(const std::exception& e)
            {
                makeDisconnect(client, server, clients);
            }
        }

        // if some connections have been made and the server is empty now;
        if ((clients.size() == 0) && connectionMade) break;
    }

    return 0;
}