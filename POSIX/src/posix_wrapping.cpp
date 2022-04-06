#include "posix_wrapping.h"
#include "posixError.h"

#include <sys/socket.h>     // socket(), bind()
#include <errno.h>
#include <string>

/* socket wrapping;
 *
 * Description:
 * socket() creates an endpoint for communication and returns a descriptor.
 *  
 * ARGS:
 * domain - specifies a communications domain within which communication 
 *      will take place; this selects the protocol family which should be used.
 * type - specifies the semantics of communication.
 * protocol - specifies a particular protocol to be used with the socket.  
 *      Normally only a single protocol exists to support a particular socket 
 *      type within a given protocol family.  However, it is possible that many 
 *      protocols may exist, in which case a particular protocol must be specified 
 *      in this manner.  The protocol number to use is particular to the “communication 
 *      domain” in which communication is to take place; see protocols(5).
 * 
 * Return values:
 * The return value is a descriptor referencing the socket.
 * Otherwise throw PosixError with information about error.
 */
int Socket(int domain, int type, int protocol)
{
    int ret = socket(domain, type, protocol);

    if (ret == -1)
    {
        std::string error("Socket error: ");
        error += strerror(errno);
        
        throw PosixError(error.c_str());
    }

    return ret;
}

/* bind wrapping
 *
 * Description:
 * bind() assigns a name to an unnamed socket.  When a socket is created with socket(2) 
 * it exists in a name space (address family) but has no name assigned.  bind() requests 
 * that address be assigned to the socket.
 * 
 * Args:
 * socketFD - a socket that has been created with socket() and that you want to give the name;
 * address - address you want to give to the socket;
 * addressLength - sizeof(address);
 * 
 * Return values:
 * Upon successful completion, a value of 0 is returned.
 * Otherwise throw PosixError with information about error.
 */
int Bind(int socketFD, const struct sockaddr* address, socklen_t addressLength)
{
    int ret = bind(socketFD, address, addressLength);
    if (ret == -1)
    {
        std::string error("Bind error: ");
        error += strerror(errno);
        
        throw PosixError(error.c_str());
    }

    return ret;
}

/* listen wrapping
 *
 * Description
 * Creation of socket-based connections requires several operations.  
 * First, a socket is created with socket(2).  Next, a willingness to 
 * accept incoming connections and a queue limit for incoming connections 
 * are specified with listen().  Finally, the connections are accepted with 
 * accept(2).  The listen() call applies only to sockets of type SOCK_STREAM.
 * 
 * Args:
 * socketFD - a socket that has been created with socket(), bound to an address 
 *      with bind(2);
 * backlog - parameter defines the maximum length for the queue of pending 
 *      connections.  If a connection request arrives with the queue full, 
 *      the client may receive an error with an indication of ECONNREFUSED.  
 *      Alternatively, if the underlying protocol supports retransmission, 
 *      the request may be ignored so that retries may succeed;
 * 
 * Return values:
 * Upon successful completion, a value of 0 is returned.
 * Otherwise throw PosixError with information about error.
 */
int Listen(int socketFD, int backlog)
{
    int ret = listen(socketFD, backlog);
    if (ret == -1)
    {
        std::string error("Listen error: ");
        error += strerror(errno);
        
        throw PosixError(error.c_str());
    }

    return ret;
}

/* accept wrapping
 *
 * Description:
 * accept() extracts the first connection request on the queue of pending 
 * connections, creates a new socket with the same properties of socket, and 
 * allocates a new file descriptor for the socket. If no pending connections 
 * are present on the queue, and the socket is not marked as non-blocking, 
 * accept() blocks the caller until a connection is present.  If the socket is 
 * marked non-blocking and no pending connections are present on the queue, accept() 
 * returns an error as described below. The accepted socket may not be used to 
 * accept more connections.  The original socket socket, remains open.
 * 
 * Args:
 * socketFD - a socket that has been created with socket(), bound to an address with 
 *      bind(2), and is listening for connections after a listen(2);
 * address - address you want to give to the socket;
 * addressLength - sizeof(address);
 * 
 * Return values:
 * If it succeeds, it returns a non-negative integer that is a descriptor for the 
 *      accepted socket.
 * Otherwise throw PosixError with information about error.
 */
int Accept(int socketFD, sockaddr* address, socklen_t* addressLength)
{
    int ret = accept(socketFD, address, addressLength);
    if (ret == -1)
    {
        std::string error("Accept error: ");
        error += strerror(errno);
        
        throw PosixError(error.c_str());
    }

    return ret;
}
