/****************************************************************************
 * @file:    LPC2xxx_wdt.h
 * @purpose: LPC2xxx Watchdog Timer Interface Header File
 * @version: V1.0
 * @author:  Tymm Twillman
 * @date:    1. June 2010
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

#ifndef LPC2XXX_WDT_H_
#define LPC2XXX_WDT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------*/

#include <stdint.h>
#include <stddef.h>
#include "LPC2xxx.h"
#include "LPC2xxx_lib_assert.h"

#ifndef LPC2XXX_HAS_WDT
#error  Your CPU does not seem to have a Watchdog Timer, or a CPU header file is missing/incorrect.
#endif


/** @defgroup WDT Watchdog Timer Peripheral
  * @{
  */

/* Register Defines --------------------------------------------------------*/


/** @defgroup WDT_Feed_Values Watchdog Timer Feed Values
  * @{
  */

#define WDT_Feed_A  (0xAA) /*!< Feed value first to start WDT/change reload  */
#define WDT_Feed_B  (0x55) /*!< Feed value second to start WDT/change reload */

/**
  * @}
  */

/* Inline Functions ---------------------------------------------------------*/

/** @defgroup WDT_Inline_Functions Watchdog Timer Inline Functions
  * @{
  */

/** @brief Enable the watchdog timer
  * @param  None.
  * @return None.
  */
__INLINE static void WDT_Enable(void)
{
    WDT->MOD |= WDT_WDEN;
}

/** @brief  Determine Whether the Watchdog Timer is Enabled
  * @param  None.
  * @return 1 if the WDT is enabled, 0 otherwise
  */
__INLINE static uint32_t WDT_IsEnabled(void)
{
    return (WDT->MOD & WDT_WDEN) ? 1:0;
}

/** @brief  Enable WDT Resetting the Chip (cannot be cleared w/o reset)
  * @param  None.
  * @return None.
  */
__INLINE static void WDT_EnableChipReset(void)
{
   WDT->MOD |= WDT_WDRESET;
}

/** @brief  Determine Whether the Watchdog Chip Reset is Enabled
  * @param  None.
  * @return 1 if the WDT Chip Reset is enabled, 0 otherwise
  */
__INLINE static uint32_t WDT_ChipResetIsEnabled(void)
{
    return (WDT->MOD & WDT_WDRESET) ? 1:0;
}

/** @brief Check Whether the Watchdog Timer has Timed Out
  * @param  None.
  * @return 1 if the timer has timed out, 0 otherwise.
  */
__INLINE static uint8_t WDT_TimedOut(void)
{
    return (WDT->MOD & WDT_WDTOF) ? 1:0;
}

 /** @brief Check whether the watchdog timer interrupt has been triggered
   * @param  None.
   * @return 1 if the timer interrupt is pending, 0 otherwise
   */
__INLINE static uint8_t WDT_ITIsPending(void)
{
    return ((WDT->MOD & WDT_WDINT) ? 1:0);
}

/** @brief Get the current value of the watchdog timer
  * @param None.
  * @return Current value of the watchdog timer
  */
__INLINE static uint32_t WDT_GetCurrentValue(void)
{
    return WDT->TV;
}

/** @brief Feed the Watchdog Timer (load the timer constant)
  * @param  None.
  * @return None.
  *
  * This is necessary to load the current value in the Timer Constant
  *  register as the watchdog's Timer Reload value.  This is also necessary
  *  to start the watchdog, after it's been enabled.
  */
__INLINE static void WDT_Feed(void)
{
    __asm__ __volatile__ (
        "       mov     r0, %0        \r\n"
        "       mov     r1, #0xaa     \r\n"
        "       mov     r2, #0x55     \r\n"
        "       str     r1, [r0, %1] \r\n"
        "       str     r2, [r0, %1] \r\n"
        :
        : "r" ((uint32_t)WDT), "i" (offsetof(WDT_Type, FEED))
        :"r0", "r1", "r2"
    );
}

/** @brief Set the watchdog's Timeout Constant (value that gets reloaded)
  * @param  Timeout  Number of watchdog ticks before the watchdog times out
  * @return None.
  */
__INLINE static void WDT_SetTimeout(uint32_t Timeout)
{
    WDT->TC = Timeout;
}

/** @brief Get the reload value of the watchdog timer
  * @param None.
  * @return Reload value of the timer
  */
__INLINE static uint8_t WDT_GetTimeout(void)
{
    return WDT->TC;
}

/**
  * @}
  */

/**
  * @}
  */


#ifdef __cplusplus
};
#endif

#endif /* #ifndef LPC2XXX_WDT_H_ */
