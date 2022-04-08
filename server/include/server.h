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
        
        /* Description:
         * make connection between new client and server. If server is not
         * full, client added to clients list. Otherwise, server send message
         * to new client with infomation about server overflow.
         */
        void acceptClientConnection(void);

        /* Description: 
         * Return client IP address in dotted-decimal notation. The string is 
         * returned in a statically allocated buffer, which subsequent calls 
         * will overwrite;
         *
         * ARGS:
         * clientSocket - socket of client we want to get IP address;
         * 
         * Return values:
         * If no errors happened, returns client IP-address.
         * Otherwise nullptr;
         */
        char* getClientIP(int clientSocket);

        /* Description:
         * Read message that client sent to the server. Max message length 
         * you can read is 256;
         *
         * ARGS:
         * clientSocket - from which socket you want to read message;
         * 
         * Return values:
         * return buffer with message contains, if success.
         * Otherwile return nullptr and output error message;
         */
        char* readMessage(int clientSocket);

        char* readMessage(void);

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
        int makeServerSocket(void);
    };
}

#endif // !SERVER_H