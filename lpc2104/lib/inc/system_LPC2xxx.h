/******************************************************************************
 * @file:    system_LPC2xxx.h
 * @purpose: Header File for LPC2000 series chip system functions
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
 
#ifndef SYSTEM_LPC2XXX_H_
#define SYSTEM_LPC2XXX_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/

#include <stdint.h>
#include "LPC2xxx.h"


/* Exported Variables -------------------------------------------------------*/

/** @defgroup System_Variables System Utility Variables
  * @{
  */

/*! @brief Frequency of the System's Input Clock */
extern uint32_t ClockSource;

/*! @brief Frequency of the MCU Core */
extern uint32_t SystemCoreClock;

/**
  * @}
  */


/* Exported Functions -------------------------------------------------------*/

/** @defgroup System_Functions System Utility Functions
  * @{
  */

/** @brief  Initialize the system to known state, set up system clocks.
  * @param  None.
  * @return None.
  */
extern void SystemInit(void);

/** @brief  Update SystemCoreClock to match current clock configuration
  * @param  None.
  * @return None.
  *
  * To be called after updates to the clock configuration.
  */
extern void SystemCoreClockUpdate(void);

/** @brief  Configure the System PLL with the given scaler.
  *
  * @param  [in]  pll_scaler  Scaler value to write to the system PLL
  *
  * @return None.
  *
  * Note: PLL_ScalerCalc (include LPC2xxx_pll.h) can be used
  *  to calculate the proper pll scaler value.
  */
void SysPLL_Config(int8_t pll_scaler);

#ifdef LPC2XXX_HAS_USB

/** @brief  Configure the USB PLL with the given scaler.
  *
  * @param  [in]  pll_scaler  Scaler value to write to the USB pll
  *
  * @return None.
  *
  * Note: PLL_ScalerCalc() (include LPC2xxx_pll.h) can be used
  *  to calculate the proper pll scaler value.
  */
void USBPLL_Config(int8_t pll_scaler);

#endif

/**
  * @}
  */


#ifdef __cplusplus
};
#endif

#endif /* #ifndef SYSTEM_LPC2XXX_H_ */
