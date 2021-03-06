#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include <string>

namespace client    // client-server
{
    class Client
    {
    private:
        struct sockaddr_in address;
        int serverSocket;

    public:
        Client(int port, char *IPv4);

        ~Client(void);

        /* Description:
         * Read message that server sent to the client. Max message length 
         * you can read is 1024;
         *
         * ARGS:
         * buffer - variable where we will save a read message;
         * 
         * Value record:
         * if function has worked successfully, record a string with message
         * content. Otherwise, empty string ("");
         * 
         * return values:
         * return size of readen message;
         */
        ssize_t readMessage(std::string& buffer) const;

        /* Description:
         * Send message to the server by a socket;
         * Output information with an error to the screen, if
         * client cannot send message to the client;
         *
         * ARGS:
         * message - message you want to send to the client;
         */
        void sendMessage(const std::string& msg) const;

        /* Description:
         * Enable keep alive checking connection between client and server;
         * 
         * ARGS:
         * serverSocket - socket we want to make keepalive checking;
         * interval - interval between checking alive connection in seconds;
         */
        void enableKeepalive(int serverSocket, int interval);
    };
}

#endif // !CLIENT_H