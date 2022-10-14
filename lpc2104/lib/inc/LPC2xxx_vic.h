/******************************************************************************
 * @file:    LPC2xxx_syscon.h
 * @purpose: Header File for using LPC2xxx CPU System Control Block
 * @version: V1.0
 * @author:  Tymm Twillman
 * @date:    1. November 2010
 * @license: Simplified BSD License
 *
 * Notes:
 *  - Slot 0 has highest priority
 *  - IRQ exception ptr -> 0x00000018
 *  - FIQ exception ptr -> 0x00000016
 *  - Must clear IRQ flag in peripheral plus do vic_irq_done()
 *     at end of IRQ's to clear pending IRQ and allow new ones
 *  - "base" argument to all macros should be base address of VIC controller
 *  - slot-specific things can actually set multiple slots, but that's
 *     usually not what you want, so this just handles 1 slot at a time.
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
 
#ifndef LPC2XXX_VIC_H_
#define LPC2XXX_VIC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/

#include <stdint.h>
#include "LPC2xxx.h"
#include "LPC2xxx_lib_assert.h"

#ifndef LPC2XXX_HAS_VIC
#error  Your CPU does not seem to have a Vectored Interrupt Controller, or a CPU header file is missing/incorrect.  
#endif


/** @addtogroup VIC Vectored Interrupt Controller Interface
  * This file defines types and functions for using the LPC2xxx Vectored
  * Interrupt Controller
  * @{
  */
  
/* Types --------------------------------------------------------------------*/

/** @addtogroup VIC_Types VIC Interface Typedefs
  * @{
  */

/**
  * @}
  */

/* Inline Functions ---------------------------------------------------------*/

/** @addtogroup VIC_Inline_Functions Vectored Interrupt Controller Interface Inline Functions
  * @{
  */

/** @brief Enable an Interrupt in the VIC Interrupt Controller
  * @param  IRQn             The IRQ Number to Enable
  * @return None.
  */
__INLINE static void VIC_EnableIRQ(IRQn_Type IRQn)
{
    lpc2xxx_lib_assert(IRQn <= 31);

    VIC->INTENABLE = (1 << ((uint32_t)(IRQn) & 0x1f));
}

/** @brief Disable an Interrupt in the VIC Interrupt Controller
  * @param  IRQn             The IRQ Number to Enable
  * @return None.
  */
__INLINE static void VIC_DisableIRQ(IRQn_Type IRQn)
{
    lpc2xxx_lib_assert(IRQn <= 31);

    VIC->INTENABLECLEAR = (1 << ((uint32_t)(IRQn) & 0x1f));
}

/** @brief Remove all IRQs from the Fast IRQ category
  * @param  None.
  * @return None.
  */
__INLINE static void VIC_ClearFIQs(void)
{
    VIC->INTSELECT = 0;
}

/** @brief Enable a Fast Interrupt in the VIC Interrupt Controller
  * @param  FIQn             The IRQ Number to set as a Fast Interrupt
  * @return None.
  */
__INLINE static void VIC_EnableFIQ(IRQn_Type FIQn)
{
    lpc2xxx_lib_assert(FIQn <= 31);

    VIC->INTSELECT |= (1 << ((uint32_t)(FIQn) & 0x1f));
}

/** @brief Disable a Fast Interrupt in the VIC Interrupt Controller
  * @param  FIQn             The IRQ Number to set as a Fast Interrupt
  * @return None.
  */
__INLINE static void VIC_DisableFIQ(IRQn_Type FIQn)
{
    lpc2xxx_lib_assert(FIQn <= 31);
    VIC->INTSELECT &= ~(1 << ((uint32_t)(FIQn) & 0x1f));
}

/** @brief Determine Whether an IRQ Number is Configured as a Fast Interrupt
  * @param  IRQn             The IRQ #
  * @return 1 if the IRQ is Configured as an FIQ, 0 Otherwise
  */
__INLINE static uint8_t VIC_IsFIQ(IRQn_Type IRQn)
{
    lpc2xxx_lib_assert(IRQn <= 31);
    
    return (VIC->INTSELECT & (1 << ((uint32_t)(IRQn) & 0x1f))); 
}

/** @brief Get a Bitmask of IRQs Configured as Fast Interrupts
  * @param  None.
  * @return Bitmask of IRQ's that are set as FIQ's
  */
__INLINE static uint32_t VIC_GetFIQs(void)
{
    return VIC->INTSELECT;
}

/** @brief Read the Interrupt Pending bit for an Interrupt Source
  * @param  IRQn             The IRQ Number of the Source
  * @return 1 if the IRQ is Pending; 0 Otherwise.
  */
__INLINE static uint8_t VIC_GetPendingIRQ(IRQn_Type IRQn)
{
    return VIC->IRQSTATUS & (1 << ((uint32_t)(IRQn) & 0x1f)) ? 1:0;
}

/** @brief Read the Interrupt Pending bit for an Interrupt Source Configured as FIQ
  * @param  FIQn             The IRQ Number of the Source
  * @return 1 if the FIQ is Pending; 0 Otherwise.
  */
__INLINE static uint8_t VIC_GetPendingFIQ(IRQn_Type FIQn)
{
    return VIC->FIQSTATUS & (1 << ((uint32_t)(FIQn) & 0x1f)) ? 1:0;
}

/** @brief Set the Pending Bit for an Interrupt
  * @param  IRQn             The IRQ Number to Pend
  * @return None.
  */
__INLINE static void VIC_SetPendingIRQ(IRQn_Type IRQn)
{
    lpc2xxx_lib_assert(IRQn <= 31);
    
    VIC->SOFTINT = (1 << ((uint32_t)(IRQn) & 0x1f));
}

/** @brief Clear the Pending Bit for an Interrupt
  * @param  IRQn             The IRQ Number to Pend
  * @return None.
  */
__INLINE static void VIC_ClearPendingIRQ(IRQn_Type IRQn)
{
    lpc2xxx_lib_assert(IRQn <= 31);
    
    VIC->SOFTINTCLEAR = (1 << ((uint32_t)(IRQn) & 0x1f));
}

/** @brief Get the Raw, Un-Masked Active IRQ Flags
  * @param  None.
  * @return A Bitmask Specifying IRQs that are Active, Masked or Not
  */
__INLINE static uint32_t VIC_GetRawIRQs(void)
{
    return VIC->RAWINT;
}

/** @brief Set the VIC Protection Bit to Restrict Access to Priviledged Tasks
  * @param  None.
  * @return None.
  */
__INLINE static void VIC_EnableProtection(void)
{
    VIC->PROTECTION = 1;
}

/** @brief Clear the VIC Protection Bit to Remove Priviledged Task Restriction
  * @param  None.
  * @return None.
  *
  * If protection is set, state can only be changed by priviledged task.
  */
__INLINE static void VIC_DisableProtection(void)
{
    VIC->PROTECTION = 0;
}

/** @brief Set the Default Handler that will be called for Unrouted Interrupts
  * @param  Handler          A Pointer to a Handler that will be Called for Unrouted Interrupts
  * @return None.
  */
__INLINE static void VIC_SetDefaultIRQHandler(void (*IRQHandler)(void))
{
    VIC->DEFVECTADDR = (uint32_t)IRQHandler;
}

/** @brief Get the Default Handler that is Configured to be called for Unrouted Interrupts
  * @param  None.
  * @return The Default IRQ Handler
  */
__INLINE static void (*VIC_GetDefaultIRQHandler(void))(void)
{
    return (void (*)())VIC->DEFVECTADDR;
}

/** @brief Set a VIC Slot for IRQ Routing
  * @param  Slot             The VIC Slot to Configure for the IRQ
  * @param  IRQn             The Peripheral IRQ Number to Route
  * @param  Handler          The IRQ Handler to Install for the IRQ
  * @return None.
  *
  * Does NOT enable the IRQ Slot; Call VIC_EnableSlot() for that.
  */
__INLINE static void VIC_SetSlot(uint8_t Slot, IRQn_Type IRQn, void (*IRQHandler)(void))
{
    lpc2xxx_lib_assert(IRQn <= 31);
    lpc2xxx_lib_assert(Slot < __VIC_IRQ_SLOTS);
    
    
    VIC->VECTCNTL0[Slot] = 0;
    VIC->VECTADDR0[Slot] = (uint32_t)IRQHandler;
    VIC->VECTCNTL0[Slot] = IRQn;
}

/** @brief Get the Configured IRQ Number for a VIC Slot
  * @param  Slot             The VIC Slot Number
  * @return The IRQ Number Configured for the VIC Slot
  */
__INLINE IRQn_Type VIC_GetSlotIRQn(uint8_t Slot)
{
    lpc2xxx_lib_assert(Slot < __VIC_IRQ_SLOTS);
    
    return (VIC->VECTCNTL0[Slot] & VIC_IRQNUM_Mask) >> VIC_IRQNUM_Shift;
}

/** @brief Get the Configured IRQ Handler for a VIC Slot
  * @param  Slot             The VIC Slot Number
  * @return The IRQ Handler Configured for the VIC Slot
  */
__INLINE static void (*VIC_GetSlotIRQHandler(uint8_t Slot))(void)
{    
    lpc2xxx_lib_assert(Slot < __VIC_IRQ_SLOTS);
    
    return (void (*)())(VIC->VECTADDR0[Slot]);
}

/** @brief Enable an IRQ Slot to Route IRQs to the Configured Handler
  * @param  Slot             The VIC Slot to Enable for IRQ Routing
  * @Return None.
  */
__INLINE static void VIC_EnableSlot(uint8_t Slot)
{
    lpc2xxx_lib_assert(Slot < __VIC_IRQ_SLOTS);

    VIC->VECTCNTL0[Slot] |= VIC_IRQEN;
}

/** @brief Disable an IRQ Slot to Stop Routing IRQs to the Configured Handler
  * @param  Slot             The VIC Slot to Disable
  * @Return None.
  */
__INLINE static void VIC_DisableSlot(uint8_t Slot)
{
    lpc2xxx_lib_assert(Slot < __VIC_IRQ_SLOTS);

    VIC->VECTCNTL0[Slot] &= ~VIC_IRQEN;
}

/** @brief Determine Whether a VIC IRQ Slot is Enabled
  * @param  Slot             The VIC IRQ Slot Number
  * @return 1 if the IRQ Slot is Enabled, 0 Otherwise.
  */
__INLINE static uint8_t VIC_IsSlotEnabled(uint8_t Slot)
{
    lpc2xxx_lib_assert(Slot < __VIC_IRQ_SLOTS);

    return (VIC->VECTCNTL0[Slot] & VIC_IRQEN) ? 1:0;
}

/** @brief Indicate to the VIC that an IRQ is Done Processing
  * @param  None.
  * @return None.
  *
  * This tells the VIC to Update Priority Hardware at the end of an IRQ Handler
  */
__INLINE static void VIC_IRQDone(void)
{
    VIC->VECTADDR = 0x00;
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

#endif /* #ifndef LPC2XXX_VIC_H_ */
