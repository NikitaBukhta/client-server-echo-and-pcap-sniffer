#ifndef PCAP_ERROR_H
#define PCAP_ERROR_H

#include <exception>
#include <string>

namespace PCAP
{
    class PcapError : public std::exception
    {
    private:
        std::string errorMessage;

    public:
        PcapError(const std::string& msg);

        virtual const char* what(void) const noexcept override;
    };
}

#endif // !PCAP_ERROR_H