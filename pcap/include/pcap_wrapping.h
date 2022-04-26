#ifndef PCAP_WRAPPING_H
#define PCAP_WRAPPING_H

#include <pcap.h>

namespace PCAP
{
    /* pcap_open_live wrapping;
     *
     * Description:
     * Is used to obtain a packet capture handle to look at packets on the network.
     * 
     * ARGS:
     * device - is a string that specifies the network device to open; on Linux systems 
     *      with 2.2 or later kernels, a device argument of "any" orNULL can be used to 
     *      capture packets from all interfaces.
     * snaplen - specifies the snapshot length to be set on the handle.
     * promisc - specifies if the interface is to be put into promiscuous mode.
     * to_ms - specifies the packet buffer timeout, as a non-negative value, in 
     *      milliseconds. (See pcap(3PCAP) for an explanation of the packet buffer 
     *      timeout.)
     * 
     * Return values:
     * pcap_open_live() returns a pcap_t * on success.
     * Otherwise throw an error;
     */
    pcap_t* _pcap_open_live(const char *device, int snaplen, int prmisc, int to_ms);
}

#endif // !PCAP_WRAPPING_H