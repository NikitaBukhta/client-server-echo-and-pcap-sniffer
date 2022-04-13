#include "server.h"
#include "posix_wrapping.h"

#include <stdexcept>
#include <vector>
#include <sys/select.h>     // temp;
#include <unistd.h>
#include <algorithm>        // std::remove;
#include <string>


int main(int argc, char **argv)
{
    if (argc < 2)
        throw std::invalid_argument("You must pass the port number when starting "
            "the server!");

    int port = atoi(argv[1]);

    cs::Server server(port, 10);
    std::vector<int> clients;
    bool connectionMade = false;
    char msg[1024];
    char prefix[] = "Server echo: ";
    int serverSocket = server.getServerSocket();

    fd_set read_fds;
    //FD_ZERO(&read_fds);
    //FD_ZERO(&write_fds);
    //FD_ZERO(&except_fds);
    //FD_SET(serverSocket, &read_fds);

    struct timeval timeout = {0};
    
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
            FD_ZERO(&read_fds);
            FD_SET(client, &read_fds);
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;

            //printf("waiting for a message...\n");
            if(select(client + 1, &read_fds, NULL, NULL, &timeout))
            {
                std::string currentClient = "client " + std::to_string(client) + " (" + server.getClientIP(client) + ")";
                std::string buf = "waiting for answer from " + currentClient + "\n";
                write(STDOUT_FILENO, buf.c_str(), buf.size());
                //printf("waiting for answer from client %d (%s)", client, server.getClientIP(client));
                if (server.readMessage(msg))
                {
                    server.modifyMessage(msg, "\n", false);
                    write(STDOUT_FILENO, msg, strlen(msg));
                    server.modifyMessage(msg, "Server echo: ", true);
                    server.sendMessage(msg, client);

                    /*
                    server.disconnectClient(client);
                    clients.clear();
                    server.getClientSockets(clients);
                    */
                }
                else
                {
                    server.disconnectClient(client);
                    clients.clear();
                    server.getClientSockets(clients);
                    //std::remove(clients.begin(), clients.end(), client);
                }

                //FD_CLR(client, &read_fds);
            } 
            strcpy(msg, "");    // clear msg data;
        }

        // if some connections have been made and the server is empty now;
        if ((clients.size() == 0) && connectionMade) break;
    }
    

    return 0;
}