#include "sniffer.h"

PCAP::Sniffer::Sniffer(const std::string& device, const std::string& filter)
{
    this->device = device;
    this->filter = filter;

    this->handle = nullptr;

    connectDevice();
}

PCAP::Sniffer::Sniffer(const std::string& device) : Sniffer(device, "") {}

PCAP::Sniffer::Sniffer(void) : Sniffer("") {}

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