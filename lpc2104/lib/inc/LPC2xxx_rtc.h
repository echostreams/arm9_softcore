/******************************************************************************
 * @file:    LPC2xxx_rtc.h
 * @purpose: Header File for using the Real Time Clock on LPC2xxx CPUs
 * @version: V1.0
 * @author:  Tymm Twillman
 * @date:    1. November 2010
 * @license: Simplified BSD License
 *
 * Notes:
 * - Does not handle powering up RTC interface on
 *   VPB bus; that will need to be done external to package
 *   to access the RTC.  Clock can run however on external 
 *   battery & crystal without bus power. 
 *
 * - Dates given must be normalized prior to calling; that
 *   is outside the scope of this package.  The RTC registers
 *   will take what is input and just increment registers
 *   as time passes, not verifying correctness.
 *
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
 
#ifndef LPC2XXX_RTC_H_
#define LPC2XXX_RTC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/

#include <stdint.h>
#include <time.h>   /* For struct tm */
#include "LPC2xxx.h"
#include "LPC2xxx_lib_assert.h"

#ifndef LPC2XXX_HAS_RTC
#error  Your CPU does not seem to have a Real Time Clock, or a CPU header file is missing/incorrect.  
#endif


/** @addtogroup RTC Real Time Clock Interface
  * @{
  */
  
/* Special Definitions ------------------------------------------------------*/

#ifdef RTC_CONFIG_LOCKING_PROVIDED

/** @brief Lock Mutex Preventing Simultaneous Access to Core RTC Registers
  * @param  None.
  * @return None.
  */
void RTC_Lock(void);

/** @brief Unlock Mutex Preventing Simultaneous Access to Core RTC Registers
  * @param  None.
  * @return None.
  */
void RTC_Unlock(void);

#else 

/* NOP Macros for Lock / Unlock functions */

# define RTC_Lock() do {} while(0)

# define RTC_Unlock() do {} while(0)

#endif


/* Types --------------------------------------------------------------------*/

/** @addtogroup RTC_Types Real Time Clock Interface Typedefs
  * @{
  */
  
/** @addtogroup RTC_Interval Real Time Clock Interval Masking Bits
  * @{
  */
#define RTC_Interval_Mask          (0xff)
#define RTC_Interval_Seconds       (1 << 0)
#define RTC_Interval_Minutes       (1 << 1)
#define RTC_Interval_Hours         (1 << 2)
#define RTC_Interval_DayOfMonth    (1 << 3)
#define RTC_Interval_DayOfWeek     (1 << 4)
#define RTC_Interval_DayOfYear     (1 << 5)
#define RTC_Interval_Month         (1 << 6)
#define RTC_Interval_Year          (1 << 7)

/**
  * @}
  */
  
/** @addtogroup RTC_IT_Mask Real Time Clock Interrupt Mask Bits
  * @{
  */
#define RTC_IT_Mask                (0x03)
#define RTC_IT_CounterIncrement    (1 << 0)
#define RTC_IT_Alarm               (1 << 1)

/**
  * @}
  */

/** @addtogroup RTC_Prescaler Real Time Clock Prescaler
  * @{
  */
typedef struct {
    uint16_t Int;  /*!< The Integer Portion (13 bits)                          */
    uint16_t Frac; /*!< The Fractional Portion (15 bits)                       */
} RTC_Prescaler_Type;

#define RTC_Prescaler_Integer_Mask  (0x1fff)
#define RTC_Prescaler_Fraction_Mask (0x7fff)
/**
  * @}
  */

/** @addtogroup RTC_Clock_Source Real Time Clock Input Clock Sources
  * @{
  */
typedef enum {
    RTC_ClockSource_Prescaler = 0x00,
    RTC_ClockSource_Xtal32K,
} RTC_ClockSource_Type;
#define RTC_IS_CLOCK_SOURCE(Source)  (((Source) == RTC_ClockSource_Prescaler) \
                                   || ((Source) == RTC_ClockSource_Xtal32k))

/**
  * @}
  */

/**
  * @}
  */

/* Inline Functions ---------------------------------------------------------*/

/** @addtogroup RTC_Inline_Functions Real Time Clock Interface Inline Functions
  * @{
  */

/** @brief Get Pending Interrupts on the Real Time Clock
  * @param  None.
  * @return A Bitmask with the Interrupts Pending on the RTC
  */
__INLINE static uint8_t RTC_GetPendingIT(void)
{
    return RTC->ILR;
}

/** @brief Clear Specified Pending Interrupts on the Real Time Clock
  * @param  IT               The Interrupts to Clear
  * @return None.
  */
__INLINE static void RTC_ClearPendingIT(uint8_t IT)
{
    lpc2xxx_lib_assert((IT & ~RTC_IT_Mask) == 0);
    
    RTC->ILR = IT;
}

/** @brief Enable Interrupts for Updates on Given Interval Counters
  * @param  UpdateMask       A Bitmask Specifying Interval Counters 
  * @return None.
  */
__INLINE static void RTC_EnableIntervalUpdateITs(uint8_t UpdateMask)
{
    lpc2xxx_lib_assert((UpdateMask & ~RTC_Interval_Mask) == 0);
    
    RTC->CIIR |= UpdateMask;
}
    
/** @brief Disable Interrupts for Updates on Given Interval Counters
  * @param  UpdateMask       A Bitmask Specifying Interval Counters 
  * @return None.
  */
__INLINE static void RTC_DisableIntervalUpdateITs(uint8_t UpdateMask)
{
    lpc2xxx_lib_assert((UpdateMask & ~RTC_Interval_Mask) == 0);
    
    RTC->CIIR &= ~UpdateMask;
}
    
/** @brief Get a Bitmask Specifying Which Interval Counters will Trigger Update Interrupts
  * @param  None.
  * @return A Bitmask Specifying those Interval Counters which will Trigger Interrupts on Updates
  */
__INLINE static uint8_t RTC_GetEnabledIntervalUpdateITs(void)
{
    return RTC->CIIR;
}

/** @brief Enable Given Interval Counters for Alarm Comparison
  * @param  UpdateMask       A Bitmask Specifying Interval Counters 
  * @return None.
  */
__INLINE static void RTC_EnableAlarmIntervals(uint8_t Intervals)
{
    lpc2xxx_lib_assert((Intervals & ~RTC_Interval_Mask) == 0);
    
    RTC->AMR |= Intervals;
}
    
/** @brief Disable Given Interval Counters for Alarm Comparison
  * @param  UpdateMask       A Bitmask Specifying Interval Counters 
  * @return None.
  *
  * All 1's (0xff) means disable alarms completely.
  */
__INLINE static void RTC_DisableAlarmIntervals(uint8_t Intervals)
{
    lpc2xxx_lib_assert((Intervals & ~RTC_Interval_Mask) == 0);
    
    RTC->AMR &= ~Intervals;
}
    
/** @brief Get a Bitmask Specifying Which Interval Counters will Trigger Alarm Interrupts on Match
  * @param  None.
  * @return A Bitmask Specifying those Interval Counters which will Trigger Interrupts on Matches
  */
__INLINE static uint8_t RTC_GetEnabledAlarmIntervals(void)
{
    return RTC->AMR;
}

/** @brief Get the Current Value of the Clock Tick Counter
  * @param  None.
  * @return The Current Clock Tick Counter Value
  *
  * The Clock Tick Counter counts from 0 to 32767; on rollover, the seconds counter is updated.
  *  Based on the prescaler & input clock settings, the CTC intervals periods may not be
  *  consistent.  The register is RO.
  */
__INLINE static uint16_t RTC_GetClockTickCounter(void)
{
    return RTC->CTC;
}


/** @brief Set the Real TIme Clock's Prescaler Values (Integer & Fractional Portions)
  * @param  Prescaler        A Pointer to an RTC_Prescaler_Type Specifying Int/Frac Parts of Prescaler
  * @return None.
  */
__INLINE static void RTC_SetPrescaler(RTC_Prescaler_Type *Prescaler)
{
    lpc2xxx_lib_assert(Prescaler != NULL);
    lpc2xxx_lib_assert((Prescaler->Int & ~RTC_Prescaler_Int_Mask) == 0);
    lpc2xxx_lib_assert((Prescaler->Frac & ~RTC_Prescaler_Frac_Mask) == 0);
    
    RTC_Lock();
    RTC->PREINT = Prescaler->Int;
    RTC->PREFRAC = Prescaler->Frac;
    RTC_Unlock();
}
    
/** @brief Get the Real TIme Clock's Current Prescaler Values (Integer & Fractional Portions)
  * @param  Prescaler        A Pointer to an RTC_Prescaler_Type Specifying Int/Frac Parts of Prescaler
  * @return the Prescaler Pointer that was Passed in
  */
__INLINE static RTC_Prescaler_Type *RTC_GetPrescaler(RTC_Prescaler_Type *Prescaler)
{
    lpc2xxx_lib_assert(Prescaler != NULL);
    
    RTC_Lock();
    Prescaler->Int = RTC->PREINT;
    Prescaler->Frac = RTC->PREFRAC;
    RTC_Unlock();
    
    return Prescaler;
}
    
/** @brief Set the Input Clock Source for the Real Time Clock
  * @param  Source           The Source to Use (of RTC_ClockSource_Type)
  * @return None.
  */
__INLINE static void RTC_SetClockSource(RTC_ClockSource_Type Source)
{
    lpc2xxx_lib_assert(RTC_IS_CLOCK_SOURCE(Source));
    
    if (Source == RTC_ClockSource_Xtal32K) {
        RTC_Lock();
        RTC->CCR |= RTC_CLKSRC;
        RTC_Unlock();
    } else {
        RTC_Lock();
        RTC->CCR &= ~RTC_CLKSRC;
        RTC_Unlock();
    }
}

/** @brief Get the Current Input Clock Source for the Real Time Clock
  * @param  None.
  * @return The Source to Used (of RTC_ClockSource_Type)
  */
__INLINE static RTC_ClockSource_Type RTC_GetClockSource(RTC_ClockSource_Type Source)
{
    return (RTC->CCR & RTC_CLKSRC) ? RTC_ClockSource_Xtal32K : RTC_ClockSource_Prescaler;
}

/** @brief Clear Core RTC Registers to 0 (for Sanity)
  * @param  None.
  * @return None.
  */
__INLINE static void RTC_Clear(void)
{
    RTC->CIIR = 0;   /* No IRQs on Updates       */
    RTC->AMR = 0xff; /* No IRQs on Alarm Matches */
    RTC->CCR = 0;
}

/**
  * @}
  */

/* External Functions ---------------------------------------------------------*/

/** @defgroup RTC_Functions Real Time Clock Exported Functions
  * @{
  */

/** @brief  Set the Real Time Clock
  * @param  tm               A Pointer to a struct tm Value Specifying New Time Values
  * @return None.
  */
void RTC_SetClock(const struct tm *tm);

/** @brief  Get the Current Time from the Real Time Clock
  * @param  tm               A Pointer to a struct tm Value that will be Filled In with Current Values
  * @return The same struct tm Value Passed In
  */
struct tm *RTC_GetClock(struct tm *tm);

/** @brief  Set the Real Time Clock's Alarm
  * @param  tm               A Pointer to a struct tm Value Specifying New Alarm Time Values
  * @return None.
  */
void RTC_SetAlarm(const struct tm *tm);

/** @brief  Get the Current Alarm Time from the Real Time Clock
  * @param  tm               A Pointer to a struct tm Value that will be Filled In with Current Values
  * @return The same struct tm Value Passed In
  */
struct tm *RTC_GetAlarm(struct tm *tm);

/**
  * @}
  */

/**
  * @}
  */


#ifdef __cplusplus
};
#endif

#endif /* #ifndef LPC2XXX_RTC_H_ */
