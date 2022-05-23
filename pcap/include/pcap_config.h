// I have got that here:
//      https://engrsalmanshaikh.wordpress.com/2014/12/09/network-packet-sniffer-c/

#include "pcap.h"

#ifndef PCAP_CONFIG_H
#define PCAP_CONFIG_H

/* Ethernet addresses are 6 bytes */
#define ETHER_ADDR_LEN	6
/* ethernet headers are always exactly 14 bytes */
#define SIZE_ETHERNET 14

// --------------------------------- IP defines --------------------------------- //
#define IP_RF 0x8000		// reserved fragment flag 
#define IP_DF 0x4000		// dont fragment flag 
#define IP_MF 0x2000		// more fragments flag 
#define IP_OFFMASK 0x1fff	// mask for fragmenting bits 

// --------------------------------- TH defines --------------------------------- //
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)

// ----------------------------------- MACROS ----------------------------------- //
#define IP_HL(ip)   (((ip)->vhl) & 0x0f)
#define IP_V(ip)    (((ip)->vhl) >> 4)
#define TH_OFF(th)  (((th)->offx2 & 0xf0) >> 4)

// ---------------------------------- TYPEDEFS ---------------------------------- //
typedef u_int tcp_seq;

// --------------------------------- Structures --------------------------------- //
namespace PCAP
{
    // Ethernet header;
    struct sniffEthernet
    {
        u_char ether_dhost[ETHER_ADDR_LEN];     // Destination host address; 
        u_char ether_shost[ETHER_ADDR_LEN];     // Source host address; 
        u_short ether_type;                     // IP? ARP? RARP? etc;
    };

    // IP header;
    struct sniffIP 
    {
        u_char vhl;		        // version << 4 | header length >> 2;
        u_char tos; 		    // type of service;
        u_short len;		    // total length;
        u_short id;		        // identification; 
        u_short offset;		    // fragment offset field;
        u_char ttl;		        // time to live;
        u_char protocol;        // protocol;
        u_short sum;		    // checksum;
        struct in_addr src;     // source address;
        struct in_addr dst;     // destination address;
    };

    // TCP header;
    struct sniffTCP 
    {
        u_short srcPort;	// source port;
        u_short dstPort;	// destination port;
        tcp_seq seq;		// sequence number;
        tcp_seq ack;		// acknowledgement number;
        u_char offx2;	    // data offset, rsvd;
        u_char flags;
        u_short win;		// window;
        u_short sum;		// checksum;
        u_short urp;		// urgent pointer;
    };

    // UDP header;
    struct sniffUDP
    {
        u_short src;
        u_short dst;
        u_short length;
        u_short checksum;
    };
}

#endif // !PCAP_CONFIG_H