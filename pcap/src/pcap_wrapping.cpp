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

int _pcap_setfilter(pcap_t *p, struct bpf_program *fp)
{
    int ret = pcap_setfilter(p, fp);

    if (ret == PCAP_ERROR)
        throw PcapError(pcap_geterr(p));

    return ret;
}

int _pcap_compile(pcap_t *p, struct bpf_program *fp, const char *str, int optimize, bpf_u_int32 netmask)
{
    int ret = pcap_compile(p, fp, str, optimize, netmask);

    if (ret == PCAP_ERROR)
        throw PcapError(pcap_geterr(p));

    return ret;
}

int _pcap_lookupnet(const char *device, bpf_u_int32 *netp, bpf_u_int32 *maskp)
{
    char errBuf[PCAP_BUF_SIZE];

    int ret = pcap_lookupnet(device, netp, maskp, errBuf);

    if (ret == PCAP_ERROR)
        throw PcapError(errBuf);

    return ret;
}

const u_char* _pcap_next(pcap_t *p, struct pcap_pkthdr *h)
{
    const u_char *ret = pcap_next(p, h);

    if (ret == nullptr)
        throw PcapError(pcap_geterr(p));

    return ret;
}