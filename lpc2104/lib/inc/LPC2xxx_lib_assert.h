/****************************************************************************
 * @file:    LPC2xxx_lib_assert.h
 * @purpose: Header file for LPC 2xxx Library Debugging
 * @version: V1.0
 * @author:  Tymm Twillman
 * @date:    1. June 2010
 * @license: Simplified BSD License
 *
 * lpc2xxx_lib_assert() is a macro that calls the function 
 * lpc2xxx_lib_assert_function() on an assertion failure (when the passed in
 * value == 0).  It disables interrupts and goes into an infinite loop to
 * make it relatively easy to track down problems with a debugger.
 *
 * To use: Define LPC2XXX_LIB_DEBUG when compiling / using the library.
 *
 * Note: Define LPC2XXX_LIB_ASSERT_TICKLE_WATCHDOG when compiling the
 *  source file if you want the assert function to keep tickling the 
 *  watchdog (to prevent WDT resets during assert-assisted debugging).
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

#ifndef LPC2XXX_LIB_ASSERT_H_
#define LPC2XXX_LIB_ASSERT_H_


/* Defines -----------------------------------------------------------------*/

/** @addtogroup DEBUG
  * @{
  */

#ifdef LPC2XXX_LIB_DEBUG

/*! Function wrapper that is only used when LPC2XXX_LIB_DEBUG is set */
# define lpc2xxx_lib_assert(x) (if (!(x)) { lpc2xxx_lib_assert_function(); } )

#else
# define lpc2xxx_lib_assert(x) do {} while(0);
#endif


/* Exported Functions ------------------------------------------------------*/

/**
  * @brief  Simple assert() function for library debugging
  * @param  None.
  * @return Does not return (infinite loop)
  */
void lpc2xxx_lib_assert_function(void);


/**
  * @}
  */


#endif /* #ifndef LPC2XXX_LIB_ASSERT_H_ */
