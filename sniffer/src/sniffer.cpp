#include "sniffer.h"
#include "pcap_config.h"

PCAP::Sniffer::Sniffer(const std::string& device, const std::string& filter)
{
    this->device = device;
    this->filter = filter;

    this->handle = nullptr;

    sniffedPacketsCount = 0;

    connectDevice();
}

PCAP::Sniffer::Sniffer(const std::string& device) : Sniffer(device, "") {}

PCAP::Sniffer::Sniffer(void) : Sniffer("") {}

PCAP::Sniffer::~Sniffer(void)
{
    pcap_close(handle);
}

void PCAP::Sniffer::setDevice(const std::string& device)
{
    this->device = device;

    connectDevice();
}

std::string PCAP::Sniffer::getDevice(void) const
{
    return device;
}

void PCAP::Sniffer::setFilter(const std::string& filter)
{
    this->filter = filter;

    if (handle != nullptr)
    {
        try
        {
            PCAP::_pcap_compile(handle, &fp, filter.c_str(), 1, net);
            PCAP::_pcap_setfilter(handle, &fp);
        }
        catch(const PcapError& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

std::string PCAP::Sniffer::getFilter(void) const
{
    return filter;
}

void PCAP::Sniffer::sniff(void)
{
    struct pcap_pkthdr header;

    const struct sniff_ip *ip;      // the IP header;
    const struct sniff_tcp *tcp;    // the TCP header
	const char *payload;            // Packet payload 

    /* I do not use std::string, because std::string need char,
     * but unsigned char is needed. So if we make the cast to char,
     * some errors may happen.
     */
    const u_char *packet;
    u_int sizeIP;
	u_int sizeTCP;

    try
    {
        packet = PCAP::_pcap_next(handle, &header);

        std::cout << "Packet # " << ++sniffedPacketsCount << ":" << std::endl;

        if (header.len != header.caplen)
            std::cout << "Warning! Captured size is different that packet size: " << header.len << std::endl;
        else
            std::cout << "Packet size: " << header.len << " byts" << std::endl;

        ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
        sizeIP = IP_HL(ip) * 4;

        if (sizeIP < 20)
        {
            std::cout << "Invalid IP header size: " << sizeIP << " bytes" << std::endl;
            return;
        }

        tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + sizeIP);
        sizeTCP = TH_OFF(tcp) * 4;
        
        if (sizeTCP < 20)
        {
            std::cout << "Invalid TCP header size: " << sizeTCP << " bytes" << std::endl;
            return;
        }

        std::cout << "Source port: " << tcp->th_sport << std::endl;
        std::cout << "Destination port: " << tcp->th_dport << std::endl;

        std::cout << "Source IP: " << POSIX::_inetNtoa(ip->ip_src) << std::endl;
        std::cout << "Source IP: " << POSIX::_inetNtoa(ip->ip_dst) << std::endl;
    }
    catch(const std::exception& e) {}
}

unsigned short PCAP::Sniffer::nonZeroBits(uint32_t number) const
{
    unsigned short nonZeroBitsCount = 0;

    while (number > 0)
    {
        if (number & 1)
            ++nonZeroBitsCount;
        
        number >>= 1;
    }

    return nonZeroBitsCount;
}

void PCAP::Sniffer::connectDevice(void)
{
    try
    {
        PCAP::_pcap_lookupnet(device.c_str(), &net, &mask);
        handle = PCAP::_pcap_open_live(device.c_str(), BUFSIZ, 1, 1000);
        PCAP::_pcap_compile(handle, &fp, filter.c_str(), 1, net);
        PCAP::_pcap_setfilter(handle, &fp);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return;
    }

    struct in_addr IPv4 = {net};
    std::cout << "Now we are listen to " << device << " (" << POSIX::_inetNtoa(IPv4) 
            << " / " << nonZeroBits(mask) << ")" << std::endl;
}