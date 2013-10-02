/*****************************************************************************
 *
 * Project          : lwIP Web
 * Subproject       : 
 * Name             : ticker.h
 * Function         : Routines for ticker
 * Designer         : K. Sterckx
 * Creation date    : 22/07/2007
 * Compiler         : GNU ARM
 * Processor        : LPC2132
 * Last update      :
 * Last updated by  :
 * History          :
 *
 *****************************************************************************
 *
 * Contains prototypes for code in ticker.c
 *
 ****************************************************************************/

#ifndef __TICKER_H
#define __TICKER_H

/*
 * VARIABLES
 */
extern volatile unsigned int TICKER_time;

/*
 * FUNCTIONS
 */
extern void     TICKER_Init(void);
extern void     TICKER_Start(void);

extern void     TICKER_Handler(void) __attribute__ ((interrupt));

#endif /* __TICKER_H */
