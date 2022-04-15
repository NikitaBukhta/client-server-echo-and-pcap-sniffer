#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>     // struct sockaddr_in
#include <map>
#include <vector>
#include <string>

namespace cs    // client-server
{
    class Server
    {
    private:
        struct sockaddr_in address = {0};
        int serverSocket;
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
        *       maxClientsCount for server;
        *       set waiting for client feedback 10 seconds;
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
         * 
         * Return values:
         * return true if connection was made;
         */
        bool acceptClientConnection(void);

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
         * buffer - variable where we will save a read message;
         * clientSocket - from which socket you want to read message;
         * 
         * Value record:
         * if function has worked successfully, record a string with message
         * content. Otherwise, empty string ("");
         * 
         * return values:
         * return size of readen message;
         */
        ssize_t readMessage(std::string& buffer, int clientSocket);

        /* Description:
         * Write all clients' sockets to the vector;
         *
         * ARGS:
         * clientSockets - array, which contains clients' sockets;
         */
        void getClientSockets(std::vector<int>& clientSockets);

        void getClientSockets(std::map<int, unsigned short>& clientSockets);

        /* Description:
         * Send message to the specific client by a socket;
         * Output information with an error to the screen, if
         * server cannot send message to the client;
         *
         * ARGS:
         * msg - message you want to send to the client;
         * clientSocket - socket of the client you want to 
         *      send the message;
         */
        void sendMessage(const std::string& msg, int clientSocket);

        /* Description:
         * Send message to the all clients by a socket;
         * Output information with an error to the screen, if
         * server cannot send message to the client (output 
         * socket number and error message. Does not stop the
         * work);
         *
         * ARGS:
         * msg - message you want to send to the client;
         */
        void sendMessage(const std::string& msg);

        // return server socket;
        int getServerSocket(void);

        /* Desctiption:
         * Disconect the client from the server. Recomend to rewrite or
         * delete element from your clients vector if you get that.
         * 
         * ARGS:
         * clientSocket - socket of client you want to remove;
         */
        void disconnectClient(int clientSocket);

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

        /* Description:
         * Enable keep alive checking connection between client and server;
         * 
         * ARGS:
         * clientSocket - socket we want to make keepalive checking;
         * interval - interval between checking alive connection in seconds;
         */
        void enableKeepalive(int clientSocket, int interval);
    };
}

#endif // !SERVER_H