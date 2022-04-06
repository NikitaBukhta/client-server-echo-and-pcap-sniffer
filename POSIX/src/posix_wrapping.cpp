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

/* inet_pton wraping
 *
 * Description:
 * The inet_pton() function converts a presentation format address (that is, 
 * printable form as held in a character string) to network format (usually 
 * a struct in_addr or some other internal binary representation, in network 
 * byte order).  It returns 1 if the address was valid for the specified 
 * address family, or 0 if the address was not parseable in the specified address 
 * family, or -1 if some system error occurred (in which case errno will have been set).  
 * This function is presently valid for AF_INET and AF_INET6.
 * 
 * family - The address family. The values currently supported are AF_INET and AF_INET6.
 * src - A pointer to the NULL-terminated string that contains the text representation of 
 *      the IP address to convert to numeric binary form.
 *      When the Family parameter is AF_INET, then the src parameter must point 
 *          to a text representation of an IPv4 address in standard dotted-decimal notation.
 *      When the Family parameter is AF_INET6, then the src parameter must point to 
 *          a text representation of an IPv6 address in standard notation.
 * destination - A pointer to a buffer in which to store the numeric binary representation 
 *      of the IP address. The IP address is returned in network byte order.
 *      When the Family parameter is AF_INET, this buffer should be large enough to hold 
 *          an IN_ADDR structure.
 *      When the Family parameter is AF_INET6, this buffer should be large enough to hold 
 *          an IN6_ADDR structure.
 * 
 * Return values:
 * If no error occurs, the InetPton function returns a value of 1 and the buffer pointed 
 *      to by the destination parameter contains the binary numeric IP address in network byte 
 *      order.
 * The InetPton function returns a value of 0 if the pAddrBuf parameter points to a string 
 *      that is not a valid IPv4 dotted-decimal string or a valid IPv6 address string. 
 * Otherwise, a value of -1 is returned, and a specific error code can be retrieved by calling 
 *      the WSAGetLastError for extended error information.
 */
int InetPton(int family, const char* src, void* destination)
{
    int res = inet_pton(family, src, destination);

    if (res == 0)
    {

        std::string error("int_pton failed: src doesn't contain a charapter"
            " string representing a valid network address in the specified"
            " address family.");

        throw PosixError(error.c_str());
    }
    else if (res == -1)
    {
        std::string error("Accept error: ");
        error += strerror(errno);
        
        throw PosixError(error.c_str());  
    }

    return res;
}
