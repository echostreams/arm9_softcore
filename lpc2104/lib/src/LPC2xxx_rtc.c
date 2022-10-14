/******************************************************************************
 * @file:    LPC2xxx_rtc.c
 * @purpose: Functions for using the Real Time Clock on LPC2xxx CPUs
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

/* Includes -----------------------------------------------------------------*/

#include <stdint.h>
#include <time.h>

#include "LPC2xxx.h"
#include "LPC2xxx_rtc.h"
#include "LPC2xxx_lib_assert.h"


/* Locally Used Types -------------------------------------------------------*/

/** @addtogroup RTC_CTIME0 Layout of RTC's CTIME0 Register (DOW/HH/MM/SS)
  * @{
  */
typedef struct {
    uint32_t packed;
    struct {
        uint8_t sec;
        uint8_t min;
        uint8_t hour;
        uint8_t dow;
    } unpacked;
} RTC_CTIME0_Type;

/**
  * @}
  */

/** @addtogroup RTC_CTIME1 Layout of RTC's CTIME1 Register (DOM/MON/YEAR)
  * @{
  */
typedef struct {
    uint32_t Packed;
    struct {
        uint8_t dom;
        uint8_t month;
        uint16_t year;
    } unpacked;
} RTC_CTIME1_Type;

/**
  * @}
  */

/** @addtogroup RTC_CTIME2 Layout of RTC's CTIME2 Register (DOY)
  * @{
  */

typedef struct {
    uint32_t packed;
    struct {
        uint16_t doy;
        uint16_t Reserved;
    } unpacked;
} RTC_CTIME2_Type;

/**
  * @}
  */


/* Functions ----------------------------------------------------------------*/

/** @brief  Set the Real Time Clock
  * @param  tm    Pointer to a struct tm value specifying new time values
  * @return None.
  */
void RTC_SetClock(const struct tm *tm)
{
    lpclib_assert(tm->tm_sec < 60);
    lpclib_assert(tm->tm_min < 60);
    lpclib_assert(tm->tm_hour < 24);
    lpclib_assert(tm->tm_mday < 32);
    lpclib_assert(tm->tm_mon < 13);
    lpclib_assert(tm->tm_year < 4096);
    lpclib_assert(tm->tm_wday < 8);
    lpclib_assert(tm->tm_yday < 367);
    
    RTC_Lock();

    /* Reset the RTC */
    RTC->CCR |= RTC_CTCRST;
    
    /* Set the RTC Time Values */
    RTC->SEC =   tm->tm_sec;
    RTC->MIN =   tm->tm_min;
    RTC->HOUR =  tm->tm_hour;
    RTC->DOM =   tm->tm_mday;
    RTC->MONTH = tm->tm_mon;
    RTC->YEAR =  tm->tm_year + 1900;
    RTC->DOW =   tm->tm_wday;
    RTC->DOY =   tm->tm_yday;
    
    /* De-assert Reset, Enable RTC */
    RTC->CCR = (RTC->CCR & ~RTC_CTCRST) | RTC_CLKEN;
    
    RTC_Unlock();    
}


/** @brief  Get the Current Time from the Real Time Clock
  * @param  tm    Pointer to a struct tm value that will be filled
  * @return The same struct tm Value Passed In
  */
struct tm *RTC_GetClock(struct tm *tm)
{
    uint32_t hms;
    uint32_t hms2;
    uint32_t my;
    uint32_t doy;
    
    /* Use the CTIME registers as they make it easier to detect a rollover */
    
reget:
    RTC_Lock();
    hms2 = RTC->CTIME0;
    doy = RTC->CTIME2;
    my = RTC->CTIME1;
    hms = RTC->CTIME0;
    RTC_Unlock();

    /* Make sure no updates have occurred that could cause the values between
     *  registers to be out of sync
     */
    if (hms != hms2) {
        goto reget;
    }
    
    /* CTIME0 */
    tm->tm_sec   = (hms >> RTC_CSEC_Shift) & RTC_CSEC_Mask;
    tm->tm_min   = (hms >> RTC_CMIN_Shift) & RTC_CMIN_Mask;
    tm->tm_hour  = (hms >> RTC_CHOUR_Shift) & RTC_CHOUR_Mask;
    tm->tm_wday  = (hms >> RTC_CDOW_Shift) & RTC_CDOW_Mask;
    
    /* CTIME1 */
    tm->tm_mday  = (my >> RTC_CDOM_Shift) & RTC_CDOM_Mask;
    tm->tm_mon   = (my >> RTC_CMON_Shift) & RTC_CMON_Mask;
    tm->tm_year  = ((my >> RTC_CYEAR_Shift) & RTC_CYEAR_Mask) - 1900;
    
    /* CTIME2 */
    tm->tm_yday = (my >> RTC_DOY_Shift) & RTC_DOY_Mask;

    /* This layer doesn't deal with DST settings */
    tm->tm_isdst = 0;
    
    return tm;
}


/** @brief  Set the Real Time Clock's Alarm
  * @param  tm    Pointer to a struct tm value specifying new alarm time values
  * @return None.
  */
void RTC_SetAlarm(const struct tm *tm)
{
    uint32_t tempAMR;
    
    
    lpclib_assert(tm->tm_sec < 60);
    lpclib_assert(tm->tm_min < 60);
    lpclib_assert(tm->tm_hour < 24);
    lpclib_assert(tm->tm_mday < 32);
    lpclib_assert(tm->tm_mon < 13);
    lpclib_assert(tm->tm_year < 4096);
    lpclib_assert(tm->tm_wday < 8);
    lpclib_assert(tm->tm_yday < 367);
    
    RTC_Lock();
    
    /* Save Alarm Mask */
    tempAMR = RTC->AMR;
    
    /* Turn off alarms to prevent spurious interrupts */
    
    RTC->AMR = 0xff;

    /* Set the RTC AlarmTime Values */
    RTC->ALSEC   = tm->tm_sec;
    RTC->ALMIN   = tm->tm_min;
    RTC->ALHOUR  = tm->tm_hour;
    RTC->ALDOM   = tm->tm_mday;
    RTC->ALMONTH = tm->tm_mon;
    RTC->ALYEAR  = tm->tm_year + 1900;
    RTC->ALDOW   = tm->tm_wday;
    RTC->ALDOY   = tm->tm_yday;
    
    /* Restore Alarm Mask */
    RTC->AMR = tempAMR;
    
    RTC_Unlock();    
}


/** @brief  Get the Current Alarm Time from the Real Time Clock
  * @param  tm    Pointer to a struct tm value that will be filled in
  * @return The same struct tm Value Passed In
  */
struct tm *RTC_GetAlarm(struct tm *tm)
{
    RTC_Lock();

    tm->tm_sec   = RTC->ALSEC;
    tm->tm_min   = RTC->ALMIN;
    tm->tm_hour  = RTC->ALHOUR;
    tm->tm_wday  = RTC->ALDOW;
    tm->tm_mday  = RTC->ALDOM;
    tm->tm_mon   = RTC->ALMONTH;
    tm->tm_year  = RTC->ALYEAR - 1900;
    tm->tm_yday  = RTC->ALDOY;

    RTC_Unlock();
    
    /* This layer doesn't deal with DST settings */
    tm->tm_isdst = 0;
    
    return tm;
}

