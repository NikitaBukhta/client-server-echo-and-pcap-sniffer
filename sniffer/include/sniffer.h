#ifndef SNIFFER_H
#define SNIFFER_H

#include "pcap_wrapping.h"
#include "posix_wrapping.h"

#include <string>
#include <iostream>

namespace PCAP
{
    class Sniffer
    {
    private:
        std::string device;
        bpf_u_int32 mask;		            // The netmask of our sniffing device
	    bpf_u_int32 net;		            // The IP of our sniffing device
        std::string filter;
        struct bpf_program fp;  // The compiled filter expression

        pcap_t *handle;

    public:
        Sniffer(const std::string& device, const std::string& filter);

        Sniffer(const std::string& device);

        Sniffer(void);

        /* Description:
         * Set a new device name we want to track;
         *
         * ARGS:
         * device - new device you want to track;
         */
        void setDevice(const std::string& device);

        /* Description:
         * return a new device name we want to track;
         */
        std::string getDevice(void) const;

        void setFilter(const std::string& filter);

        std::string getFilter(void) const;

        void startSniffing(void);

    private:
        /* Description:
         * calculate count of non zero bits of a number;
         *
         * ARGS:
         * number - number you want to calculate;
         * 
         * Return values;
         * return count of non-zero bits;
         */
        unsigned short nonZeroBits(uint32_t number) const;

        void connectDevice(void);
    };
}

#endif // !SNIFFER_H