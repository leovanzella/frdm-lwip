/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: Freescale MKL25Z128xxx4 LED Flasher
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/
#include <MKL25Z4.H>
#include <string.h>

volatile uint32_t msTicks;                            /* counts 1ms timeTicks */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;                        /* increment counter necessary in Delay() */
}

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

static struct netif  netif_eth0_data;
static struct netif* netif_eth0 = &netif_eth0_data;

static struct ip_addr my_ipaddr_data;
static struct ip_addr my_netmask_data;
static struct ip_addr my_gw_data;

static u32_t         last_arp_time;
static u32_t         last_tcpslow_time;
static u32_t         last_tcpfast_time;
static u32_t         last_led_time;
static u32_t         last_led_state;

static char webpage[] =
	"HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n\
	<html><head><title>Congrats!</title></head><body><h1>Welcome to our lwIP HTTP server!</h1><p>This is a small test page.</body></html>";

static void lwip_init(void)
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

  netif_add(netif_eth0, &my_ipaddr_data, &my_netmask_data, &my_gw_data, NULL, enc28j60_if_init, ip_input);
 
	netif_set_default(netif_eth0);

	netif_set_up(netif_eth0);
}

err_t http_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
	char* rq;

	if (p != NULL)
	{
		rq = p->payload;

		if (rq[0] == 'G' && rq[1] == 'E' && 
		    rq[2] == 'T' && rq[3] == ' ' &&
		    rq[4] == '/')
		{
			u32_t  bytestosend;
			u32_t* newarg;

			bytestosend = sizeof(webpage);

			newarg = (u32_t*) mem_malloc(sizeof(u32_t));
			*newarg = bytestosend;	

			if (tcp_write(tpcb, webpage, sizeof(webpage), 0) != ERR_OK)
			{
				mem_free(newarg);
				tcp_close(tpcb);				
			}
			else
			{
				tcp_arg(tpcb, newarg);
			}
		}

		tcp_recved(tpcb, p->len);
		pbuf_free(p);		
	}

	return ERR_OK;
}

err_t http_poll_callback(void *arg, struct tcp_pcb *tpcb)
{
	if (arg != NULL)
	{
		u32_t bytestosend = *((u32_t*) arg);

		if (bytestosend == 0)
		{
			mem_free(arg);
			tcp_arg(tpcb, NULL);
			tcp_close(tpcb);
		}
	}

	return ERR_OK;
}

err_t http_send_callback(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	u32_t bytestosend;

	if (arg != NULL)
	{
		bytestosend = *((u32_t*) arg);

		if ((bytestosend - len) >= 0)
			bytestosend -= len;
		else
			bytestosend = 0;

		*((u32_t*) arg) = bytestosend;
	}

	return ERR_OK;
}

err_t http_accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	tcp_arg(newpcb, NULL);
	tcp_recv(newpcb, http_recv_callback);
	tcp_sent(newpcb, http_send_callback);
	tcp_poll(newpcb, http_poll_callback, 1);

	return ERR_OK;
}

static void http_init(void)
{
	struct tcp_pcb* tcpweb;
	struct tcp_pcb* tcpweb_listen;

	tcpweb = tcp_new();
	if (tcpweb == NULL)
		return;

	/* Bind to port 80 for any address */
	if (tcp_bind(tcpweb, IP_ADDR_ANY, 80) != ERR_OK)
		return;

	tcpweb_listen = tcp_listen(tcpweb);
	if (tcpweb_listen == NULL)
	{
		tcp_abort(tcpweb);
		tcpweb = NULL;
		return;
	}	

	tcpweb = tcpweb_listen;

	tcp_accept(tcpweb, http_accept_callback);
}

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
  int num     = -1; 
  int dir     =  1;
	
  SystemCoreClockUpdate();                      /* Get Core Clock Frequency */
  SysTick_Config(SystemCoreClock/1000);         /* Generate interrupt each 1 ms    */

	lwip_init();
	http_init();

	TICKER_Start();  

  while(1) 
	{
		enc28j60_if_input(netif_eth0);
        tcp_tmr();
  }
}
