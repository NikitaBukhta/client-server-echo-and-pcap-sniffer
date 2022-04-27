#include "server.h"
#include "posix_wrapping.h"
#include "pcap_wrapping.h"

#include <stdexcept>        // default exceptions;
#include <unistd.h>         // STDOUT_FILENO;
#include <string>
#include <iostream>         // cout, endl;
#include <thread>
#include <chrono>

#define DEVICE "en0"

#define PREFIX "Server echo: "

using namespace server;

// count of connections;
static int connectionCount = 0;

/* Description:
 * Desconnect the client from the server and rewrite content from
 * vector (remove disconnected client);
 * 
 * ARGS:
 * client - client socket;
 * server - server where we remove the client;
 */
void makeDisconnect(int client, Server& server);

/* Desctiption:
 * With function contains interaction between client and server;
 *
 * ARGS:
 * clientSocket - socket of clients with which server interacts;
 * server - our server to which clients connected;
 * clientVector - list of clients that connected to the server. 
 *      That is needed for disconnecting the clients;
 */
void clientCommunication(int clientSocket, Server& server);

/* Description:
 * sniffing the packets from the specific device;
 * 
 * ARGS:
 * device - the device we are sniffing;
 */
void pcapSniffing(const std::string& device);

int calculateMaskLength(uint32_t address);

int main(int argc, char **argv)
{
    if (argc < 2)
        throw std::invalid_argument("You must pass the port number when starting "
            "the server!");

    int port = atoi(argv[1]);

    static Server server(port, 2);

    bool connectionMade = false;    /* this var is need in order to 
                                     * do not close the server, if
                                     * any connections have't made
                                     * earlier;
                                     */
    
    int newClient = 0;

    pcapSniffing(DEVICE);

    while (true)
    {  

        if((newClient = server.acceptClientConnection()) != 0)
        {
            connectionMade = true;
            ++connectionCount;
            // new thread for a new server;
            std::thread clientThread(clientCommunication, newClient, std::ref(server));
            clientThread.detach();
        }

        /* if one more connection was made and now there are no connection,
         * just end the loop and turn off the server;
         */
        if (connectionMade && (connectionCount == 0))
        {
            std::cout << "Finish work!" << std::endl;
            break;
        }
    }

    return 0;
}

void makeDisconnect(int client, Server& server)
{
    std::cout << "Disconnect socket " << client << " (" << server.getClientIP(client) << ")" << std::endl;
    --connectionCount;

    server.disconnectClient(client);
}

void clientCommunication(int clientSocket, Server& server)
{
    static unsigned short iterationForCheck = 20;
    unsigned short currentIteration = 0;
    std::string msg;

    // if nothing is read, return 0;
    while (true)
    {
        ++currentIteration;

        if (server.readMessage(msg, clientSocket))
        {
            std::cout << "Message from socket " << clientSocket << " (" 
                << server.getClientIP(clientSocket) << "): " << msg << std::endl;
            
            msg = PREFIX + msg + '\0';
            server.sendMessage(msg, clientSocket);

            // if we have got the message, reset the number of iterations to zero;
            currentIteration = 0;
        }
        else if (currentIteration >= iterationForCheck)
        {
            makeDisconnect(clientSocket, server);
            return;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void pcapSniffing(const std::string& device)
{
    bpf_u_int32 mask;		            // The netmask of our sniffing device
	bpf_u_int32 net;		            // The IP of our sniffing device
    struct bpf_program fp;              // The compiled filter expression
    std::string filter = "tcp";    // sniffing only tcp and udp packets;


    char *dev;  // temp;
    try
    {
        PCAP::_pcap_lookupnet(device.c_str(), &net, &mask);
        pcap_t *handle = PCAP::_pcap_open_live(device.c_str(), BUFSIZ, 1, 1000);
        PCAP::_pcap_compile(handle, &fp, filter.c_str(), 1, net);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return;
    }

    struct in_addr IPv4 = {net};
    struct in_addr IPMask = {mask};
    std::cout << "Now we are listen to " << device << " (" << POSIX::_inetNtoa(IPv4) 
            << " / " << calculateMaskLength(mask) << ")" << std::endl;
}

int calculateMaskLength(uint32_t address) 
{
    int maskLength = 0;

    while (address > 0)
    {
        if (address & 1)
            ++maskLength;
        
        address >>= 1;
    }

    return maskLength;
}