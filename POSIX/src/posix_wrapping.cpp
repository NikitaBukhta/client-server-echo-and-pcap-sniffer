#include "posix_wrapping.h"

#include <sys/socket.h>     // socket()
#include <stdio.h>          // perror()
#include <stdlib.h>         // exit()

/* socket wrapping
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
 */
int Socket(int domain, int type, int protocol)
{
    int ret = socket(domain, type, protocol);

    if (ret == -1)
    {
        perror("socket error: ");
        exit(EXIT_FAILURE);
    }

    return ret;
}