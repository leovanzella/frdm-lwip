#include "lwip_high_level.h"

static struct ip_addr my_ipaddr_data;
static struct ip_addr my_netmask_data;
static struct ip_addr my_gw_data;

static struct netif  netif_eth0_data;
static struct netif* netif_eth0 = &netif_eth0_data;

static u32_t         last_arp_time;
static u32_t         last_tcpslow_time;
static u32_t         last_tcpfast_time;
static u32_t         last_led_time;
static u32_t         last_led_state;

static char webpage[] =
	"HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n\
	<html><head><title>Congrats!</title></head><body><h1>Welcome to our lwIP HTTP server!</h1><p>This is a small test page.</body></html>";

void lwip_init(void)
{
	stats_init();

	sys_init();
  
	mem_init();

	memp_init();

	pbuf_init();

	etharp_init();

	ip_init();

	udp_init();

	tcp_init();

	IP4_ADDR(&my_ipaddr_data,  192, 168, 1, 189);
	IP4_ADDR(&my_netmask_data, 255, 255, 255, 0);
	IP4_ADDR(&my_gw_data, 192, 168, 1, 1);

	netif_add(netif_eth0, &my_ipaddr_data, &my_netmask_data, &my_gw_data, NULL, ethernetif_init, ip_input);

	netif_set_default(netif_eth0);

	netif_set_up(netif_eth0);
}