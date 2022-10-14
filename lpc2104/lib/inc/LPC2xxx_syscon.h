/******************************************************************************
 * @file:    LPC2xxx_syscon.h
 * @purpose: Header File for using LPC2xxx CPU System Control Block
 * @version: V1.0
 * @author:  Tymm Twillman
 * @date:    1. November 2010
 * @license: Simplified BSD License
 *
 * This file defines types and functions for using the LPC2xxx System Control
 * block:
 *  - External Interrupts
 *  - Memory Remapping
 *  - PLL Configuration
 *  - Power Control
 *  - APB Bus Divider
 *  - Reset Source Identification
 *  - Code Security Protection
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

#ifndef LPC2XXX_SYSCON_H_
#define LPC2XXX_SYSCON_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/

#include <stdint.h>
#include "LPC2xxx.h"
#include "LPC2xxx_lib_assert.h"

#ifndef LPC2XXX_HAS_SYSCON
#error  Your CPU does not seem to have a System Control Block, or a CPU header
#error  file is missing/incorrect.
#endif

/** @addtogroup SYSCON System Control Block Interface
  * @{
  */

/* Types --------------------------------------------------------------------*/

/** @addtogroup SYSCON_Types SYSCON Interface Typedefs
  * @{
  */

/** @addtogroup SYSCON_MAM_Mode Memory Accelerator Modes
  * @{
  */
typedef enum {
    SYSCON_MAMMode_Disabled = 0x00,    /*!< Memory Accel Disabled           */
    SYSCON_MAMMode_Partial,            /*!< Memory Accel Partially Enabled  */
    SYSCON_MAMMode_Full,               /*!< Memory Accel Fully Enabled      */
} SYSCON_MAMMode_Type;
#define SYSCON_IS_MAM_MODE(Mode) (((Mode) == SYSCON_MAMMode_Disabled) \
                               || ((Mode) == SYSCON_MAMMode_Partial)  \
                               || ((Mode) == SYSCON_MAMMode_Full))

/**
  * @}
  */

/** @addtogroup SYSCON_ExtInt External Interrupts
  * @{
  */

#define SYSCON_ExtInt_Mask (0x0f)      /*!< External Interrupt Enable Mask */
#define SYSCON_ExtInt_0  (1 << 0)      /*!< Enable Ext Int 0               */
#define SYSCON_ExtInt_1  (1 << 1)      /*!< Enable Ext Int 1               */
#define SYSCON_ExtInt_2  (1 << 2)      /*!< Enable Ext Int 2               */
#define SYSCON_ExtInt_3  (1 << 3)      /*!< Enable Ext Int 3               */

/* @brief Type for passing External Interrupt Sources (bitmask) */
typedef uint8_t SYSCON_ExtInt_Type;

/**
  * @}
  */

/** @addtogroup SYSCON_IntWake Interrupt Wakeup Sources
  * @{
  */

#define SYSCON_IntWake_Mask      (0xc00f)  /*!< Bitmask for Int Wakeup Ena.  */
#define SYSCON_IntWake_ExtInt_0  (1 << 0)  /*!< Wake on Ext Int 0            */
#define SYSCON_IntWake_ExtInt_1  (1 << 1)  /*!< Wake on Ext Int 1            */
#define SYSCON_IntWake_ExtInt_2  (1 << 2)  /*!< Wake on Ext Int 2            */
#define SYSCON_IntWake_ExtInt_3  (1 << 3)  /*!< Wake on Ext Int 3            */
#define SYSCON_IntWake_BOD       (1 << 14) /*!< Wake on Brownout             */
#define SYSCON_IntWake_RTC       (1 << 15) /*!< Wake from Real Time Clock    */

/* @brief Type for passing Interrupt Wake Sources (bitmask) */
typedef uint16_t SYSCON_IntWake_Type;

/**
  * @}
  */

/** @addtogroup SYSCON_ExtIntSenseMode External Interrupt Sensing Modes
  * @{
  */
typedef enum {
    SYSCON_ExtIntSenseMode_Level = 0x00,    /*!< Ext Int on levels    */
    SYSCON_ExtIntSenseMode_Edge,            /*!< Ext Int on edges     */
} SYSCON_ExtIntSenseMode_Type;
#define SYSCON_IS_EXT_INT_SENSE_MODE(Mode) (((Mode) == SYSCON_ExtIntSenseMode_Level) \
                                         || ((Mode) == SYSCON_ExtIntSenseMode_Edge))

/**
  * @}
  */

/** @defgroup SYSCON_ExtIntSensePolarity External Interrupt Sensing Polarities
  * @{
  */
typedef enum {
    SYSCON_ExtIntSensePolarity_Low = 0x00,  /*!< Ext Int on LOW levels  */
    SYSCON_ExtIntSensePolarity_High,        /*!< Ext Int on HIGH levels */
} SYSCON_ExtIntSensePolarity_Type;
#define SYSCON_IS_EXT_INT_SENSE_POLARITY(Mode) (((Mode) == SYSCON_ExtIntSensePolarity_Low) \
                                             || ((Mode) == SYSCON_ExtIntSensePolarity_High))

/**
  * @}
  */

/** @defgroup SYSCON_MemRemap Interrupt Vector Table System Remapping Values
  * @{
  */
typedef enum {
    SYSCON_MemRemap_Bootloader = 0x00, /*!< Remap Vect Table to Bootloader */
    SYSCON_MemRemap_RAM,               /*!< Remap Vect Table to RAM        */
    SYSCON_MemRemap_FLASH,             /*!< Remap Vect Table to Flash      */
} SYSCON_MemRemap_Type;
#define SYSCON_IS_MEM_REMAP(MemRemap) (((MemRemap) == SYSCON_MemRemap_Bootloader)  \
                                    || ((MemRemap) == SYSCON_MemRemap_RAM)         \
                                    || ((MemRemap) == SYSCON_MemRemap_FLASH))

/**
  * @}
  */

/** @defgroup SYSCON_SysPLLPVal_Type PLL P (Divisor) Values
  * @{
  */
/*! System PLL P (Divisor) Values -- divisor is 2 times the PVal value */
typedef enum {
    SYSCON_SysPLLPVal_1 = 0x00,            /*!< Divide by 2  */
    SYSCON_SysPLLPVal_2 = 0x01,            /*!< Divide by 4  */
    SYSCON_SysPLLPVal_4 = 0x02,            /*!< Divide by 8  */
    SYSCON_SysPLLPVal_8 = 0x03             /*!< Divide by 16 */
} SYSCON_SysPLLPVal_Type;
#define SYSCON_IS_SYSPLL_PVAL_TYPE(PVal) (((PVal) == SYSCON_SysPLLPVal_1) \
                                       || ((PVal) == SYSCON_SysPLLPVal_2) \
                                       || ((PVal) == SYSCON_SysPLLPVal_4) \
                                       || ((PVal) == SYSCON_SysPLLPVal_8))

/**
  * @}
  */

/** @defgroup SYSCON_PeriphPowerLine_Vals Peripheral Power Enable Bits
  * @{
  */
#define SYSCON_PeriphPowerLine_Mask   (0x1817beUL) /*!< Periph. Power Mask   */
#define SYSCON_PeriphPowerLine_TIM0   (1UL << 1)   /*!< Timer0  Power Ctrl   */
#define SYSCON_PeriphPowerLine_TIM1   (1UL << 2)   /*!< Timer1  Power Ctrl   */
#define SYSCON_PeriphPowerLine_UART0  (1UL << 3)   /*!< UART0   Power Ctrl   */
#define SYSCON_PeriphPowerLine_UART1  (1UL << 4)   /*!< UART1   Power Ctrl   */
#define SYSCON_PeriphPowerLine_PWM0   (1UL << 5)   /*!< PWM0    Power Ctrl   */
#define SYSCON_PeriphPowerLine_I2C0   (1UL << 7)   /*!< I2C0    Power Ctrl   */
#define SYSCON_PeriphPowerLine_SPI0   (1UL << 8)   /*!< SPI0    Power Ctrl   */
#define SYSCON_PeriphPowerLine_RTC    (1UL << 9)   /*!< RTC     Power Ctrl   */
#define SYSCON_PeriphPowerLine_SPI1   (1UL << 10)  /*!< SPI1    Power Ctrl   */
#define SYSCON_PeriphPowerLine_AD0    (1UL << 12)  /*!< ADC0    Power Ctrl   */
#define SYSCON_PeriphPowerLine_I2C1   (1UL << 19)  /*!< I2C1    Power Ctrl   */
#define SYSCON_PeriphPowerLine_AD1    (1UL << 20)  /*!< ADC1    Power Ctrl   */

/**
  * @}
  */

/** @defgroup SYSCON_ResetSource_Vals MCU Reset Sources
  * @{
  */
#define SYSCON_ResetSource_Mask      (0x0f)    /*!< MCU Reset Source Mask  */
#define SYSCON_ResetSource_PowerOn   (1 << 0)  /*!< MCU Reset by Power On  */
#define SYSCON_ResetSource_External  (1 << 1)  /*!< MCU Reset by Ext Reset */
#define SYSCON_ResetSource_WDT       (1 << 2)  /*!< MCU Reset by Watchdog  */
#define SYSCON_ResetSource_BOD       (1 << 3)  /*!< MCU Reset by Brownout  */

/**
  * @}
  */

/** @defgroup SYSCON_FIOPort_Bits Fast IO Port Enable Bits
  * @{
  */
#define SYSCON_FIOPort_Mask          (0x03)    /*!< Fast IO Enable Bits Mask */
#define SYSCON_FIOPort_0             (1 << 0)  /*!< Fast IO Enable on Port 0 */
#define SYSCON_FIOPort_1             (1 << 1)  /*!< Fast IO Enable on Port 1 */

/**
  * @}
  */

/**
  * @}
  */

/* Inline Functions ---------------------------------------------------------*/

/** @defgroup SYSCON_Inline_Functions SYSCON Interface Inline Functions
  * @{
  */

/** @brief Set the Operating Mode of the Memory Accelerator
  * @param  Mode             The New Operating Mode
  * @return None.
  */
__INLINE static void SYSCON_SetMAMMode(SYSCON_MAMMode_Type Mode)
{
    lpc2xxx_lib_assert(MAM_IS_MODE(Mode));

    SYSCON->MAMCR = Mode;
}

/** @brief Get the Operating Mode of the Memory Accelerator
  * @param  None.
  * @return The Current MAM Mode
  */
__INLINE static SYSCON_MAMMode_Type SYSCON_GetMAMMode(void)
{
    return (SYSCON->MAMCR & SYSCON_MAMMODE_Mask) >> SYSCON_MAMMODE_Shift;
}

/** @brief Set the Cycle Timing of Memory Accesses
  * @param  Cycles           Number of Cycles Per FLASH Memory Access (1-7)
  * @return None.
  */
__INLINE static void SYSCON_SetMAMFlashAccessCycles(uint8_t Cycles)
{
    lpc2xxx_lib_assert((Cycles > 0) && (Cycles <= 7));

    SYSCON->MAMTIM = Cycles;
}

/** @brief Get the Currently Configured Cycle Timing of Memory Accesses
  * @param  None.
  * @return Number of Cycles Per Flash Access
  */
__INLINE static uint8_t SYSCON_GetMAMFlashAccessCycles(void)
{
    return (SYSCON->MAMTIM & SYSCON_MAMTIM_Mask) & SYSCON_MAMTIM_Shift;
}

/** @brief  Remap the ISR vector area of memory to selected region
  * @param  Memmap   Token indicating the area to map to the ISR vector area
  * @return None.
  *
  * Used to change memory area where the interrupt vectors are stored.
  */
__INLINE static void SYSCON_SetMemRemap(uint8_t MemRemap)
{
    lpc2xxx_lib_assert(SYSCON_IS_MEM_REMAP(MemRemap));

    SYSCON->MEMMAP = MemRemap;
}

/** @brief  Return the currently remapped memory region
  * @param  None.
  * @return Bits specifying the currently remapped region
  *
  * Used to change memory area where the interrupt vectors are stored.
  */
__INLINE static uint8_t SYSCON_GetMemRemap(void)
{
    return SYSCON->MEMMAP;
}

/** @brief  Get the "lock" state of the System PLL
  * @param  None.
  * @return 1 if the System PLL is locked, 0 otherwise.
  */
__INLINE static uint8_t SYSCON_SysPLLIsLocked(void)
{
    return (SYSCON->PLLSTAT & SYSCON_PLLSTAT_PLOCK);
}

/** @brief  Set the M and P System PLL parameters controlling PLL clock scaling
  * @param  Scaler  ORed M & P values to set
  * @return None.
  *
  * Generally will be called in a while(1) loop after the source has been updated
  * and SYSPLL_EnableSourceUpdate() has been called.
  */
__INLINE static void SYSCON_SetSysPLLClockScaler(uint16_t Scaler)
{
    lpc2xxx_lib_assert((Scaler & ~(SYSPLLCON_MSEL_Mask | SYSPLLCON_PSEL_Mask)) == 0);

    SYSCON->PLLCON = Scaler;
}

/** @brief  Set the System PLL M (multiplier) value
  * @param  MVal  The PLL Multiplier Value
  * @return None.
  */
__INLINE static void SYSCON_SetSysPLLMVal(uint16_t MVal)
{
    lpc2xxx_lib_assert(((MVal - 1) & ~(PLLCFG_MSEL_Mask)) == 0);

    SYSCON->PLLCON = (SYSCON->PLLCFG & ~SYSCON_PLLCFG_MSEL_Mask) | (MVal - 1);
}

/** @brief  Get the System PLL M (multiplier) value
  * @param  None.
  * @return The System PLL Multiplier value
  */
__INLINE static uint8_t SYSCON_GetSysPLLMVal(void)
{
    return (SYSCON->PLLCON & SYSCON_PLLCFG_MSEL_Mask) + 1;
}

/** @brief  Set the System PLL P (divider) value
  * @param  PVal  The PLL Divider Value
  * @return None.
  */
__INLINE static void SYSCON_SetSysPLLPVal(uint16_t PVal)
{
    lpc2xxx_lib_assert(SYSCON_IS_SYSPLL_PVAL_TYPE(PVal));

    SYSCON->PLLCON = (SYSCON->PLLCFG & ~SYSCON_PLLCFG_PSEL_Mask)
                          | (PVal << SYSCON_PLLCFG_PSEL_Shift);
}

/** @brief  Get the System PLL P (Divider) value
  * @param  None.
  * @return The System PLL Divider value
  */
__INLINE static uint8_t SYSCON_GetSysPLLPVal(void)
{
    return (SYSCON->PLLCON & SYSCON_PLLCFG_PSEL_Mask)  >> SYSCON_PLLCFG_PSEL_Shift;
}

/** @brief  Get the currently configured M and P System PLL values
  * @param  None.
  * @return System PLL scaler value.
  */
__INLINE static uint16_t SYSCON_GetSysPLLClockScaler(void)
{
    return (SYSCON->PLLCON & (SYSCON_PLLCFG_MSEL_Mask | SYSCON_PLLCFG_PSEL_Mask));
}

/** @brief  Tell the System PLL to use updated settings
  * @param  None.
  * @return None.
  */
__INLINE static void SYSCON_FeedSysPLL(void)
{
    SYSCON->PLLFEED = 0xaa;
    SYSCON->PLLFEED = 0x55;
}

/** @brief  Enable the System PLL
  * @param  None.
  * @return None.
  */
__INLINE static void SYSCON_EnableSysPLL(void)
{
    SYSCON->PLLCON |= SYSCON_PLLCON_PLLE;
}

/** @brief  Disable the System PLL
  * @param  None.
  * @return None.
  */
__INLINE static void SYSCON_DisableSysPLL(void)
{
    SYSCON->PLLCON &= ~SYSCON_PLLCON_PLLE;
}

/** @brief  Determine whether the System PLL is enabled
  * @param  None.
  * @return 1 if the System PLL is Enabled, 0 Otherwise
  */
__INLINE static uint8_t SYSCON_SysPLLIsEnabled(void)
{
    return (SYSCON->PLLCON & SYSCON_PLLCON_PLLE) ? 1:0;
}

/** @brief  Connect the System PLL
  * @param  None.
  * @return None.
  */
__INLINE static void SYSCON_ConnectSysPLL(void)
{
    SYSCON->PLLCON |= SYSCON_PLLCON_PLLC;
}

/** @brief  Disonnect the System PLL
  * @param  None.
  * @return None.
  */
__INLINE static void SYSCON_DisconnectSysPLL(void)
{
    SYSCON->PLLCON &= ~SYSCON_PLLCON_PLLC;
}

/** @brief  Determine whether the System PLL is connected
  * @param  None.
  * @return 1 if the System PLL is Connected, 0 Otherwise
  */
__INLINE static uint8_t SYSCON_SysPLLIsConnected(void)
{
    return (SYSCON->PLLCON & SYSCON_PLLCON_PLLC) ? 1:0;
}

/** @brief  Enable Idle Mode (only comes out w/interrupt)
  * @param  None.
  * @return None.
  */
__INLINE static void SYSCON_EnableIdleMode(void)
{
    SYSCON->PCON |= SYSCON_PCON_IDL;
}

/** @brief  Enable Power Down Mode (only comes out w/ext interrupt)
  * @param  None.
  * @return None.
  */
__INLINE static void SYSCON_EnablePowerDownMode(void)
{
    SYSCON->PCON |= SYSCON_PCON_PD;
}

/** @brief  Enable Power to Peripherals
  * @param  Periph  The peripherals for which to enable power
  * @return None.
  */
__INLINE static void SYSCON_EnablePeriphPowerLines(uint32_t Periph)
{
    lpc2xxx_lib_assert((Periph & SYSCON_PeriphPowerLine_Mask) == 0);

    SYSCON->PCONP |= Periph;
}

/** @brief  Disable Power to Peripherals
  * @param  Periph  The peripherals for which to disable power
  * @return None.
  */
__INLINE static void SYSCON_DisablePeriphPowerLines(uint32_t Periph)
{
    lpc2xxx_lib_assert((Periph & SYSCON_PeriphPowerLine_Mask) == 0);

    SYSCON->PCONP &= ~Periph;
}

/** @brief  Get Power Settings for System Peripheral Blocks
  * @param  None.
  * @return Bits Specifying Powered Peripherals
  */
__INLINE static uint16_t SYSCON_GetPeriphPowerLines(void)
{
    return SYSCON->PCONP & SYSCON_PeriphPowerLine_Mask;
}

/** @brief  Get the Source of Last System Reset(s)
  * @param  None.
  * @return Bits Specifying the Reset Source(s) (SYSCON_ResetSource_XYZ)
  */
__INLINE static uint8_t SYSCON_GetResetSource(void)
{
    return (SYSCON->RSID & SYSCON_RSID_Mask) >> SYSCON_RSID_Shift;
}

/** @brief  Clear bits specifying source(s) of previous system resets
  * @param   Bits specifying the reset source(s) (SYSCON_ResetSource_XYZ) to clear
  * @return  None.
  */
__INLINE static void SYSCON_ClearResetSource(uint8_t Sources)
{
    lpc2xxx_lib_assert((Sources & ~SYSCON_RSID_Mask) == 0);

    SYSCON->RSID |= (Sources << SYSCON_RSID_Shift);
}

/** @brief  Set the bus divider for the APB bus
  * @param  Divider -- the new divider value (1, 2 or 4)
  * @return None.
  */
__INLINE static void SYSCON_SetAPBClockDivider(uint8_t Divider)
{
    lpc2xxx_lib_assert((Divider == 1) || (Divider == 2) || (Divider == 4));

    /* Masking desired as 4 -> 0 (value the register wants for div 4) */
    SYSCON->APBDIV = (Divider << SYSCON_APBDIV_Shift) & SYSCON_APBDIV_Mask;
}

/** @brief  Get the current value of the bus divider for the APB bus
  * @param  None.
  * @return The current APB Bus Divider Value
  */
__INLINE static uint8_t SYSCON_GetAPBClockDivider(void)
{
    uint8_t Divider;


    Divider = (SYSCON->APBDIV & SYSCON_APBDIV_Mask) >> SYSCON_APBDIV_Shift;

    if (Divider == 0) {
        Divider = 4;
    }

    return Divider;
}

/** @brief  Get any External Interrupts that have been triggered
  * @param  None.
  * @return Bits specifying the triggered external interrupts
  */
__INLINE static uint8_t SYSCON_GetExtInt(void)
{
    return (SYSCON->EXTINT & SYSCON_EINT_Mask) >> SYSCON_EINT_Shift;
}

/** @brief  Clear bits specifying source(s) of external interrupts
  * @param  Sources  The ExtInt Sources to clear.
  * @return Bits specifyingthe interrupt(s) to clear
  */
__INLINE static void SYSCON_ClearExtInt(uint8_t Sources)
{
    lpc2xxx_lib_assert((Sources & ~(SYSCON_EINT_Mask >> SYSCON_EINT_Shift)) == 0);

    SYSCON->RSID |= (Sources << SYSCON_EINT_Shift);
}

/** @brief Enable Interrupt Wake Sources for CPU's Power Down / Idle Modes
  * @param  WakeSources   Sources to enable waking the CPU with an interrupt
  * @return None.
  */
__INLINE static void SYSCON_EnableIntWakeConfig(uint16_t WakeSources)
{
    lpc2xxx_lib_assert((WakeSources & ~SYSCON_IntWake_Mask) == 0);

    SYSCON->INTWAKE |= WakeSources;
}

/** @brief Disable Interrupt Wake Sources for CPU's Power Down / Idle Modes
  * @param  WakeSources   Sources to disable waking the CPU with an interrupt
  * @return None.
  */
__INLINE static void SYSCON_DisableIntWakeConfig(uint16_t WakeSources)
{
    lpc2xxx_lib_assert((WakeSources & ~SYSCON_IntWake_Mask) == 0);

    SYSCON->INTWAKE &= ~WakeSources;
}

/** @brief Get the currently configured Interrupt Wake sources for CPU
  * @param  None.
  * @return Bits specifying currently configured wake sources
  */
__INLINE static uint16_t SYSCON_GetEnabledIntWake(void)
{
    return (SYSCON->INTWAKE & SYSCON_IntWake_Mask);
}

/** @brief Set the sense mode (edge / level triggering) for given external interrupt line
  * @param ExtIntNum  The External Interrupt # (0-3) to set the sense mode on
  * @param Mode       The Mode (SYSCON_ExtIntSenseMode_Type) to set for the EINT
  * @return None.
  */
__INLINE static void SYSCON_SetExtIntSenseMode(uint8_t ExtIntNum, SYSCON_ExtIntSenseMode_Type Mode)
{
    lpc2xxx_lib_assert(ExtIntNum <= 3);
    lpc2xxx_lib_assert(SYSCON_IS_EXT_INT_SENSE_MODE(Mode));

    SYSCON->EXTMODE = (SYSCON->EXTMODE & ~(3 << (ExtIntNum * 2))) | (Mode << (ExtIntNum * 2));
}

/** @brief Get the configured sense mode (edge / level triggering) for given external interrupt line
  * @param ExtIntNum  The External Interrupt # (0-3) to get the configured sense mode of
  * @return The Sense Mode (SYSCON_ExtIntSenseMode_Type) currently set for the EINT
  */
__INLINE static SYSCON_ExtIntSenseMode_Type SYSCON_GetExtIntSenseMode(uint8_t ExtIntNum)
{
    lpc2xxx_lib_assert(ExtIntNum <= 3);

    return (SYSCON->EXTMODE >> (ExtIntNum * 2)) & 0x03;
}

/** @brief Set the sense polarity (high / low triggering) for given external interrupt line
  * @param ExtIntNum  The External Interrupt # (0-3) to set the sense polarity on
  * @param Polarity   The Polarity (SYSCON_ExtIntSensePolarity_Type) to set for the EINT
  * @return None.
  */
__INLINE static void SYSCON_SetExtIntSensePolarity(uint8_t ExtIntNum, SYSCON_ExtIntSensePolarity_Type Polarity)
{
    lpc2xxx_lib_assert(ExtIntNum <= 3);
    lpc2xxx_lib_assert(SYSCON_IS_EXT_INT_SENSE_MODE(Polarity));

    SYSCON->EXTPOLAR = (SYSCON->EXTPOLAR & ~(3 << (ExtIntNum * 2))) | (Polarity << (ExtIntNum * 2));
}

/** @brief Get the configured sense polarity (high/low triggering) for given external interrupt line
  * @param ExtIntNum  The External Interrupt # (0-3) to get the configured sense polarity of
  * @return The Sense Polarity (SYSCON_ExtIntSenseMode_Type) currently set for the EINT
  */
__INLINE static SYSCON_ExtIntSensePolarity_Type SYSCON_GetExtIntSensePolarity(uint8_t ExtIntNum)
{
    lpc2xxx_lib_assert(ExtIntNum <= 3);

    return (SYSCON->EXTPOLAR >> (ExtIntNum * 2)) & 0x03;
}

/** @brief Enable Fast IO Mode on GPIO Port 0 / 1
  * @param FIOPorts  Bit fields of ports to enable FIO mode on
  * @return None.
  *
  * Fast GPIO Mode is incompatible with old GPIO style & FIO access must be used.
  */
__INLINE static void SYSCON_EnableFastIO(uint8_t FIOPorts)
{
    lpc2xxx_lib_assert((FIOPorts & ~SYSCON_FIOPort_Mask) == 0);

    SYSCON->SCS |= FIOPorts;
}

/** @brief Disable Fast IO Mode on GPIO Port 0 / 1
  * @param FIOPorts  Bit fields of ports to disable FIO mode on
  * @return None.
  *
  * Fast GPIO Mode is incompatible with old GPIO style & FIO access must be used.
  */
__INLINE static void SYSCON_DisableFastIO(uint8_t FIOPorts)
{
    lpc2xxx_lib_assert((FIOPorts & ~SYSCON_FIOPort_Mask) == 0);

    SYSCON->SCS &= ~FIOPorts;
}

/** @brief Test Whether Fast IO Mode is Enabled on Ports
  * @param None.
  * @return Bit Mask Specifying Ports with FIO Enabled
  */
__INLINE static uint8_t SYSCON_FastIOIsEnabled(void)
{
    return (SYSCON->SCS & SYSCON_SCS_Mask) >> SYSCON_SCS_Shift;
}

#ifdef SYSCON_PCON_PDBOD

/** @brief  Enable the System Brown-Out Detector on Powerdown
  * @param  None.
  * @return None.
  */
__INLINE static void SYSCON_EnablePowerDownBOD(void)
{
    /* Note: powerdown bit active low */
    SYSCON->PCON &= ~SYSCON_PCON_PDBOD;
}

/** @brief  Disable the System Brown-Out Detector on Powerdown
  * @param  None.
  * @return None.
  */
__INLINE static void SYSCON_DisablePowerDownBOD(void)
{
    /* Note: powerdown bit active low */
    SYSCON->PCON |= SYSCON_PCON_PDBOD;
}

/** @brief  Determine whether the BOD is enabled on Powerdown
  * @param  None.
  * @return 1 if the BOD is enabled on powerdown, 0 otherwise
  */
__INLINE static uint8_t SYSCON_PowerDownBODIsEnabled(void)
{
    /* Note: powerdown bit active low */
    return (SYSCON->PCON & SYSCON_PCON_PDBOD) ? 0:1;
}

#endif

/**
  * @}
  */

/* Exported Functions ---------------------------------------------------------*/

/** @defgroup SYSCON_Functions SYSCON Interface Non-Inline Functions
  * @{
  */

/** @brief  Configure the System PLL With a New M/P Value
  * @param  PLLVal  A hybrid M/P value ready for use in the PLLCFG Register
  * @return None.
  */
void SYSCON_SysPLLConfig(int8_t PLLVal);

/** @brief  Calculate the PLL Value (for use with SYSCON_SysPLLConfig) for given ClkIn / ClkOut
  * @param  clk_in   PLL Input clock frequency
  * @param  clk_out  The desired PLL output frequency
  * @param  err      Pointer to location to save error between requested output & actual
  * @return          Value to use to configure PLL for requested PLL output; -1 on error.
  *
  * Frequency error is returned in Hz.  NULL is valid (no clock error will be saved).
  * This function will round the output clock to the nearest possible.  In some cases
  *  it may be desired to abort with an error if error is not within a given range.
  *
  */
int8_t SYSCON_PLLScalerCalc(uint32_t clk_in, uint32_t clk_out, int32_t *err);

/**
  * @}
  */


/**
  * @}
  */

#ifdef __cplusplus
};
#endif

#endif /* #ifndef LPC2XXX_SYSCON_H_ */
