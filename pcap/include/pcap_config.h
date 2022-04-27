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
#define IP_HL(ip)   (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)    (((ip)->ip_vhl) >> 4)
#define TH_OFF(th)  (((th)->th_offx2 & 0xf0) >> 4)

// ---------------------------------- TYPEDEFS ---------------------------------- //
typedef u_int tcp_seq;

// --------------------------------- Structures --------------------------------- //
namespace PCAP
{
    // Ethernet header 
    struct sniff_ethernet
    {
        u_char ether_dhost[ETHER_ADDR_LEN];     // Destination host address; 
        u_char ether_shost[ETHER_ADDR_LEN];     // Source host address; 
        u_short ether_type;                     // IP? ARP? RARP? etc;
    };

    // IP header
    struct sniff_ip 
    {
        u_char ip_vhl;		    // version << 4 | header length >> 2;
        u_char ip_tos;		    // type of service;
        u_short ip_len;		    // total length;
        u_short ip_id;		    // identification; 
        u_short ip_off;		    // fragment offset field;
        u_char ip_ttl;		    // time to live;
        u_char ip_p;		    // protocol;
        u_short ip_sum;		    // checksum;
        struct in_addr ip_src;  // source address;
        struct in_addr ip_dst;  // destination address;
    };

    // TCP header
    struct sniff_tcp 
    {
        u_short th_sport;	// source port;
        u_short th_dport;	// destination port;
        tcp_seq th_seq;		// sequence number;
        tcp_seq th_ack;		// acknowledgement number;
        u_char th_offx2;	// data offset, rsvd;
        u_char th_flags;
        u_short th_win;		// window;
        u_short th_sum;		// checksum;
        u_short th_urp;		// urgent pointer;
    };
}

#endif // !PCAP_CONFIG_H