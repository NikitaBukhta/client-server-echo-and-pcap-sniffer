#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>     // struct sockaddr_in
#include <map>
#include <vector>

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
         * you can read is 1024;
         *
         * ARGS:
         * clientSocket - from which socket you want to read message;
         * 
         * Value record:
         * if function has worked successfully, record a string with message
         * content. Otherwise, empty string ("");
         */
        ssize_t readMessage(char *buffer, int clientSocket);

        /* NOT TESTED!!!
         *
         * DESCRIPTION:
         * read the first message that was sent to the server.
         * Function works until the server didn't receive the message;
         * Max message length you can read is 1024;
         */
        ssize_t readMessage(char *buffer);

        /* Description:
         * Write all clients' sockets to the vector;
         *
         * ARGS:
         * clientSockets - array, which contains clients' sockets;
         */
        void getClientSockets(std::vector<int>& clientSockets);

        /* Description:
         * Send message to the specific client by a socket;
         * Output information with an error to the screen, if
         * server cannot send message to the client;
         *
         * ARGS:
         * message - message you want to send to the client;
         * clientSocket - socket of the client you want to 
         *      send the message;
         */
        void sendMessage(const char *message, int clientSocket);

        /* Description:
         * Send message to the all clients by a socket;
         * Output information with an error to the screen, if
         * server cannot send message to the client (output 
         * socket number and error message. Does not stop the
         * work);
         *
         * ARGS:
         * message - message you want to send to the client;
         */
        void sendMessage(const char *message);

        /* Description:
         * add prefix to your message and the message start 
         * or at the end;
         *
         * ARGS:
         * message - message you want to modify;
         * prefix - some string you want to merge with message;
         * modifyAtStart - if true, add prefix in the message start.
         *      Otherwise, modify message at the end;
         */
        void modifyMessage(char *message, char *prefix, bool modifyAtStart);

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