
#ifndef _STDAFX_H
#define _STDAFX_H

#include "types.h"
#include "ethernet.h"
#include "ip.h"
#include "arp.h"
#include "dhcp.h"
#include "udp.h"
#include "tftp.h"

ULONG local_ip;
ULONG server_ip;
ULONG netmask;
ULONG sid;

extern char  net_in_data[1536];
extern char  net_out_data[1536];
extern int	 iReceiveData;
extern int   iSendData;
extern const UCHAR mac_addr[6];

struct NETWORKFRAME
{
    struct ETHERHDR eth_hdr;
    struct IPHDR    ip_hdr;
    struct UDPHDR   udp_hdr;
    union
    {
        struct TFTPHDR  tftp_hdr;
        struct DHCPHDR  dhcp_hdr;
    };
}__attribute__((packed));

extern int EtherOutput(UCHAR *dmac,char* pdata, USHORT len);
extern int EtherInput(USHORT tms);
extern void NetInit();

#endif /* _STDAFX_H_ */