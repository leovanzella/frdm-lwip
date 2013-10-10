#include "spi.h"

/*  Setup the spi for 8 bit data, high steady state clock,
    second edge capture, with a 8MHz clock rate 					
		spi.format(8,0) POL=0 PHA = 0;
    spi.frequency(8000000);
		*/
void spi_init(void)  
{  
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;     					  /* Turn on clock to D module */ 
	SIM->SCGC4 |= SIM_SCGC4_SPI0_MASK;      					  /* Enable SPI0 clock */ 
	PORTD->PCR[0] = PORT_PCR_MUX(0x2);      					  /* Set PTD0 to mux 2 [SPI0_PCS0] */
	PORTD->PCR[1] = PORT_PCR_MUX(0x2);      					  /* Set PTD1 to mux 2 [SPI0_SCK] */
	PORTD->PCR[2] = PORT_PCR_MUX(0x2);      					  /* Set PTD2 to mux 2 [SPI0_MOSI] */
	PORTD->PCR[3] = PORT_PCR_MUX(0x2);      					  /* Set PTD3 to mux 2 [SPIO_MISO] */

	SPI0->C1 = SPI_C1_MSTR_MASK;    /* Set SPI0 to Master & SS pin to auto SS */

	SPI0->BR = (SPI_BR_SPPR(0x00) | SPI_BR_SPR(0x07)); /* Set baud rate prescale divisor to 3 & set baud rate divisor to 64 for baud rate of 15625 hz */

	SPI0->C1 |= SPI_C1_SPE_MASK;    										/* Enable SPI0 */
	
	//PORTB->PCR[0] = (1UL <<  8); 
	FPTD->PDOR = 1;            /* switch Blue LED off  */
  FPTD->PDDR = 1;            /* enable PTD1 as Output */
	
	PORTD->PCR[0] = 0x00000105;
	PORTD->PCR[1] = 0x00000205;
	PORTD->PCR[2] = 0x00000205;
	PORTD->PCR[3] = 0x00000205;
	PORTD->PCR[4] = 0x00000103;
}  
  
uint8_t spi_send(char spiMsg)  
{
	volatile int i;
	uint8_t result;
	while(!(SPI_S_SPTEF_MASK & SPI0->S))
	{      
		__asm
		{
			NOP /* While buffer is not empty do nothing */
		}
	}  
	SPI0->D = spiMsg;     /* Write char to SPI */
	
	while(!(SPI_S_SPRF_MASK & SPI0->S))
	{      
		__asm
		{
			NOP /* While buffer is not empty do nothing */
		}
	}	
	return(SPI0->D & 0xff);
}

void spi_recv(char spiMsg)  
{  
	while(!(SPI_S_SPTEF_MASK & SPI0->S))
	{      
		__asm
		{
		NOP /* While buffer is not empty do nothing */
		}
	}  
	spiMsg = SPI0->D;     /* Read char from SPI */
}