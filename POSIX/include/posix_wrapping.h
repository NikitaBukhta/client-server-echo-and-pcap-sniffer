#ifndef POSIX_WRAPPING_H
#define POSIX_WRAPPING_H

#include "posixError.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>

namespace POSIX
{
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
    int _socket(int domain, int type, int protocol);

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
    int _bind(int socketFD, const struct sockaddr* address, socklen_t addressLength);

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
    int _listen(int socketFD, int backlog);

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
    int _accept(int socketFD, struct sockaddr* address, socklen_t* addressLength);

    /* inet_pton wrapping
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
    int _inetPton(int family, const char *src, void* destination);

    /* connect wrapping 
     *
     * Descrition:
     * The parameter socketFD is a socket.  If it is of type SOCK_DGRAM, 
     * this call specifies the peer with which the socket is to be associated; 
     * this address is that to which datagrams are to be sent, and the only 
     * address from which datagrams are to be received.  If the socket is of 
     * type SOCK_STREAM, this call attempts to make a connection to another socket.  
     * The other socket is specified by address, which is an address in the communications 
     * space of the socket.
     * 
     * Args:
     * socketFD - socket;
     * address - address you want to give to the socket;
     * addressLength - sizeof(address);
     * 
     * Return values:
     * Upon successful completion, a value of 0 is returned.
     * Otherwise throw PosixError with information about error.
     */
    int _connect(int socketFD, const struct sockaddr * address, socklen_t addressLength);

    /* setsockopt wraping
     *
     * Desctiption:
     * The setsockopt() function shall set the option specified by the option_name 
     * argument, at the protocol level specified by the level argument, to the value 
     * pointed to by the option_value argument for the socket associated with the file 
     * descriptor specified by the socket argument.
     *  
     * ARGS:
     * socketFD - a descriptor that identifies a socket;
     * level - The level argument specifies the protocol level at which the option 
     *      resides. To set options at the socket level, specify the level argument 
     *      as SOL_SOCKET. To set options at other levels, supply the appropriate level 
     *      identifier for the protocol controlling the option. 
     *      For example, to indicate that an option is interpreted by the TCP, set level 
     *      to IPPROTO_TCP as defined in the <netinet/in.h> header;
     * optionName - the socket option for which the value is to be set (for example, 
     *      SO_BROADCAST). The optname parameter must be a socket option defined within 
     *      the specified level, or behavior is undefined;
     * optionValue - A pointer to the buffer in which the value for the requested option 
     *      is specified;
     * optionLength - sizeof(optionValue);
     * 
     * Return values:
     * Upon successful completion, a value of 0 is returned.
     * Otherwise throw PosixError with information about error.
     */
    int _setsockopt(int socketFD, int level, int optionName, const void  *optionValue, socklen_t optionLength);

    /* inet_ntoa wrapping
     *
     * Desctiption:
     * converts the Internet host address in, given in network byte order, to a string 
     * in IPv4 dotted-decimal notation. The string is returned in a statically 
     * allocated buffer, which subsequent calls will overwrite.
     * 
     * ARGS:
     * in - IP address we want to convert;
     * 
     * Return values:
     * 
     * If no error occurs, inet_ntoa returns a character pointer to a static buffer 
     * containing the text address in standard "." notation.
     * Otherwise throw PosixError with information about error.
     */
    char* _inetNtoa(const struct in_addr& in);

    /* read warpping
     *
     * Description:
     * Attempts to read nbyte bytes of data from the object referenced by the 
     * socketFD fildes into the buffer pointed to by buf.
     * 
     * ARGS:
     * socketFD - from which descriptor we read message;
     * buf - buffer, where we write data to the buffer;
     * nbyte - max count of byte we can read;
     * 
     * Return values:
     * If successful, the number of bytes actually read is returned.  Upon 
     * reading end-of-file, 0 is returned;
     * Otherwise throw PosixError with information about error;
     */
    ssize_t _read(int socketFD, void *buf, size_t nbyte);

    /* write wrapping
     *
     * Description:
     * write() attempts to write nbyte of data to the object referenced 
     * by the socketFD fildes from the buffer pointed to by buf.
     * 
     * ARGS:
     * socketFD - to which descriptor we send message;
     * buf - buffer, where we write data to the socketFD;
     * nbyte - max count of byte we can read;
     * 
     * Return values:
     * Upon successful completion the number of bytes which were written
     * is returned.
     * Otherwise throw PosixError with information about error;
     */
    ssize_t _write(int socketFD, const void *buf, size_t nbyte);

    /* select wrapping
     *
     * Description:
     * select() examines the I/O descriptor sets whose addresses are 
     * passed in readfds, writefds, and errorfds to see if some of 
     * their descriptors are ready for reading, are ready for writing, 
     * or have an exceptional condition pending, respectively.  The 
     * first nfds descriptors are checked in each set; i.e., the 
     * descriptors from 0 through nfds-1 in the descriptor sets are 
     * examined.  (Example: If you have set two file descriptors "4" and 
     * "17", nfds should  not be "2", but rather "17 + 1" or "18".)  On 
     * return, select() replaces the given descriptor sets with subsets 
     * consisting of those descriptors that are ready for the requested 
     * operation.  select() returns the total number of ready descriptors 
     * in all the sets.
     *
     * ARGS:
     * socketFD - This argument should be set to the highest-numbered file
     *      descriptor in any of the three sets, plus 1.  The
     *      indicated file descriptors in each set are checked, up to
     *      this limit (but see BUGS).
     * readfds - The file descriptors in this set are watched to see if
     *      they are ready for reading.  A file descriptor is ready
     *      for reading if a read operation will not block; in
     *      particular, a file descriptor is also ready on end-of-file.
     * writefds - The file descriptors in this set are watched to see if
     *      they are ready for writing.  A file descriptor is ready
     *      for writing if a write operation will not block.  However,
     *      even if a file descriptor indicates as writable, a large
     *      write may still block.
     * errorfds - The file descriptors in this set are watched for
     *      "exceptional conditions".
     * 
     * Return values:
     * return true if socket is ready;
     * return false if not;
     */
    bool _select(int socketFD, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout);

    /* send wrapping
     *
     * Description:
     * The send() function shall initiate transmission of a message from 
     * the specified socket to its peer. The send() function shall send 
     * a message only when the socket is connected (including when the 
     * peer of a connectionless socket has been set via connect()).
     * 
     * ARGS:
     * socket - specifies the socket file descriptor we send message.
     * buffer - points to the buffer containing the message to send.
     * length - specifies the length of the message in bytes.
     * flags - Specifies the type of message transmission. Values of 
     *      this argument are formed by logically OR'ing zero or more 
     *      of the following flags:
     *      - MSG_EOR - terminates a record (if supported by the protocol).
     *      - MSG_OOB - Sends out-of-band data on sockets that support 
     *          out-of-band communications. The significance and semantics 
     *          of out-of-band data are protocol-specific.
     * 
     * Retutrn values
     * Upon successful completion, send() shall return the number of bytes 
     * sent. 
     * Otherwise throw PosixError with information about error;
     */
    ssize_t _send(int socket, const void *buffer, size_t length, int flags);

    /* fcntl wrapping
     *
     * Description:
     * fcntl() provides for control over descriptors;
     * 
     * ARGS:
     * The argument fildes is a descriptor to be operated on by cmd;
     * 
     * Return values
     * Upon successful completion, the value returned depends on cmd.
     * Otherwise throw the exception;
     */
    int _fcntl(int fildes, int cmd, ...);
};

#endif // !POSIX_WRAPPING_H