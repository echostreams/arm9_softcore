/******************************************************************************
 * @file:    LPC2xxx_pll.h
 * @purpose: Header File for utility functions for the PLLs on LPC2xxx CPUs
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
 
#ifndef LPC2XXX_PLL_H_
#define LPC2XXX_PLL_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/

#include <stdint.h>
#include "LPC2xxx.h"


/** @defgroup PLL
  * @{
  */

  
/* External Functions ---------------------------------------------------------*/

/** @defgroup PLL_Functions Phase Lock Loop Exported Functions
  * @{
  */

/** @brief  Calculate the value to load into the System PLL's PLLCFG register
  *
  * @param  [in]  clk_in   The clock frequency that is fed into the PLL
  * @param  [in]  clk_out  The desired output clock frequency
  * @param  [out] err      Error between desired & actual vals is output here.
  *
  * @return Value to load into PLLCFG / -1 if freq is outside PLL range.
  */
int8_t PLL_ScalerCalc(uint32_t clk_in, uint32_t clk_out, int32_t *err);

/**
  * @}
  */

/**
  * @}
  */


#ifdef __cplusplus
};
#endif

#endif /* #ifndef LPC2XXX_PLL_H_ */
