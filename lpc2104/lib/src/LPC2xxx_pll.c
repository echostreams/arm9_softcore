/******************************************************************************
 * @file:    LPC2xxx_pll.c
 * @purpose: System PLL Utility Functions for NXP LPC2xxx Chips
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
 
/* Includes -----------------------------------------------------------------*/

#include <stdint.h>
#include "LPC2xxx_pll.h"


/* Functions ----------------------------------------------------------------*/

/** @brief  Calculate the value to load into the System PLL's PLLCFG register
  *
  * @param  [in]  clk_in   The clock frequency that is fed into the PLL
  * @param  [in]  clk_out  The desired output clock frequency
  * @param  [out] err      Error between desired & actual vals is output here.
  *
  * @return Value to load into PLLCFG / -1 if freq is outside PLL range.
  */
int8_t PLL_ScalerCalc(uint32_t clk_in, uint32_t clk_out, int32_t *err)
{
    uint32_t actual;
    uint32_t m;
    uint32_t p;


    /* 9,750,000 Hz is lowest valid setting for this PLL.
     * 160,000,000 Hz is highest (though obviously beyond CPU capabilities)
     */

    m = (clk_out + (clk_in / 2)) / clk_in;
    if ((m < 1) || (m > 0x20)) {
        return -1;
    }
    
    actual = m * clk_in;

    if (actual < 19500000UL) {
        p = SYSCON_PLLSTAT_PSEL_8;
    } else if (actual < 39000000UL) {
        p = SYSCON_PLLSTAT_PSEL_4;
    } else if (actual < 78000000UL) {
        p = SYSCON_PLLSTAT_PSEL_2;
    } else {
        p = SYSCON_PLLSTAT_PSEL_1;
    }

    if (err) {
        *err = (actual - clk_out);
    }

    return ((p << 5) | (m - 1));
}


