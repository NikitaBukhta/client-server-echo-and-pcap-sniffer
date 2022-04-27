#ifndef PCAP_WRAPPING_H
#define PCAP_WRAPPING_H

#include "pcapError.h"

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
     * Otherwise throw an PcapError;
     */
    pcap_t* _pcap_open_live(const char *device, int snaplen, int prmisc, int to_ms);

    /* pcap_setfilter wrapping;
     *
     * Description:
     * set the filter. Is used to specify a filter program.
     * 
     * ARGS:
     * p - handle;
     * fp - a pointer to a bpf_program, usually the result of a call to pcap_compile(3PCAP);
     * 
     * Return values:
     * returns 0 on success.
     * Otherwise throw an PcapError;
     */
    int _pcap_setfilter(pcap_t *p, struct bpf_program *fp);

    /* pcap_compile wrapping;
     *
     * Description:
     * Compile a filter expression. pcap_compile() is used to compile the string str 
     * into a filter program.
     * 
     * ARGS:
     * p - handle;
     * fp - a pointer to a bpf_program struct ans is filled in by this function;
     * str - program filter. See pcap-filter(7) for the syntax of that string.
     * optimize - controls whether optimization on the resulting code is performed.
     * nemask - specifies the IPv4 netmask of the network on which packets are being
     *      captured. It is used only when checking for IPv4 broadcast address in the
     *      filter program. If netmask of the network on which packets are being captured,
     *      is not known to the program, or if packets are being captured on the Linux
     *      "any" preuso-interface that can capture on more that one network, a value of
     *      PCAP_NETMASK_UNKNOWN can be supplied. Test for IPv4 broadcast addresses will 
     *      fail to compile, but all other tests in the filter program will be OK.
     * 
     * NOTE:
     * in libpcap 1.8.0 and later, pcap_compile() can be used in multiple threads within a 
     * single process.  However, in earlier versions of libpcap, it is not safe to use 
     * pcap_compile() in multiple threads in a single process without some form of mutual 
     * exclusion allowing only one thread to call it at any given time.
     * 
     * Return values:
     * returns 0 on success.
     * Otherwise throw an PcapError;
     */
    int _pcap_compile(pcap_t *p, struct bpf_program *fp, const char *str, int optimize, 
            bpf_u_int32 netmask);

    /* pcap_lookupnet wrapping;
     *
     * Desctiption:
     * find the IPv4 network number and netmask for a device. Is used to determine the IPv4
     * network number and mask associated with the network device.
     * 
     * ARGS:
     * device - network device;
     * netp - IPv4 address (will be written);
     * maskp - mask of network (will be written);
     * 
     * Return values:
     * returns 0 on success.
     * Otherwise throw an PcapError;
     */
    int _pcap_lookupnet(const char *device, bpf_u_int32 *netp, bpf_u_int32 *maskp);

    /* pcap_next wrapping
     *
     * Desctiption:
     * reads the next packet (by calling pcap_dispatch() with a cnt of 1) and returns a u_char
     * pointer to the data in that packet. The packet data is not to be freed by the caller, 
     * and is not guaranteed to be value after the next call to pcap_next_ex(), pcap_next(), 
     * pcap_loop() or pcap_dispatch(); if the code nneds it to remain valid, it must make a copy
     * of it.
     * 
     * ARGS:
     * p - handle;
     * h - filter in with the appropriate values for the packet;
     * 
     * Return values:
     * returns a pointer to the packet data on success;
     * If an error occured or no packets were read from a live capture, throws an PcapError;
     */
    const u_char* _pcap_next(pcap_t *p, struct pcap_pkthdr *h);
}

#endif // !PCAP_WRAPPING_H