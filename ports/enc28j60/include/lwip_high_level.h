#ifndef __LWIP_HIGH_LEVEL_H__
#define __LWIP_HIGH_LEVEL_H__

// lwIP API Header Files
#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/netifapi.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/tcpip.h"
#include "lwip/sockets.h"
#include "lwip/mem.h"
#include "netif/etharp.h"
#include "lwip/autoip.h"
#include "lwip/dhcp.h"
#include "lwip/init.h"

#include <string.h>



#include "ticker.h"

#include "lwip\opt.h"
#include "lwip\stats.h"
#include "lwip\sys.h"
#include "lwip\pbuf.h"
#include "lwip\udp.h"
#include "lwip\tcp.h"
#include "lwip\tcp_impl.h"

#include "netif\etharp.h"

#include "arch\ethernetif.h"
#include "arch\clock.h"



#include "netif/enc28j60if.h"

// Defines for setting up the system clock.
#define SYSTICKHZ               (F_CPU / 262144UL)
#define SYSTICKMS               (1000UL / SYSTICKHZ)
#define SYSTICKUS               (1000000UL / SYSTICKHZ)
#define SYSTICKNS               (1000000000UL / SYSTICKHZ)


#define IPADDR_USE_STATIC       0
#define IPADDR_USE_DHCP         1
#define IPADDR_USE_AUTOIP       2

extern struct netif lwip_netif;


extern void lwip_init(void);
         
#endif //__LWIP_HIGH_LEVEL_H__