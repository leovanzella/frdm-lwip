/*
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef __LWIP_LWIOPTS_H__
#define __LWIP_LWIOPTS_H__

#define LINK_STATS      1
#define ETHARP_STATS    1
#define IPFRAG_STATS    1
#define IP_STATS        1
#define ICMP_STATS      1
#define IGMP_STATS      1
#define UDP_STATS       1
#define TCP_STATS       1
#define MEM_STATS       1
#define MEMP_STATS      1
#define SYS_STATS       1

#define ETH_MAX_FRAMELEN 1500

/* Align memory on 4 byte boundery (32-bit) */
#define MEM_ALIGNMENT                   4

/* No operating system present */
#define NO_SYS                          1

/* Time in milliseconds to perform ARP processing */
#define ETHARP_TMR_INTERVAL             5000

#endif /* __LWIP_LWIOPTS_H__ */
