#ifndef SERVER_H
#define SERVER_H

#include "netinet/in.h"

#include <map>

namespace cs    // client-server
{
    class Server
    {
    private:
        struct sockaddr_in address = {0};
        int serversSocket;
        unsigned short maxClientsCount;
        std::map<int, struct sockaddr_in> clients;

    public:
        /* Description:
        * set port for server and set a value of max queue 1
        *      for server;
        * 
        * ARGS:
        * port - the server port on which it is running;
        */
        Server(int port);
        /* Description:
        * set port for server and set a value of max queue 
        *      maxClientsCount for server;
        * 
        * ARGS:
        * port - the server port on which it is running;
        * maxClientsCount - the max count of clients on server;
        */
        Server(int port, unsigned short maxClientsCount);
        /* Description:
        * close all clients connection and close the server;
        */
        ~Server(void);
        
        void AcceptClientConnection(void);

    private:
        /* Description:
        * make socket, bind and listen for our server;
        * 
        * Errors:
        * outputs information about to the screen, if some POSIX errors
        * happened!;
        * 
        * Returns:
        * Upon successful completion, a value of EXIT_SUCCESS (0) is returned;
        * Otherwise return EXIT_FAILURE (1);
        */
        int MakeServerSocket(void);
    };
}

#endif // !SERVER_H