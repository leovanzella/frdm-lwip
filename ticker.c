/*****************************************************************************
 *
 * Project          : lwIP Web
 * Subproject       : 
 * Name             : ticker.c
 * Function         : Routines for ticker
 * Designer         : K. Sterckx
 * Creation date    : 22/07/2007
 * Compiler         : GNU ARM
 * Processor        : LPC23xx
 * Last update      :
 * Last updated by  :
 * History          :
 *
 *****************************************************************************
 *
 *  Routines that start ticker
 *
 ****************************************************************************/

#include "ticker.h"

/***************************************************************************** 
 *
 * Local defines
 *
 *****************************************************************************/

/***************************************************************************** 
 *
 * Local variables
 *
 *****************************************************************************/

/***************************************************************************** 
 *
 * Global variables
 *
 *****************************************************************************/

volatile unsigned int TICKER_time;

/***************************************************************************** 
 *
 * Local functions
 *
 *****************************************************************************/

/***************************************************************************** 
 *
 * Global functions
 *
 *****************************************************************************/

/***************************************************************************** 

   Function name : TICKER_Init

   Description   :

    This function initialize the ticker.
    We want a 1ms resolution

    PCLK is 72MHz.
    Timer Freq = PCLK / (T0PR * T0MR1) = 1000Hz

    T0PR  = 60
		T0MR1 = 1000

   Parameters    : NONE

   Returns       : NONE

*/
void TICKER_Init(void)
{
//	PCLKSEL0 = (PCLKSEL0 & (~(0x3<<2))) | (0x01 << 2);

//	T0TCR  = 2;         /* Stop and reset the timer */
//	T0CTCR = 0;         /* Timer mode               */
//	T0PR   = 72;        /* Prescaler                */
//	T0MR1  = 1000;      /* Match value              */
//	T0MCR  = (3 << 3);  /* Reset timer on match and generate interrupt */
}

/***************************************************************************** 

   Function name : TICKER_Start

   Description   :

    This function starts the ticker.

   Parameters    : NONE

   Returns       : NONE

*/
void TICKER_Start(void)
{
	TICKER_time  = 0;
	
//	T0TCR        = 1;    /* Start timer */
}

void TICKER_Handler(void)
{
	/* 1ms */
	TICKER_time  += 1;

//	T0IR        = 2;
//	VICVectAddr = 0;
}
