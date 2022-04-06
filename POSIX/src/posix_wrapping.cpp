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
