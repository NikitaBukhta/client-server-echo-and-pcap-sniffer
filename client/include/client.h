#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>

namespace cs    // client-server
{
    class Client
    {
    private:
        struct sockaddr_in address;
        int serverSocket;

    public:
        Client(int port, char *IPv4);

        ~Client(void);

        void readMessage(char *buffer);

        void sendMessage(char *message);
    };
}

#endif // !CLIENT_H