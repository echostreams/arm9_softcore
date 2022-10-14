/******************************************************************************
 * @file:    LPC2xxx_lib_assert.c
 * @purpose: Assert function for library debugging
 * @version: V1.0
 * @author:  Tymm Twillman
 * @date:    31. December 2011
 * @license: Simplified BSD License
 ******************************************************************************
 * Copyright (c) 2012, Timothy Twillman
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 * 
 *    1. Redistributions of source code must retain the above copyright notice,
 *        this list of conditions and the following disclaimer.
 * 
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY TIMOTHY TWILLMAN ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL <COPYRIGHT HOLDER> ORCONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, ORCONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of Timothy Twilllman.
 *****************************************************************************/

/* Includes -----------------------------------------------------------------*/

#include "LPC2xxx.h"
#include "LPC2xxx_wdt.h"
#include "LPC2xxx_lib_assert.h"


/* Functions ----------------------------------------------------------------*/

/** @brief  Simple assert() function for library debugging
  *
  * @return Does not return (infinite loop)
  *
  * This assert function is separate from the regular assert() so that
  * the library can be debugged independently of other code.  It disables
  * system interrupts so that other things don't keep running & possibly create
  * confusion.
  *
  * It is declared weak for easy overriding by user-defined functions.
  *
  */
void lpc2xxx_lib_assert_function(void) __attribute__((weak));
void lpc2xxx_lib_assert_function(void)
{
    __disable_irq();
    
    while(1) {
#ifdef LPC2XXX_LIB_ASSERT_TICKLE_WATCHDOG
        WDT_Reset();
#endif
    }
}
