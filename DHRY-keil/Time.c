/******************************************************************************/
/* TIME.C: Time Functions for 100Hz Clock Tick                                */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2006 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

//#include <91M40800.H>                      /* AT91M40800 definitions          */

long timeval = 0;

/* Timer Counter 0 Interrupt executes each 10ms @ 40 MHz Crystal Clock        */
__irq void IRQ_Handler (void) {
  timeval++;
}


