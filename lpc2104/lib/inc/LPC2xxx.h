/******************************************************************************
 * @file:    LPC2xxx.h
 * @purpose: Header File for including proper headers for a LPC2xxx chip
 * @version: V1.0
 * @author:  Tymm Twillman
 * @date:    1. November 2010
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
 
#ifndef LPC2XXX_H_
#define LPC2XXX_H_

/* Includes -----------------------------------------------------------------*/

#if defined(lpc2138) || defined(lpc2136) || defined(lpc2134) \
    || defined(lpc2132) || defined(lpc2131)
# include "LPC2138.h"
#elif defined(lpc2103) || defined(lpc2102) || defined(lpc2101)
# include "LPC2103.h"
#elif defined(lpc2106) || defined(lpc2105) || defined(lpc2104)
# include "LPC2106.h"
#else
# error "No Header File Found.  Did you -D<chip> (e.g. -Dlpc2138) ?"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/* Exported Variables -------------------------------------------------------*/

/*! @brief Frequency of the System's Input Clock */
extern uint32_t ClockSource;

/*! @brief Frequency of the MCU Core */
extern uint32_t SystemFrequency;

/*! @brief Frequency of the APB Bus  */
extern uint32_t SystemAPBFrequency;


/* Exported Functions -------------------------------------------------------*/

/** @brief  Initialize the system to known state, set up system clocks.
  * @param  None.
  * @return None.
  */
extern void SystemInit(void);

/** @brief  Update SystemCoreClock to match current hardware configuration
  * @param  None.
  * @return None.
  *
  * Checks system clocking registers to determine current CPU core clock speed.
  */
extern void SystemCoreClockUpdate(void);

#ifdef __cplusplus
};
#endif

#endif /* #ifndef LPC2XXX_H_ */
