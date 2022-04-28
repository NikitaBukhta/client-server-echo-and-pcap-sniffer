#include "sniffer.h"
#include "pcap_config.h"

unsigned int PCAP::Sniffer::sniffedPacketsCount = 0;

PCAP::Sniffer::Sniffer(const std::string& device, const std::string& filter)
{
    this->device = device;
    this->filter = filter;

    std::transform(this->filter.begin(), this->filter.end(), this->filter.begin(), ::tolower);

    this->handle = nullptr;

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
    std::transform(this->filter.begin(), this->filter.end(), this->filter.begin(), ::tolower);

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

    const struct sniffIP *ip;       // the IP header;
    const struct sniffTCP *tcp;     // the TCP header
    const struct sniffUDP *udp;     // the UDP headerl
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

        ip = (struct sniffIP*)(packet + SIZE_ETHERNET);
        sizeIP = IP_HL(ip) * 4;

        if (sizeIP < 20)
        {
            std::cout << "Invalid IP header size: " << sizeIP << " bytes" << std::endl;
            return;
        }

        std::cout << "Source IP: " << POSIX::_inetNtoa(ip->src) << std::endl;
        std::cout << "Destination IP: " << POSIX::_inetNtoa(ip->dst) << std::endl;

        if (filter == "tcp")
        {
            tcp = (struct sniffTCP*)(packet + SIZE_ETHERNET + sizeIP);
            sizeTCP = TH_OFF(tcp) * 4;
            
            if (sizeTCP < 20)
            {
                std::cout << "Invalid TCP header size: " << sizeTCP << " bytes" << std::endl;
                return;
            }

            std::cout << "Protocol TCP: " << std::endl;
            std::cout << "Source port: " << tcp->srcPort << std::endl;
            std::cout << "Destination port: " << tcp->dstPort << std::endl;
        }
        else if (filter == "udp")
        {
            udp = (struct sniffUDP*)(packet + SIZE_ETHERNET + sizeIP);

            std::cout << "Protocol UDP" << std::endl;
            std::cout << "Source port: " << udp->src << std::endl;
            std::cout << "Destination port: " << udp->dst << std::endl;
        }
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