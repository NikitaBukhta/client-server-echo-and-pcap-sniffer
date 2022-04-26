#include "pcap_wrapping.h"
#include <pcapError.h>

using namespace PCAP;

pcap_t* _pcap_open_live(const char *device, int snaplen, int prmisc, int to_ms)
{
    char errBuf[PCAP_BUF_SIZE];
    pcap_t *handle;

    if ((handle = pcap_open_live(device, snaplen, prmisc, to_ms, errBuf)) == nullptr)
        throw PcapError(errBuf);

    return handle;
}