#include "pcapError.h"

using namespace PCAP;

PcapError::PcapError(const std::string& msg)
{
    errorMessage = msg;
}

const char* PcapError::what(void) const noexcept
{
    return errorMessage.c_str();
}