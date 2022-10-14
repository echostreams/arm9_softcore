/******************************************************************************
 * @file:    LPC2106.h
 * @purpose: Header File for using LPC2106/5/4 CPUs
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

#ifndef LPC2106_H_
#define LPC2106_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/* Compiler Abstraction Defines -----------------------------------------------------------------*/

#if defined ( __GNUC__ ) || defined ( __ICCARM__ ) || defined ( __TASKING__ )
# define __ASM     __asm
# define __INLINE  inline
#elif defined ( __CC_ARM )
# define __ASM     __asm
# define __INLINE  __inline
#endif


#ifdef __cplusplus
  #define   __I              volatile       /*!< Read Only (C++)             */
#else
  #define   __I              volatile const /*!< Read Only (C)               */
#endif
#define     __O              volatile       /*!< Write Only                  */
#define     __IO             volatile       /*!< Read / Write                */


/* CPU Defines ----------------------------------------------------------------------------------*/

#define LPC2XXX_MODEL        2106          /*!< General model type of MCU    */
#define __VIC_IRQ_SLOTS      16            /*!< # of IRQ slots in VIC        */


/* IRQ Numbers ----------------------------------------------------------------------------------*/

/** @defgroup LPC2106_IRQn LPC2104/5/6 IRQ Numbers
  * @{
  */

typedef enum IRQn {
    WDT_IRQn                 = 0x00,   /*!< Watchdog Timer IRQ               */
    ICE_RX_IRQn              = 0x02,   /*!< ARM ICE Receive IRQ              */
    ICE_TX_IRQn              = 0x03,   /*!< ARM ICE Transmit IRQ             */
    TIM0_IRQn                = 0x04,   /*!< 32-Bit Timer 0 IRQ               */
    TIM1_IRQn                = 0x05,   /*!< 32-Bit Timer 1 IRQ               */
    UART0_IRQn               = 0x06,   /*!< UART 0 IRQ                       */
    UART1_IRQn               = 0x07,   /*!< UART 1 IRQ                       */
    PWM0_IRQn                = 0x08,   /*!< PWM Controller 0 IRQ             */
    I2C0_IRQn                = 0x09,   /*!< I2C 0 IRQ                        */
    SPI0_IRQn                = 0x0a,   /*!< Standard SPI Port IRQ            */
    PLL_IRQn                 = 0x0c,   /*!< System PLL IRQ                   */
    RTC_IRQn                 = 0x0d,   /*!< Real Time Clock IRQ              */
    EINT0_IRQn               = 0x0e,   /*!< External Interrupt 0 IRQ         */
    EINT1_IRQn               = 0x0f,   /*!< External Interrupt 1 IRQ         */
    EINT2_IRQn               = 0x10,   /*!< External Interrupt 2 IRQ         */
    EINT3_IRQn               = 0x11,   /*!< External Interrupt 3 IRQ         */
} IRQn_Type;

/**
  * @}
  */


/* Peripheral Registers -----------------------------------------------------*/

/** @addtogroup WDT
  *
  * Watchdog Timer Peripheral
  *
  * @{
  */

typedef struct {
    __IO    uint32_t    MOD;           /*!< Offset: 0x00 Mode Register                           */
    __IO    uint32_t    TC;            /*!< Offset: 0x04 Timer Constant Register                 */
    __O     uint32_t    FEED;          /*!< Offset: 0x08 Feed Sequence Register                  */
    __I     uint32_t    TV;            /*!< Offset: 0x0c Timer Value Register                    */
} WDT_Type;

/**
  * @}
  */


/** @addtogroup TIMER
  *
  * TIMER Peripheral
  *
  * @{
  */

typedef struct {
    __IO    uint32_t    IR;            /*!< Offset: 0x00 Interrupt Register                      */
    __IO    uint32_t    TCR;           /*!< Offset: 0x04 Timer Control Register                  */
    __IO    uint32_t    TC;            /*!< Offset: 0x08 Timer Counter                           */
    __IO    uint32_t    PR;            /*!< Offset: 0x0c Prescale Register                       */
    __IO    uint32_t    PC;            /*!< Offset: 0x10 Prescale Counter                        */
    __IO    uint32_t    MCR;           /*!< Offset: 0x14 Match Control Register                  */
    __IO    uint32_t    MR0;           /*!< Offset: 0x18 Match Register 0                        */
    __IO    uint32_t    MR1;           /*!< Offset: 0x1c Match Register 1                        */
    __IO    uint32_t    MR2;           /*!< Offset: 0x20 Match Register 2                        */
    __IO    uint32_t    MR3;           /*!< Offset: 0x24 Match Register 3                        */
    __IO    uint32_t    CCR;           /*!< Offset: 0x28 Capture Control Register                */
    __I     uint32_t    CR0;           /*!< Offset: 0x2c Capture Register 0                      */
    __I     uint32_t    CR1;           /*!< Offset: 0x30 Capture Register 1                      */
    __I     uint32_t    CR2;           /*!< Offset: 0x34 Capture Register 2                      */
    __I     uint32_t    CR3;           /*!< Offset: 0x38 Capture Register 3                      */
    __IO    uint32_t    EMR;           /*!< Offset: 0x3c External Match Register                 */
            uint32_t      Reserved16_27[12];
    __IO    uint32_t    CTCR;          /*!< Offset: 0x70 Count Control Register                  */
} TIMER_Type;

/**
  * @}
  */


/** @addtogroup UART
  *
  * UART Peripheral
  *
  * @{
  */

typedef struct {
    union {
        __I  uint32_t   RBR;           /*!< Offset: 0x00 Rx Buffer Register  (DLAB=0)         */
        __O  uint32_t   THR;           /*!< Offset: 0x00 Tx Holding Register (DLAB=0)         */
        __IO uint32_t   DLL;           /*!< Offset: 0x00 Divisor Latch LSB (DLAB=1)           */
    };

    union {
        __IO uint32_t   DLM;           /*!< Offset: 0x04 Divisor Latch MSB (DLAB=1)           */
        __IO uint32_t   IER;           /*!< Offset: 0x04 Interrupt Enable Reg (DLAB=0)        */
    };

    union {
        __I uint32_t    IIR;           /*!< Offset: 0x08 Interrupt ID Register                */
        __O uint32_t    FCR;           /*!< Offset: 0x08 FIFO Control Register                */
    };

    __IO    uint32_t    LCR;           /*!< Offset: 0x0c Line Control Register                */
    __IO    uint32_t    MCR;           /*!< Offset: 0x10 Modem Control Register               */
    __I     uint32_t    LSR;           /*!< Offset: 0x14 Line Status Register                 */
    __I     uint32_t    MSR;           /*!< Offset: 0x18 Modem Status Register                */
    __IO    uint32_t    SCR;           /*!< Offset: 0x1C Scratch Pad Register                 */
            uint32_t      Reserved8_11[4];
    __IO    uint32_t    TER;           /*!< Offset: 0x30 Transmit Enable Register             */
} UART_Type;

/**
  * @}
  */


/** @addtogroup PWM
  *
  * Pulse Width Modulator Controller
  *
  * @{
  */

typedef struct {
    __IO    uint32_t    IR;            /*!< Offset: 0x00 Interrupt Register                      */
    __IO    uint32_t    TCR;           /*!< Offset: 0x04 Timer Control Register                  */
    __IO    uint32_t    TC;            /*!< Offset: 0x08 Timer Counter                           */
    __IO    uint32_t    PR;            /*!< Offset: 0x0c Prescale Register                       */
    __IO    uint32_t    PC;            /*!< Offset: 0x10 Prescale Count                          */
    __IO    uint32_t    MCR;           /*!< Offset: 0x14 Match Control Register                  */
    __IO    uint32_t    MR0;           /*!< Offset: 0x18 Match Register 0                        */
    __IO    uint32_t    MR1;           /*!< Offset: 0x1c Match Register 1                        */
    __IO    uint32_t    MR2;           /*!< Offset: 0x20 Match Register 2                        */
    __IO    uint32_t    MR3;           /*!< Offset: 0x24 Match Register 3                        */
            uint32_t      Reserved10_15[6];
    __IO    uint32_t    MR4;           /*!< Offset: 0x40 Match Register 4                        */
    __IO    uint32_t    MR5;           /*!< Offset: 0x44 Match Register 5                        */
    __IO    uint32_t    MR6;           /*!< Offset: 0x48 Match Register 6                        */
    __IO    uint32_t    PCR;           /*!< Offset: 0x4c PWM Control Register                    */
    __IO    uint32_t    LER;           /*!< Offset: 0x50 Latch Enable Register                   */
} PWM_Type;

/**
  * @}
  */


/** @addtogroup I2C
  *
  * I2C Controller Peripheral
  *
  * @{
  */

typedef struct {
    __IO    uint32_t    CONSET;        /*!< Offset: 0x00 Control Set Register                    */
    __I     uint32_t    STAT;          /*!< Offset: 0x04 Status Register                         */
    __IO    uint32_t    DAT;           /*!< Offset: 0x08 Data Register                           */
    __IO    uint32_t    ADR0;          /*!< Offset: 0x0c Address Register 0                      */
    __IO    uint32_t    SCLH;          /*!< Offset: 0x10 SCL Duty Cycle High                     */
    __IO    uint32_t    SCLL;          /*!< Offset: 0x14 SCL Duty Cycle Low                      */
    __O     uint32_t    CONCLR;        /*!< Offset: 0x18 Control Clear Register                  */
} I2C_Type;

/**
  * @}
  */


/** @addtogroup SPI
  *
  * SPI Interface Peripheral
  *
  * @{
  */

typedef struct {
    __IO    uint32_t    CR;            /*!< Offset: 0x00 Control Register                        */
    __I     uint32_t    SR;            /*!< Offset: 0x04 Status Register                         */
    __IO    uint32_t    DR;            /*!< Offset: 0x08 Data Register                           */
    __IO    uint32_t    CCR;           /*!< Offset: 0x0c Clock Counter Register                  */
    __IO    uint32_t    INT;           /*!< Offset: 0x10 Interrupt Flag Register                 */
} SPI_Type;

/**
  * @}
  */


/** @addtogroup RTC
  *
  * Real Time Clock
  *
  * @{
  */

typedef struct {
    __IO    uint32_t    ILR;           /*!< Offset: 0x00 Interrupt Location Register             */
    __I     uint32_t    CTC;           /*!< Offset: 0x04 Clock Tick Counter                      */
    __IO    uint32_t    CCR;           /*!< Offset: 0x08 Clock Control Register                  */
    __IO    uint32_t    CIIR;          /*!< Offset: 0x0c Counter Increment Interrupt Register    */
    __IO    uint32_t    AMR;           /*!< Offset: 0x10 Alarm Mask Register                     */
    __I     uint32_t    CTIME0;        /*!< Offset: 0x14 Consolidated Time Register 0            */
    __I     uint32_t    CTIME1;        /*!< Offset: 0x18 Consolidated Time Register 1            */
    __I     uint32_t    CTIME2;        /*!< Offset: 0x1c Consolidated Time Register 2            */
    __IO    uint32_t    SEC;           /*!< Offset: 0x20 Seconds Counter                         */
    __IO    uint32_t    MIN;           /*!< Offset: 0x24 Minutes Register                        */
    __IO    uint32_t    HOUR;          /*!< Offset: 0x28 Hours Register                          */
    __IO    uint32_t    DOM;           /*!< Offset: 0x2c Day of Month Register                   */
    __IO    uint32_t    DOW;           /*!< Offset: 0x30 Day of Week Register                    */
    __IO    uint32_t    DOY;           /*!< Offset: 0x34 Day of Year Register                    */
    __IO    uint32_t    MONTH;         /*!< Offset: 0x38 Months Register                         */
    __IO    uint32_t    YEAR;          /*!< Offset: 0x3c Years Register                          */
            uint32_t      Reserved16_23[8];
    __IO    uint32_t    ALSEC;         /*!< Offset: 0x60 Alarm Seconds Value                     */
    __IO    uint32_t    ALMIN;         /*!< Offset: 0x64 Alarm Minutes Value                     */
    __IO    uint32_t    ALHOUR;        /*!< Offset: 0x68 Alarm Seconds Value                     */
    __IO    uint32_t    ALDOM;         /*!< Offset: 0x6c Alarm Day of Month Value                */
    __IO    uint32_t    ALDOW;         /*!< Offset: 0x70 Alarm Day of Week Value                 */
    __IO    uint32_t    ALDOY;         /*!< Offset: 0x74 Alarm Day of Year Value                 */
    __IO    uint32_t    ALMONTH;       /*!< Offset: 0x78 Alarm Months Value                      */
    __IO    uint32_t    ALYEAR;        /*!< Offset: 0x7c Alarm Years Value                       */
    __IO    uint32_t    PREINT;        /*!< Offset: 0x80 Prescaler Integer Value                 */
    __IO    uint32_t    PREFRAC;       /*!< Offset: 0x84 Prescaler Fraction Value                */
} RTC_Type;

/**
  * @}
  */


/** @addtogroup GPIO
  *
  * General Purpose IO
  *
  * @{
  */

typedef struct {
    __IO    uint32_t    PIN;           /*!< Offset: 0x00 Pin Value Register                      */
    __IO    uint32_t    SET;           /*!< Offset: 0x04 Output Set Register                     */
    __IO    uint32_t    DIR;           /*!< Offset: 0x08 Direction Control Register              */
    __IO    uint32_t    CLR;           /*!< Offset: 0x0c Output Clear Register                   */
} GPIO_Type;

/**
  * @}
  */


/** @addtogroup PINSEL
  *
  * Pin Select (GPIO / Peripheral Configuration) Block
  *
  * @{
  */

typedef struct {
    __IO    uint32_t    PINSEL0;       /*!< Offset: 0x00 Pin Function Select Register 0          */
    __IO    uint32_t    PINSEL1;       /*!< Offset: 0x04 Pin Function Select Register 1          */
    __IO    uint32_t    PINSEL2;       /*!< Offset: 0x08 Pin Function Select Register 2          */
} PINSEL_Type;

/**
  * @}
  */


/** @addtogroup SYSCON
  *
  * System Configuration Block
  *
  * @{
  */

typedef struct {
    __IO    uint32_t    MAMCR;         /*!< Offset: 0x000 Mem Accel Control Register             */
    __IO    uint32_t    MAMTIM;        /*!< Offset: 0x004 Mem Accel Timing Control               */
    __IO    uint32_t      Reserved_2_15[14];
    __IO    uint32_t    MEMMAP;        /*!< Offset: 0x040 Memory Mapping Control                 */
            uint32_t      Reserved_17_31[15];
    __IO    uint32_t    PLLCON;        /*!< Offset: 0x080 PLL Control Register                   */
    __IO    uint32_t    PLLCFG;        /*!< Offset: 0x084 PLL Config Register                    */
    __I     uint32_t    PLLSTAT;       /*!< Offset: 0x088 PLL Status Register                    */
    __O     uint32_t    PLLFEED;       /*!< Offset: 0x08c PLL Feed Register                      */
            uint32_t      Reserved_36_47[11];
    __IO    uint32_t    PCON;          /*!< Offset: 0x0c0 Power Control Register                 */
    __IO    uint32_t    PCONP;         /*!< Offset: 0x0c4 Power Control for Peripherals          */
            uint32_t      Reserved_50_63[14];
    __IO    uint32_t    APBDIV;        /*!< Offset: 0x100 APB Divider Control                    */
            uint32_t      Reserved_65_79[15];
    __IO    uint32_t    EXTINT;        /*!< Offset: 0x140 External Int Flag Register             */
    __IO    uint32_t    INTWAKE;       /*!< Offset: 0x144 External Int Wkup Register             */
    __IO    uint32_t    EXTMODE;       /*!< Offset: 0x148 External Int Mode Register             */
    __IO    uint32_t    EXTPOLAR;      /*!< Offset: 0x14c External Int Polarities                */
            uint32_t      Reserved_84_103[20];
    __IO    uint32_t    SCS;           /*!< Offset: 0x1a0 System Control & Status                */
} SYSCON_Type;

/**
  * @}
  */


/** @addtogroup VIC
  *
  * Vectored Interrupt Controller
  *
  * @{
  */

typedef struct {
    __I     uint32_t    IRQSTATUS;     /*!< Offset: 0x000 IRQ Status                             */
    __I     uint32_t    FIQSTATUS;     /*!< Offset: 0x004 FIQ Status                             */
    __I     uint32_t    RAWINT;        /*!< Offset: 0x008 Raw Interrupt Status                   */
    __IO    uint32_t    INTSELECT;     /*!< Offset: 0x00c Interrupt Select                       */
    __IO    uint32_t    INTENABLE;     /*!< Offset: 0x010 Interrupt Enable                       */
    __O     uint32_t    INTENABLECLEAR;/*!< Offset: 0x014 Interrupt Enable Clear                 */
    __IO    uint32_t    SOFTINT;       /*!< Offset: 0x018 Software Interrupt Register            */
    __O     uint32_t    SOFTINTCLEAR;  /*!< Offset: 0x01c Software Interrupt Clear               */
    __IO    uint32_t    PROTECTION;    /*!< Offset: 0x020 Protection Enable                      */
            uint32_t      Reserved9_11[3];
    __IO    uint32_t    VECTADDR;      /*!< Offset: 0x030 Vector Address                         */
    __IO    uint32_t    DEFVECTADDR;   /*!< Offset: 0x034 Default Vector Address                 */
            uint32_t      Reserved14_63[50];
    __IO    uint32_t    VECTADDR0[16]; /*!< Offset: 0x100 Vector Addresses                       */
            uint32_t      Reserved80_127[48];
    __IO    uint32_t    VECTCNTL0[16]; /*!< Offset: 0x200 Vector Control Reg                     */
} VIC_Type;

/**
  * @}
  */


/* Peripheral Bit Masks & Postions ------------------------------------------*/

/** @addtogroup WDT
  *
  * Watchdog Timer
  *
  * @{
  */

#define LPC2XXX_HAS_WDT

/** @defgroup WDT_WDMOD_Bit_Definitions (WDMOD) Watchdog Mode Register Bit Definitions
  *
  * @{
  */

#define WDT_WDEN                       (1 << 0)            /*!< Watchdog Enable                  */
#define WDT_WDRESET                    (1 << 1)            /*!< Watchdog Chip Reset Enable       */
#define WDT_WDTOF                      (1 << 2)            /*!< Watchdog Timeout Flag            */
#define WDT_WDINT                      (1 << 3)            /*!< Watchdog Interrupt Enable        */

/**
  * @}
  */

/**
  * @}
  */


/** @addtogroup TIMER
  *
  * 32-Bit Timer
  *
  * @{
  */

#define LPC2XXX_HAS_TIMER

/** @defgroup TIMER_IR_Bit_Definitions (TxIR) TIMER Interrupt Register Bit Definitions
  *
  * @{
  */

#define TIMER_IR_Mask                  (0xff)              /*!< Useable Bits in IR               */
#define TIMER_IR_Shift                 (0)

#define TIMER_IR_MR0                   (1 << 0)            /*!< Match Channel 0 Interrupt Flag   */
#define TIMER_IR_MR1                   (1 << 1)            /*!< Match Channel 1 Interrupt Flag   */
#define TIMER_IR_MR2                   (1 << 2)            /*!< Match Channel 2 Interrupt Flag   */
#define TIMER_IR_MR3                   (1 << 3)            /*!< Match Channel 3 Interrupt Flag   */
#define TIMER_IR_CR0                   (1 << 4)            /*!< Capture Channel 0 Interrupt Flag */
#define TIMER_IR_CR1                   (1 << 5)            /*!< Capture Channel 1 Interrupt Flag */
#define TIMER_IR_CR2                   (1 << 6)            /*!< Capture Channel 2 Interrupt Flag */
#define TIMER_IR_CR3                   (1 << 7)            /*!< Capture Channel 3 Interrupt Flag */

/**
  * @}
  */

/** @defgroup TIMER_TCR_Bit_Definitions (TxTCR) TIMER Control Register Bit Definitions
  *
  * @{
  */

#define TIMER_TCR_Mask                 (0x03)              /*!< Useable Bits in TCR              */
#define TIMER_TCR_Shift                (0)

#define TIMER_CE                       (1 << 0)            /*!< Counter Enable                   */
#define TIMER_CR                       (1 << 1)            /*!< Counter Reset                    */

/**
  * @}
  */

/** @defgroup TIMER_TC_Bit_Definitions (TxTC) TIMER Timer Counter Register Bit Definitions
  *
  * @{
  */

#define TIMER_TC_Mask                  (0xffffffffUL)      /*!< Mask for Timer Ctr Count         */
#define TIMER_TC_Shift                 (0)

/**
  * @}
  */

/** @defgroup TIMER_PR_Bit_Definitions (TxPR) TIMER Prescale Register Bit Definitions
  *
  * @{
  */

#define TIMER_PR_Mask                  (0xffffffffUL)      /*!< Mask for Prescale Reg Value      */
#define TIMER_PR_Shift                 (0)

/**
  * @}
  */

/** @defgroup TIMER_PC_Bit_Definitions (TxPC) TIMER Prescale Counter Register Bit Definitions
  *
  * @{
  */

#define TIMER_PC_Mask                  (0xffffffffUL)      /*!< Mask for Prescale Ctr Value      */
#define TIMER_PC_Shift                 (0)

/**
  * @}
  */

/** @defgroup TIMER_MCR_Bit_Definitions (TxMCR) TIMER Match Control Register Bit Definitions
  *
  * @{
  */

#define TIMER_MCR_Mask                 (0x0fff)            /*!< Useable Bits in MCRn             */
#define TIMER_MCR_Shift                (0)

#define TIMER_MR0I                     (1 << 0)            /*!< Interrupt when MR0 matches TC    */
#define TIMER_MR0R                     (1 << 1)            /*!< Reset if MR0 matches TC          */
#define TIMER_MR0S                     (1 << 2)            /*!< Stop if MR0 matches TC           */
#define TIMER_MR1I                     (1 << 3)            /*!< Interrupt when MR1 matches TC    */
#define TIMER_MR1R                     (1 << 4)            /*!< Reset if MR1 matches TC          */
#define TIMER_MR1S                     (1 << 5)            /*!< Stop if MR1 matches TC           */
#define TIMER_MR2I                     (1 << 6)            /*!< Interrupt when MR2 matches TC    */
#define TIMER_MR2R                     (1 << 7)            /*!< Reset if MR2 matches TC          */
#define TIMER_MR2S                     (1 << 8)            /*!< Stop if MR2 matches TC           */
#define TIMER_MR3I                     (1 << 9)            /*!< Interrupt when MR3 matches TC    */
#define TIMER_MR3R                     (1 << 10)           /*!< Reset if MR3 matches TC          */
#define TIMER_MR3S                     (1 << 11)           /*!< Stop if MR3 matches TC           */

/**
  * @}
  */

/** @defgroup TIMER_MR0_Bit_Definitions (TxMR0) TIMER Match Register 0 Bit Definitions
  *
  * @{
  */

#define TIMER_MR0_Mask                 (0xffffffffUL)      /*!< Mask for Match Value             */
#define TIMER_MR0_Shift                (0)

/**
  * @}
  */

/** @defgroup TIMER_MR1_Bit_Definitions (TxMR1) TIMER Match Register 1 Bit Definitions
  *
  * @{
  */

#define TIMER_MR1_Mask                 (0xffffffffUL)      /*!< Mask for Match Value             */
#define TIMER_MR1_Shift                (0)

/**
  * @}
  */

/** @defgroup TIMER_MR2_Bit_Definitions (TxMR2) TIMER Match Register 2 Bit Definitions
  *
  * @{
  */

#define TIMER_MR2_Mask                 (0xffffffffUL)      /*!< Mask for Match Value             */
#define TIMER_MR2_Shift                (0)

/**
  * @}
  */

/** @defgroup TIMER_MR3_Bit_Definitions (TxMR3) TIMER Match Register 3 Bit Definitions
  *
  * @{
  */

#define TIMER_MR3_Mask                 (0xffffffffUL)      /*!< Mask for Match Value             */
#define TIMER_MR3_Shift                (0)

/**
  * @}
  */

/** @defgroup TIMER_CCR_Bit_Definitions (TxCCR) TIMER Capture Control Register Bit Definitions
  *
  * @{
  */

#define TIMER_CCR_Mask                 (0x0fff)            /*!< Useable Bits in CCR              */
#define TIMER_CCR_Shift                (0)

#define TIMER_CAP0RE                   (1 << 0)            /*!< Capture on CAP0 rising edge      */
#define TIMER_CAP0FE                   (1 << 1)            /*!< Capture on CAP0 falling edge     */
#define TIMER_CAP0I                    (1 << 2)            /*!< Enable Interrupt on Capt. Event  */
#define TIMER_CAP1RE                   (1 << 3)            /*!< Capture on CAP1 rising edge      */
#define TIMER_CAP1FE                   (1 << 4)            /*!< Capture on CAP1 falling edge     */
#define TIMER_CAP1I                    (1 << 5)            /*!< Enable Interrupt on Capture Event*/
#define TIMER_CAP2RE                   (1 << 6)            /*!< Capture on CAP2 rising edge      */
#define TIMER_CAP2FE                   (1 << 7)            /*!< Capture on CAP2 falling edge     */
#define TIMER_CAP2I                    (1 << 8)            /*!< Enable Interrupt on Capture Event*/
#define TIMER_CAP3RE                   (1 << 9)            /*!< Capture on CAP3 rising edge      */
#define TIMER_CAP3FE                   (1 << 10)           /*!< Capture on CAP3 falling edge     */
#define TIMER_CAP3I                    (1 << 11)           /*!< Enable Interrupt on Capture Event*/

/**
  * @}
  */

/** @defgroup TIMER_CR0_Bit_Definitions (TxCR0) TIMER Capture Register 0 Bit Definitions
  *
  * @{
  */

#define TIMER_CR0_Mask                 (0xffffffffUL)      /*!< Mask for Capture Value           */
#define TIMER_CR0_Shift                (0)

/**
  * @}
  */

/** @defgroup TIMER_CR1_Bit_Definitions (TxCR1) TIMER Capture Register 1 Bit Definitions
  *
  * @{
  */

#define TIMER_CR1_Mask                 (0xffffffffUL)      /*!< Mask for Capture Value           */
#define TIMER_CR1_Shift                (0)

/**
  * @}
  */

/** @defgroup TIMER_CR2_Bit_Definitions (TxCR2) TIMER Capture Register 2 Bit Definitions
  *
  * @{
  */

#define TIMER_CR2_Mask                 (0xffffffffUL)      /*!< Mask for Capture Value           */
#define TIMER_CR2_Shift                (0)

/**
  * @}
  */

/** @defgroup TIMER_CR3_Bit_Definitions (TxCR3) TIMER Capture Register 3 Bit Definitions
  *
  * @{
  */

#define TIMER_CR3_Mask                 (0xffffffffUL)      /*!< Mask for Capture Value           */
#define TIMER_CR3_Shift                (0)

/**
  * @}
  */

/** @defgroup TIMER_EMR_Bit_Definitions (TxEMR) TIMER External Match Register Bit Definitions
  *
  * @{
  */

#define TIMER_EMR_Mask                 (0x0fff)            /*!< Useable Bits in EMR              */
#define TIMER_EMR_Shift                (0)

#define TIMER_EM0                      (1 << 0)            /*!< Status of MAT0 Pin               */
#define TIMER_EM1                      (1 << 1)            /*!< Status of MAT1 Pin               */
#define TIMER_EM2                      (1 << 2)            /*!< Status of MAT2 Pin               */
#define TIMER_EM3                      (1 << 3)            /*!< Status of MAT3 Pin               */

#define TIMER_EMC0_Mask                (0x03 << 4)         /*!< MAT0 Match Control Bits          */
#define TIMER_EMC0_Shift               (4)
#define TIMER_EMC1_Mask                (0x03 << 6)         /*!< MAT1 Match Control Bits          */
#define TIMER_EMC1_Shift               (6)
#define TIMER_EMC2_Mask                (0x03 << 8)         /*!< MAT2 Match Control Bits          */
#define TIMER_EMC2_Shift               (8)
#define TIMER_EMC3_Mask                (0x03 << 10)        /*!< MAT3 Match Control Bits          */
#define TIMER_EMC3_Shift               (10)

/**
  * @}
  */

/** @defgroup TIMER_CTCR_Bit_Definitions (TxCTCR) TIMER Count Control Register Bit Definitions
  *
  * @{
  */

#define TIMER_CTCR_Mask                (0x0f)              /*!< Useable Bits in CTCR             */
#define TIMER_CTCR_Shift               (0)

#define TIMER_MODE_Mask                (0x03)              /*!< TIMER Count/Timer Mode Mask      */
#define TIMER_MODE_Shift               (0)
#define TIMER_MODE_TIMER               (0x00)              /*!< Timer Mode                       */
#define TIMER_MODE_COUNT_RISE          (0x01)              /*!< Count Rising Edges               */
#define TIMER_MODE_COUNT_FALL          (0x02)              /*!< Count Falling Edges              */
#define TIMER_MODE_COUNT_BOTH          (0x03)              /*!< Count Both Edges                 */

#define TIMER_COUNT_INPUT_Mask         (0x03 << 2)         /*!< Counter Mode Input Channel       */
#define TIMER_COUNT_INPUT_Shift        (2)
#define TIMER_COUNT_INPUT_CAP0         (0x00)              /*!< Count CAP0 Transitions           */
#define TIMER_COUNT_INPUT_CAP1         (0x01 << 2)         /*!< Count CAP1 Transitions           */
#define TIMER_COUNT_INPUT_CAP2         (0x02 << 2)         /*!< Count CAP2 Transitions           */
#define TIMER_COUNT_INPUT_CAP3         (0x03 << 2)         /*!< Count CAP3 Transitions           */

/**
  * @}
  */

/**
  * @}
  */


/** @addtogroup UART
  *
  * UART Peripheral
  *
  * @{
  */

#define LPC2XXX_HAS_UART

/** @defgroup UART_RBR_Bit_Definitions (UxRBR) UART Receive Buffer Register Bit Definitions
  *
  * @{
  */

#define UART_RBR_Mask                  (0xff)              /*!< Receiver Buffer Register Mask    */
#define UART_RBR_Shift                 (0)

/**
  * @}
  */

/** @defgroup UART_DLL_Bit_Definitions (UxDLL) UART Divisor Latch LSB Register Bit Definitions
  *
  * @{
  */

#define UART_DLLSB_Mask                (0xff)              /*!< Divisor Latch LSB Mask           */
#define UART_DLLSB_Shift               (0)

/**
  * @}
  */

/** @defgroup UART_DLM_Bit_Definitions (UxDLM) UART Divisor Latch MSB Register Bit Definitions
  *
  * @{
  */

#define UART_DLMSB_Mask                (0xff)              /*!< Divisor Latch MSB Mask           */
#define UART_DLMSB_Shift               (0)

/**
  * @}
  */

/** @defgroup UART_IER_Bit_Definitions (UxIER) UART Interrupt Enable Register Bit Definitions
  *
  * @{
  */

#define UART_IER_Mask                  (0x030f)            /*!< Useable Bits in IER Register     */
#define UART_IER_Shift                 (0)

#define UART_RBR_INT_ENA               (1 << 0)            /*!< Rx Data Avail/Timeout IRQ Enable */
#define UART_THRE_INT_ENA              (1 << 1)            /*!< TX Holding Reg Empty IRQ Enable  */
#define UART_LINE_INT_ENA              (1 << 2)            /*!< RX Line Status IRQ Enable        */
#define UART_MODM_INT_ENA              (1 << 2)            /*!< RX Modem Status IRQ Enable       */

/**
  * @}
  */

/** @defgroup UART_IIR_Bit_Definitions (UxIIR) UART Interrupt ID Register Bit Definitions
  *
  * @{
  */

#define UART_IIR_Mask                  (0x03cf)            /*!< Useable Bits in UART IIR Reg     */
#define UART_IIR_Shift                 (0)

#define UART_INTID_Mask                (0x0f)              /*!< Mask for Standard Interrupt ID's */
#define UART_INTID_Shift               (0)
#define UART_INTID_NONE                (0x01)              /*!< Int. Status (0 == IT pending)    */
#define UART_INTID_RLS                 (0x06)              /*!< Receive Line Status Interrupt    */
#define UART_INTID_RDA                 (0x04)              /*!< Receive Data Available Interrupt */
#define UART_INTID_CTI                 (0x0c)              /*!< Character Time-out Interrupt     */
#define UART_INTID_THRE                (0x02)              /*!< Tx Holding Reg Empty Interrupt   */
#define UART_INTID_MODM                (0x00)              /*!< Modem Line Interrupt             */

#define UART_FIFO_Mask                 (0x03 << 6)         /*!< Copy of UFCR                     */
#define UART_FIFO_Shift                (6)                 /*!< Bit Shift for Fifo Bits          */

/**
  * @}
  */

/** @defgroup UART_FCR_Bit_Definitions (UxFCR) UART FIFO Control Register
  *  (NOTE: Write Only Register)
  *
  * @{
  */

#define UART_FCR_Mask                  (0xc7)              /*!< Useable Bits in UART FCR Reg     */
#define UART_FCR_Shift                 (0)

#define UART_RXTRIGLVL_Mask            (3 << 6)            /*!< Mask for RX Trigger Level        */
#define UART_RXTRIGLVL_Shift           (6)
#define UART_RXTRIGLVL_1               (0x00)              /*!< Trigger RX IRQ on 1B  in FIFO    */
#define UART_RXTRIGLVL_4               (0x01 << 6)         /*!< Trigger RX IRQ on 4B  in FIFO    */
#define UART_RXTRIGLVL_8               (0x02 << 6)         /*!< Trigger RX IRQ on 8B  in FIFO    */
#define UART_RXTRIGLVL_14              (0x03 << 6)         /*!< Trigger RX IRQ on 14B in FIFO    */

#define UART_FIFOEN                    (1 << 0)            /*!< Enable RX/TX FIFOS (NECESSARY)   */
#define UART_FIFO_RX_RESET             (1 << 1)            /*!< Clear RX FIFO (self-clearing)    */
#define UART_FIFO_TX_RESET             (1 << 2)            /*!< Clear TX FIFO (self-clearing)    */

/**
  * @}
  */

/** @defgroup UART_LCR_Bit_Definitions (UxLCR) UART Line Control Register Bit Definitions
  *
  * @{
  */

#define UART_LCR_Mask                  (0xff)              /*!< Useable Bits in UART LCR Reg     */
#define UART_LCR_Shift                 (0)

#define UART_WORDLEN_Mask              (0x03)              /*!< Mask for Word Length             */
#define UART_WORDLEN_Shift             (0)
#define UART_WORDLEN_5                 (0x00)              /*!< 5 bit characters                 */
#define UART_WORDLEN_6                 (0x01)              /*!< 6 bit characters                 */
#define UART_WORDLEN_7                 (0x02)              /*!< 7 bit characters                 */
#define UART_WORDLEN_8                 (0x03)              /*!< 8 bit characters                 */

#define UART_PARITY_Mask               (0x07 << 3)         /*!< Mask for Parity Type             */
#define UART_PARITY_Shift              (3)
#define UART_PARITY_NONE               (0x00 << 3)         /*!< No Parity                        */
#define UART_PARITY_ODD                (0x01 << 3)         /*!< Odd Parity                       */
#define UART_PARITY_EVEN               (0x03 << 3)         /*!< Even Parity                      */
#define UART_PARITY_1                  (0x05 << 3)         /*!< Force Parity = 1                 */
#define UART_PARITY_0                  (0x07 << 3)         /*!< Force Parity = 0                 */

#define UART_2STOPBITS                 (1 << 2)            /*!< 2 stop bits                      */
#define UART_PAREN                     (1 << 3)            /*!< Enable Parity                    */
#define UART_BREAK                     (1 << 6)            /*!< Set BREAK on TX line             */
#define UART_DLAB                      (1 << 7)            /*!< Enable Access to Div Latch       */

/**
  * @}
  */

/** @defgroup UART_MCR_Bit_Definitions (UxMCR) UART Modem Control Register Bit Definitions
  *
  * @{
  */

#define UART_MCR_Mask                  (0xd3)              /*!< Useable Bits in UART MCR Reg     */
#define UART_MCR_Shift                 (0)

#define UART_DTR                       (1 << 0)            /*!< Control of /DTR line             */
#define UART_RTS                       (1 << 1)            /*!< Control of /RTS line             */
#define UART_LOOPBACK                  (1 << 4)            /*!< Enable Loopback Mode             */
#define UART_RTSENA                    (1 << 6)            /*!< Enable Auto-RTS Flow Control     */
#define UART_CTSENA                    (1 << 7)            /*!< Enable Auto-CTS Flow Control     */

/**
  * @}
  */

/** @defgroup UART_LSR_Bit_Definitions (UxLSR) UART Line Status Register Bit Definitions
  *
  * @{
  */

#define UART_LSR_Mask                  (0xff)              /*!< Useable Bits in UART LSR Reg     */
#define UART_LSR_Shift                 (0)

#define UART_RDR                       (1 << 0)            /*!< Receiver Data Ready              */
#define UART_OE                        (1 << 1)            /*!< Overrun Error                    */
#define UART_PE                        (1 << 2)            /*!< Parity Error                     */
#define UART_FE                        (1 << 3)            /*!< Framing Error                    */
#define UART_BI                        (1 << 4)            /*!< Break Interrupt                  */
#define UART_THRE                      (1 << 5)            /*!< Tx Holding Register Empty        */
#define UART_TEMT                      (1 << 6)            /*!< Transmitter Empty                */
#define UART_RXFE                      (1 << 7)            /*!< Error in RX FIFO                 */

/**
  * @}
  */

/** @defgroup UART_MSR_Bit_Definitions (UxMSR) UART Modem Status Register Bit Definitions (UART1 Only)
  *
  * @{
  */

#define UART_MSR_Mask                  (0xff)              /*!< Useable Bits in UART MSR Reg     */
#define UART_MSR_Shift                 (0)

#define UART_DCTS                      (1 << 0)            /*!< Change in CTS                    */
#define UART_DDSR                      (1 << 1)            /*!< Change in DSR                    */
#define UART_TERI                      (1 << 2)            /*!< Low-to-high on RI                */
#define UART_DDCD                      (1 << 3)            /*!< Change on DCD                    */
#define UART_CTS                       (1 << 4)            /*!< Current CTS State (inverted)     */
#define UART_DSR                       (1 << 5)            /*!< Current DSR State (inverted)     */
#define UART_RI                        (1 << 6)            /*!< Current RI State (inverted)      */
#define UART_DCD                       (1 << 7)            /*!< Current DCD State (inverted)     */

/**
  * @}
  */

/** @defgroup UART_TER_Bit_Definitions (UxTER) UART Transmit Enable Register Bit Definitions
  *
  * @{
  */

#define UART_TXEN                      (1 << 7)            /*!< Enable Transmitter               */

/**
  * @}
  */

/**
  * @}
  */


/** @addtogroup PWM
  *
  * Pulse Width Modulator Peripheral
  *
  * @{
  */

#define LPC2XXX_HAS_PWM

/** @defgroup PWM_IR_Bit_Definitions PWM Interrupt Register Bit Definitions (PWMxIR)
  *
  * @{
  */

#define PWM_IR_MR0                     (1 << 0)            /*!< Match Channel 0 Interrupt Flag   */
#define PWM_IR_MR1                     (1 << 1)            /*!< Match Channel 1 Interrupt Flag   */
#define PWM_IR_MR2                     (1 << 2)            /*!< Match Channel 2 Interrupt Flag   */
#define PWM_IR_MR3                     (1 << 3)            /*!< Match Channel 3 Interrupt Flag   */
#define PWM_IR_MR4                     (1 << 8)            /*!< Match Channel 4 Interrupt Flag   */
#define PWM_IR_MR5                     (1 << 9)            /*!< Match Channel 5 Interrupt Flag   */
#define PWM_IR_MR6                     (1 << 10)           /*!< Match Channel 6 Interrupt Flag   */

/**
  * @}
  */

/** @defgroup PWM_TCR_Bit_Definitions PWM Control Register Bit Definitions (PWMxCR)
  *
  * @{
  */

#define PWM_CE                         (1 << 0)            /*!< Counter Enable                   */
#define PWM_CR                         (1 << 1)            /*!< Counter Reset                    */
#define PWM_PWME                       (1 << 3)            /*!< PWM Enable                       */

/**
  * @}
  */

/** @defgroup PWM_MCR_Bit_Definitions PWM Match Control Register Bit Definitions (PWMxMCR)
  *
  * @{
  */

#define PWM_MR0I                       (1 << 0)            /*!< Interrupt when MR0 matches TC    */
#define PWM_MR0R                       (1 << 1)            /*!< Reset if MR0 matches TC          */
#define PWM_MR0S                       (1 << 2)            /*!< Stop if MR0 matches TC           */
#define PWM_MR1I                       (1 << 3)            /*!< Interrupt when MR1 matches TC    */
#define PWM_MR1R                       (1 << 4)            /*!< Reset if MR1 matches TC          */
#define PWM_MR1S                       (1 << 5)            /*!< Stop if MR1 matches TC           */
#define PWM_MR2I                       (1 << 6)            /*!< Interrupt when MR2 matches TC    */
#define PWM_MR2R                       (1 << 7)            /*!< Reset if MR2 matches TC          */
#define PWM_MR2S                       (1 << 8)            /*!< Stop if MR2 matches TC           */
#define PWM_MR3I                       (1 << 9)            /*!< Interrupt when MR3 matches TC    */
#define PWM_MR3R                       (1 << 10)           /*!< Reset if MR3 matches TC          */
#define PWM_MR3S                       (1 << 11)           /*!< Stop if MR3 matches TC           */
#define PWM_MR4I                       (1 << 12)           /*!< Interrupt when MR4 matches TC    */
#define PWM_MR4R                       (1 << 13)           /*!< Reset if MR4 matches TC          */
#define PWM_MR4S                       (1 << 14)           /*!< Stop if MR4 matches TC           */
#define PWM_MR5I                       (1 << 15)           /*!< Interrupt when MR5 matches TC    */
#define PWM_MR5R                       (1 << 16)           /*!< Reset if MR5 matches TC          */
#define PWM_MR5S                       (1 << 17)           /*!< Stop if MR5 matches TC           */
#define PWM_MR6I                       (1 << 18)           /*!< Interrupt when MR6 matches TC    */
#define PWM_MR6R                       (1 << 19)           /*!< Reset if MR6 matches TC          */
#define PWM_MR6S                       (1 << 20)           /*!< Stop if MR6 matches TC           */

/**
  * @}
  */

/** @defgroup PWM_PCR_Bit_Definitions PWM Control Register Bit Definitions (PWMxPCR)
  *
  * @{
  */

#define PWM_PWMSEL2                    (1 << 2)            /*!< Select Dbl. Edged Mode for PWM2  */
#define PWM_PWMSEL3                    (1 << 3)            /*!< Select Dbl. Edged Mode for PWM3  */
#define PWM_PWMSEL4                    (1 << 4)            /*!< Select Dbl. Edged Mode for PWM4  */
#define PWM_PWMSEL5                    (1 << 5)            /*!< Select Dbl. Edged Mode for PWM5  */
#define PWM_PWMSEL6                    (1 << 6)            /*!< Select Dbl. Edged Mode for PWM6  */
#define PWM_PWMENA1                    (1 << 9)            /*!< Enable PWM1 Output               */
#define PWM_PWMENA2                    (1 << 10)           /*!< Enable PWM2 Output               */
#define PWM_PWMENA3                    (1 << 11)           /*!< Enable PWM3 Output               */
#define PWM_PWMENA4                    (1 << 12)           /*!< Enable PWM4 Output               */
#define PWM_PWMENA5                    (1 << 13)           /*!< Enable PWM5 Output               */
#define PWM_PWMENA6                    (1 << 14)           /*!< Enable PWM6 Output               */

/**
  * @}
  */

/** @defgroup PWM_LER_Bit_Definitions PWM Latch Enable Register Bit Definitions
  *
  * @{
  */

#define PWM_M0LEN                      (1 << 0)            /*!< Enable PWM Match 0 Latch         */
#define PWM_M1LEN                      (1 << 1)            /*!< Enable PWM Match 1 Latch         */
#define PWM_M2LEN                      (1 << 2)            /*!< Enable PWM Match 2 Latch         */
#define PWM_M3LEN                      (1 << 3)            /*!< Enable PWM Match 3 Latch         */
#define PWM_M4LEN                      (1 << 4)            /*!< Enable PWM Match 4 Latch         */
#define PWM_M5LEN                      (1 << 5)            /*!< Enable PWM Match 5 Latch         */
#define PWM_M6LEN                      (1 << 6)            /*!< Enable PWM Match 6 Latch         */

/**
  * @}
  */

/**
  * @}
  */


/** @addtogroup I2C
  *
  * I2C Controller Peripheral
  *
  * @{
  */

#define LPC2XXX_HAS_I2C

/** @defgroup I2C_CONSET_Bit_Definitions (I2CxCONSET) I2C Control Set Register Bit Definitions
  *
  * @{
  */

#define I2C_CONSET_Mask                (0x7c)              /*!< Useable Bits in CONSET Register  */
#define I2C_CONSET_Shift               (0x00)

#define I2C_AA                         (1 << 2)            /*!< Assert Acknowledge Set Bit       */
#define I2C_SI                         (1 << 3)            /*!< I2C Interrupt Enable Bit         */
#define I2C_STO                        (1 << 4)            /*!< STOP Flag Set Bit                */
#define I2C_STA                        (1 << 5)            /*!< START Flag Set Bit               */
#define I2C_I2EN                       (1 << 6)            /*!< I2C Interface Enable Bit         */

/**
  * @}
  */

/** @defgroup I2C_CONCLR_Bit_Definitions (I2CxCONCLR) I2C Control Clear Register Bit Definitions
  *
  * @{
  */

#define I2C_CONCLR_Mask                (0x6e)              /*!< Useable Bits in CONCLR Register  */
#define I2C_CONCLR_Shift               (0x00)

#define I2C_AAC                        (1 << 2)            /*!< Assert Acknowledge Clear Bit     */
#define I2C_SIC                        (1 << 3)            /*!< I2C Interrupt Disable Flag       */
#define I2C_STAC                       (1 << 5)            /*!< START Flag Clear Bit             */
#define I2C_I2ENC                      (1 << 6)            /*!< I2C Interface Disable Bit        */

/**
  * @}
  */

/** @defgroup I2C_STAT_Bit_Definitions (I2CxSTAT) I2C Status Register Bit Definitions
  *
  * @{
  */

#define I2C_STAT_Mask                  (0xf8)              /*!< I2C Data Bits Mask               */
#define I2C_STAT_Shift                 (0)

/**
  * @}
  */

/** @defgroup I2C_DAT_Bit_Definitions (I2CxDAT) I2C Data Register Bit Definitions
  *
  * @{
  */

#define I2C_DAT_Mask                   (0xff)              /*!< I2C Data Bits Mask               */
#define I2C_DAT_Shift                  (0)

/**
  * @}
  */

/** @defgroup I2C_ADR_Bit_Definitions (I2CxADR) I2C Slave Address Register Bit Definitions
  *
  * @{
  */

#define I2C_ADDR_Mask                  (0x7f << 1)         /*!< Address 0 Mask (7 bits)          */
#define I2C_ADDR_Shift                 (1)

#define I2C_GC                         (1 << 0)            /*!< Gen. Call Enable (Answer 0x00)   */

/**
  * @}
  */

/** @defgroup I2C_SCLH_Bit_Definitions (I2CxSCLH) I2C Duty Cycle Register Bit Definitions High Half Word
  *
  * @{
  */

#define I2C_SCLH_Mask                  (0xffff)            /*!< Count for SCL High Time          */
#define I2C_SCLH_Shift                 (0)

/**
  * @}
  */

/** @defgroup I2C_SCLL_Bit_Definitions (I2CxSCLL) I2C Duty Cycle Register Bit Definitions Low Half Word
  *
  * @{
  */

#define I2C_SCLL_Mask                  (0xffff)            /*!< Count for SCL Low Time           */
#define I2C_SCLL_Shift                 (0)

/**
  * @}
  */

/**
  * @}
  */


/** @addtogroup SPI
  *
  * SPI Interface Peripheral
  *
  * @{
  */

#define LPC2XXX_HAS_SPI

/** @defgroup SPI_CR_Bit_Definitions (SPIxCR) SPI Control Register Bit Definitions
  *
  * @{
  */

#define SPI_CR_Mask                    (0xfc)              /*!< Useable Bits in SPICR            */
#define SPI_CR_Shift                   (0)

#define SPI_BITS_Mask                  (0x0f << 8)         /*!< SPI Word Size                    */
#define SPI_BITS_Shift                 (8)
#define SPI_BITS_8Bits                 (0x08 << 8)         /*!<  8 Bit Words                     */
#define SPI_BITS_9Bits                 (0x09 << 8)         /*!<  9 Bit Words                     */
#define SPI_BITS_10Bits                (0x0a << 8)         /*!< 10 Bit Words                     */
#define SPI_BITS_11Bits                (0x0b << 8)         /*!< 11 Bit Words                     */
#define SPI_BITS_12Bits                (0x0c << 8)         /*!< 12 Bit Words                     */
#define SPI_BITS_13Bits                (0x0d << 8)         /*!< 13 Bit Words                     */
#define SPI_BITS_14Bits                (0x0e << 8)         /*!< 14 Bit Words                     */
#define SPI_BITS_15Bits                (0x0f << 8)         /*!< 15 Bit Words                     */
#define SPI_BITS_16Bits                (0x00 << 8)         /*!< 16 Bit Words                     */

#define SPI_BITSEN                     (1 << 2)            /*!< SPI Bit Size Control Enable      */
#define SPI_CPHA                       (1 << 3)            /*!< Clock Phase Ctrl (0 = 1st edge)  */
#define SPI_CPOL                       (1 << 4)            /*!< Clock Polar Ctl (0 = SCK act hi) */
#define SPI_MSTR                       (1 << 5)            /*!< Master Mode                      */
#define SPI_LSBF                       (1 << 6)            /*!< LSB First (0 = MSB first)        */
#define SPI_SPIE                       (1 << 7)            /*!< SPI Interrupt Enable             */

/**
  * @}
  */

/** @defgroup SPI_SR_Bit_Definitions (SPIxSR) SPI Status Register Bit Definitions
  *
  * @{
  */

#define SPI_SR_Mask                    (0xf8)              /*!< Useable Bits in SPISR            */
#define SPI_SR_Shift                   (0)

#define SPI_ABRT                       (1 << 3)            /*!< Slave Abort Occurred             */
#define SPI_MODF                       (1 << 4)            /*!< Mode Fault                       */
#define SPI_ROVR                       (1 << 5)            /*!< Read Overrun                     */
#define SPI_WCOL                       (1 << 6)            /*!< Write Collision                  */
#define SPI_SPIF                       (1 << 7)            /*!< Transfer Complete                */

/**
  * @}
  */

/** @defgroup SPI_INT_Bit_Definitions (SPIxINT) SPI Interrupt Register Bit Definitions
  *
  * @{
  */

#define SPI_IT                         (1 << 0)            /*!< SPI Interrupt Flag               */

/**
  * @}
  */

/**
  * @}
  */


/** @addtogroup RTC
  *
  * Real Time Clock
  *
  * @{
  */

#define LPC2XXX_HAS_RTC

/** @defgroup RTC_ILR_Bit_Definitions (RTCILR) RTC Interrupt Location
  *
  * @{
  */

#define RTC_ILR_Mask                   (0x03)              /*!< Useable Bits in RTCILR           */
#define RTC_ILR_Shift                  (0)

#define RTC_RTCCIF                     (1 << 0)            /*!< Ctr Increment Interrupt Occurred */
#define RTC_RTCALF                     (1 << 1)            /*!< Alarm Regs Generated Interrupt   */

/**
  * @}
  */

/** @defgroup RTC_CTCR_Bit_Definitions (RTCCTCR) RTC Clock Tick Counter
  *
  * @{
  */

#define RTC_CTCR_Mask                  (0x7fff)            /*!< Counter is 15 bits               */
#define RTC_CTCR_Shift                 (0)

/**
  * @}
  */

/** @defgroup RTC_CCR_Bit_Definitions (RTCCCR) RTC Clock Control Register Bit Definitions
  *
  * @{
  */

#define RTC_CCR_Mask                   (0x1f)              /*!< Useable Bits in RTCCCR           */
#define RTC_CCR_Shift                  (0)

#define RTC_CTTEST_Mask                (0x03 << 2)         /*!< TEST Bits                        */
#define RTC_CTTEST_Shift               (2)

#define RTC_CLKEN                      (1 << 0)            /*!< Clock Enable                     */
#define RTC_CTCRST                     (1 << 1)            /*!< CTC Reset                        */
#define RTC_CLKSRC                     (1 << 4)            /*!< 0=>from prescaler, 1=>32KHzOsc   */

/**
  * @}
  */

/** @defgroup RTC_CIIR_Bit_Definitions (RTCCIIR) RTC Counter Increment Interrupt Register Bit Definitions
  *
  * @{
  */

#define RTC_CIIR_Mask                  (0x0f)              /*!< Useable Bits in CIIR             */
#define RTC_CIIR_Shift                 (0)

#define RTC_IMSEC                      (1 << 0)            /*!< Interrupt on Increment of Secs   */
#define RTC_IMMIN                      (1 << 1)            /*!< Interrupt on Increment of Mins   */
#define RTC_IMHOUR                     (1 << 2)            /*!< Interrupt on Increment of Hours  */
#define RTC_IMDOM                      (1 << 3)            /*!< Interrupt on Increment of MthDay */
#define RTC_IMDOW                      (1 << 4)            /*!< Interrupt on Increment of WkDay  */
#define RTC_IMDOY                      (1 << 5)            /*!< Interrupt on Increment of YrDay  */
#define RTC_IMMON                      (1 << 6)            /*!< Interrupt on Increment of Month  */
#define RTC_IMYEAR                     (1 << 7)            /*!< Interrupt on Increment of Years  */

/**
  * @}
  */

/** @defgroup RTC_AMR_Bit_Definitions (RTCAMR) RTC Alarm Mask Register Bit Definitions
  *
  * @{
  */

#define RTC_AMR_Mask                   (0x0f)              /*!< Useable Bits in AMR              */
#define RTC_AMR_Shift                  (0)

#define RTC_AMRSEC                     (1 << 0)            /*!< Don't Compare Seconds  for Alarm */
#define RTC_AMRMIN                     (1 << 1)            /*!< Don't compare Minutes  for Alarm */
#define RTC_AMRHOUR                    (1 << 2)            /*!< Don't compare Hours    for Alarm */
#define RTC_AMRDOM                     (1 << 3)            /*!< Don't compare MonthDay for Alarm */
#define RTC_AMRDOW                     (1 << 4)            /*!< Don't compare WeekDay  for Alarm */
#define RTC_AMRDOY                     (1 << 5)            /*!< Don't compare YearDay  for Alarm */
#define RTC_AMRMON                     (1 << 6)            /*!< Don't compare Months   for Alarm */
#define RTC_AMRYEAR                    (1 << 7)            /*!< Don't compare Years    for Alarm */

/**
  * @}
  */

/** @defgroup RTC_CTIME0_Bit_Definitions (RTCCTIME0) RTC Consolidated Time Register 0 Bit Definitions
  *   -- Seconds, Minutes, Hours, Day of Week
  *
  * @{
  */

#define RTC_CTIME0_Mask                (0x071f3f3fUL)      /*!< Useable Bits in CTIME0           */
#define RTC_CTIME0_Shift               (0)

#define RTC_CSEC_Mask                  (0x3fUL)            /*!< Consolidated Seconds Bit Mask    */
#define RTC_CSEC_Shift                 (0)

#define RTC_CMIN_Mask                  (0x3f << 8)         /*!< Consolidated Minutes Bit Mask    */
#define RTC_CMIN_Shift                 (8)

#define RTC_CHOUR_Mask                 (0x1f << 16)        /*!< Consolidated Hours Bit Mask      */
#define RTC_CHOUR_Shift                (16)

#define RTC_CDOW_Mask                  (0x07 << 24)        /*!< Consolidated Day/Wk Bit Mask     */
#define RTC_CDOW_Shift                 (24)

/**
  * @}
  */

/** @defgroup RTC_CTIME1_Bit_Definitions (RTCCTIME1) RTC Consolidated Time Register 1 Bit Definitions
  *
  * @{
  */

#define RTC_CTIME1_Mask                (0x0fff0f1fUL)      /*!< Useable Bits in CTIME1           */
#define RTC_CTIME1_Shift               (0)

#define RTC_CDOM_Mask                  (0x1fUL)            /*!< Consolidated Day/Mon Bit Mask    */
#define RTC_CDOM_Shift                 (0)

#define RTC_CMON_Mask                  (0x0fUL << 8)       /*!< Consolidated Months Bit Mask     */
#define RTC_CMON_Shift                 (8)

#define RTC_CYEAR_Mask                 (0x0fffUL << 16)    /*!< Consolidated Years Bit Mask      */
#define RTC_CYEAR_Shift                (16)

/**
  * @}
  */

/** @defgroup RTC_CTIME2_Bit_Definitions (RTCCTIME2) RTC Consolidated Time Register 2 Bit Definitions
  *
  * @{
  */

#define RTC_CTIME2_Mask                (0x0fffUL)          /*!< Useable Bits in CTIME2           */
#define RTC_CTIME2_Shift               (0)

#define RTC_CDOY_Mask                  (0x0fffUL)          /*!< Consolidated Day/Year Bit Mask   */
#define RTC_CDOY_Shift                 (0)

/**
  * @}
  */

/** @defgroup RTC_SEC_Bit_Definitions (RTCSEC) RTC Seconds Register Bit Definitions
  *
  * @{
  */

#define RTC_SEC_Mask                   (0x3f)              /*!< Seconds Mask                     */
#define RTC_SEC_Shift                  (0)

/**
  * @}
  */

/** @defgroup RTC_MIN_Bit_Definitions (RTCMIN) RTC Minutes Register Bit Definitions
  *
  * @{
  */

#define RTC_MIN_Mask                   (0x3f)              /*!< Minutes Mask                     */
#define RTC_MIN_Shift                  (0)

/**
  * @}
  */

/** @defgroup RTC_HOUR_Bit_Definitions (RTCHOUR) RTC Hours Register Bit Definitions
  *
  * @{
  */

#define RTC_HOUR_Mask                  (0x1f)              /*!< Hours Mask                       */
#define RTC_HOUR_Shift                 (0)

/**
  * @}
  */


/** @defgroup RTC_DOM_Bit_Definitions (RTCDOM) RTC Day of Month Register Bit Definitions
  *
  * @{
  */

#define RTC_DOM_Mask                   (0x1f)              /*!< Day of Month Mask                */
#define RTC_DOM_Shift                  (0)

/**
  * @}
  */


/** @defgroup RTC_DOW_Bit_Definitions (RTCDOW) RTC Day of Week Register Bit Definitions
  *
  * @{
  */

#define RTC_DOW_Mask                   (0x07)              /*!< Day of Week Mask                 */
#define RTC_DOW_Shift                  (0)

/**
  * @}
  */

/** @defgroup RTC_DOY_Bit_Definitions (RTCDOY) RTC Day of Year
  *
  * @{
  */

#define RTC_DOY_Mask                   (0x01ff)            /*!< Day of Year Mask                 */
#define RTC_DOY_Shift                  (0)

/**
  * @}
  */

/** @defgroup RTC_MONTH_Bit_Definitions (RTCMON) RTC Month Register Bit Definitions
  *
  * @{
  */

#define RTC_MONTH_Mask                 (0x0f)              /*!< Months Mask                      */
#define RTC_MONTH_Shift                (0)

/**
  * @}
  */

/** @defgroup RTC_YEAR_Bit_Definitions (RTCYEAR) RTC Year Register Bit Definitions
  *
  * @{
  */

#define RTC_YEAR_Mask                  (0x0fff)            /*!< Years Mask                       */
#define RTC_YEAR_Shift                 (0)

/**
  * @}
  */

/** @defgroup RTC_ALSEC_Bit_Definitions (RTCALSEC) RTC Alarm Seconds Register Bit Definitions
  *
  * @{
  */

#define RTC_ALSEC_Mask                 (0x3f)              /*!< Alarm Seconds Mask               */
#define RTC_ALSEC_Shift                (0)

/**
  * @}
  */

/** @defgroup RTC_ALMIN_Bit_Definitions (RTCALMIN) RTC Alarm Minutes Register Bit Definitions
  *
  * @{
  */

#define RTC_ALMIN_Mask                 (0x3f)              /*!< Alarm Minutes Mask               */
#define RTC_ALMIN_Shift                (0)

/**
  * @}
  */

/** @defgroup RTC_ALHOUR_Bit_Definitions (RTCALHOUR) RTC Alarm Hour Register Bit Definitions
  *
  * @{
  */

#define RTC_ALHOUR_Mask                (0x1f)              /*!< Alarm Hours Mask                 */
#define RTC_ALHOUR_Shift               (0)

/**
  * @}
  */


/** @defgroup RTC_ALDOM_Bit_Definitions (RTCALDOM) RTC Alarm Day of Month Register Bit Definitions
  *
  * @{
  */

#define RTC_ALDOM_Mask                 (0x1f)              /*!< Alarm Day of Month Mask          */
#define RTC_ALDOM_Shift                (0)

/**
  * @}
  */


/** @defgroup RTC_ALDOW_Bit_Definitions (RTCALDOW) RTC Alarm Day of Week Register Bit Definitions
  *
  * @{
  */

#define RTC_ALDOW_Mask                 (0x07)              /*!< Alarm Day of Week Mask           */
#define RTC_ALDOW_Shift                (0)

/**
  * @}
  */

/** @defgroup RTC_ALDOY_Bit_Definitions (RTCALDOY) RTC Alarm Day of Year Register Bit Definitions
  *
  * @{
  */

#define RTC_ALDOY_Mask                 (0x01ff)            /*!< Alarm Day of Year Mask           */
#define RTC_ALDOY_Shift                (0)

/**
  * @}
  */

/** @defgroup RTC_ALMONTH_Bit_Definitions (RTCALMONTH) RTC Alarm Month Register Bit Definitions
  *
  * @{
  */

#define RTC_ALMONTH_Mask               (0x0f)              /*!< Alarm Months Mask                */
#define RTC_ALMONTH_Shift              (0)

/**
  * @}
  */

/** @defgroup RTC_ALYEAR_Bit_Definitions (RTCALYEAR) RTC Alarm Year Register Bit Definitions
  *
  * @{
  */

#define RTC_ALYEAR_Mask                (0x0fff)            /*!< Alarm Years Mask                 */
#define RTC_ALYEAR_Shift               (0)

/**
  * @}
  */

/** @defgroup RTC_PREINT_Bit_Definitions (RTCPREINT) RTC Prescaler (Integer Portion) Register Bit Definitions
  *
  * @{
  */

#define RTC_PREINT_Mask                (0x1fff)            /*!< Prescaler Integer Mask           */
#define RTC_PREINT_Shift               (0)

/**
  * @}
  */

/** @defgroup RTC_PREFRAC_Bit_Definitions (RTCPREFRAC) RTC Prescaler (Fractional Portion) Register Bit Definitions
  *
  * @{
  */

#define RTC_PREFRAC_Mask               (0x7fff)            /*!< Prescaler Fraction Mask          */
#define RTC_PREFRAC_Shift              (0)

/**
  * @}
  */

/**
  * @}
  */


/** @addtogroup GPIO
  *
  * General Purpose I/O
  *
  * @{
  */

#define LPC2XXX_HAS_GPIO

/** @defgroup GPIO_PIN_Bit_Definitions / GPIO_SET_BIT_Definitions (GPIOxPIN, GPIOxSET) GPIO Pin Values Register, GPIO Set Register Bit Definitions
  *
  * @{
  */

#define GPIO_D0                        (1 << 0)            /*!< GPIO Pin 0  Data                 */
#define GPIO_D1                        (1 << 1)            /*!< GPIO Pin 1  Data                 */
#define GPIO_D2                        (1 << 2)            /*!< GPIO Pin 2  Data                 */
#define GPIO_D3                        (1 << 3)            /*!< GPIO Pin 3  Data                 */
#define GPIO_D4                        (1 << 4)            /*!< GPIO Pin 4  Data                 */
#define GPIO_D5                        (1 << 5)            /*!< GPIO Pin 5  Data                 */
#define GPIO_D6                        (1 << 6)            /*!< GPIO Pin 6  Data                 */
#define GPIO_D7                        (1 << 7)            /*!< GPIO Pin 7  Data                 */
#define GPIO_D8                        (1 << 8)            /*!< GPIO Pin 8  Data                 */
#define GPIO_D9                        (1 << 9)            /*!< GPIO Pin 9  Data                 */
#define GPIO_D10                       (1 << 10)           /*!< GPIO Pin 10 Data                 */
#define GPIO_D11                       (1 << 11)           /*!< GPIO Pin 11 Data                 */
#define GPIO_D12                       (1 << 12)           /*!< GPIO Pin 12 Data                 */
#define GPIO_D13                       (1 << 13)           /*!< GPIO Pin 13 Data                 */
#define GPIO_D14                       (1 << 14)           /*!< GPIO Pin 14 Data                 */
#define GPIO_D15                       (1 << 15)           /*!< GPIO Pin 15 Data                 */
#define GPIO_D16                       (1 << 16)           /*!< GPIO Pin 16 Data                 */
#define GPIO_D17                       (1 << 17)           /*!< GPIO Pin 17 Data                 */
#define GPIO_D18                       (1 << 18)           /*!< GPIO Pin 18 Data                 */
#define GPIO_D19                       (1 << 19)           /*!< GPIO Pin 19 Data                 */
#define GPIO_D20                       (1 << 20)           /*!< GPIO Pin 20 Data                 */
#define GPIO_D21                       (1 << 21)           /*!< GPIO Pin 21 Data                 */
#define GPIO_D22                       (1 << 22)           /*!< GPIO Pin 22 Data                 */
#define GPIO_D23                       (1 << 23)           /*!< GPIO Pin 23 Data                 */
#define GPIO_D24                       (1 << 24)           /*!< GPIO Pin 24 Data                 */
#define GPIO_D25                       (1 << 25)           /*!< GPIO Pin 25 Data                 */
#define GPIO_D26                       (1 << 26)           /*!< GPIO Pin 26 Data                 */
#define GPIO_D27                       (1 << 27)           /*!< GPIO Pin 27 Data                 */
#define GPIO_D28                       (1 << 28)           /*!< GPIO Pin 28 Data                 */
#define GPIO_D29                       (1 << 29)           /*!< GPIO Pin 29 Data                 */
#define GPIO_D30                       (1 << 30)           /*!< GPIO Pin 30 Data                 */
#define GPIO_D31                       (1 << 31)           /*!< GPIO Pin 31 Data                 */

/**
  * @}
  */

/** @defgroup GPIO_DIR_Bit_Definitions (GPIOxDIR) GPIO Pin Direction Register Bit Definitions
  *
  * @{
  */

#define GPIO_IO0                       (1 << 0)            /*!< GPIO Pin 0  Direction            */
#define GPIO_IO1                       (1 << 1)            /*!< GPIO Pin 1  Direction            */
#define GPIO_IO2                       (1 << 2)            /*!< GPIO Pin 2  Direction            */
#define GPIO_IO3                       (1 << 3)            /*!< GPIO Pin 3  Direction            */
#define GPIO_IO4                       (1 << 4)            /*!< GPIO Pin 4  Direction            */
#define GPIO_IO5                       (1 << 5)            /*!< GPIO Pin 5  Direction            */
#define GPIO_IO6                       (1 << 6)            /*!< GPIO Pin 6  Direction            */
#define GPIO_IO7                       (1 << 7)            /*!< GPIO Pin 7  Direction            */
#define GPIO_IO8                       (1 << 8)            /*!< GPIO Pin 8  Direction            */
#define GPIO_IO9                       (1 << 9)            /*!< GPIO Pin 9  Direction            */
#define GPIO_IO10                      (1 << 10)           /*!< GPIO Pin 10 Direction            */
#define GPIO_IO11                      (1 << 11)           /*!< GPIO Pin 11 Direction            */
#define GPIO_IO12                      (1 << 12)           /*!< GPIO Pin 12 Direction            */
#define GPIO_IO13                      (1 << 13)           /*!< GPIO Pin 13 Direction            */
#define GPIO_IO14                      (1 << 14)           /*!< GPIO Pin 14 Direction            */
#define GPIO_IO15                      (1 << 15)           /*!< GPIO Pin 15 Direction            */
#define GPIO_IO16                      (1 << 16)           /*!< GPIO Pin 16 Direction            */
#define GPIO_IO17                      (1 << 17)           /*!< GPIO Pin 17 Direction            */
#define GPIO_IO18                      (1 << 18)           /*!< GPIO Pin 18 Direction            */
#define GPIO_IO19                      (1 << 19)           /*!< GPIO Pin 19 Direction            */
#define GPIO_IO20                      (1 << 20)           /*!< GPIO Pin 20 Direction            */
#define GPIO_IO21                      (1 << 21)           /*!< GPIO Pin 21 Direction            */
#define GPIO_IO22                      (1 << 22)           /*!< GPIO Pin 22 Direction            */
#define GPIO_IO23                      (1 << 23)           /*!< GPIO Pin 23 Direction            */
#define GPIO_IO24                      (1 << 24)           /*!< GPIO Pin 24 Direction            */
#define GPIO_IO25                      (1 << 25)           /*!< GPIO Pin 25 Direction            */
#define GPIO_IO26                      (1 << 26)           /*!< GPIO Pin 26 Direction            */
#define GPIO_IO27                      (1 << 27)           /*!< GPIO Pin 27 Direction            */
#define GPIO_IO28                      (1 << 28)           /*!< GPIO Pin 28 Direction            */
#define GPIO_IO29                      (1 << 29)           /*!< GPIO Pin 29 Direction            */
#define GPIO_IO30                      (1 << 30)           /*!< GPIO Pin 30 Direction            */
#define GPIO_IO31                      (1 << 31)           /*!< GPIO Pin 31 Direction            */

/**
  * @}
  */

/**
  * @}
  */


/** @addtogroup PINSEL
  *
  * Pin Function Select Block
  *
  * @{
  */

#define LPC2XXX_HAS_PINSEL

/** @defgroup PINSEL_Function_Definitions Pin Select Register Function Settings (for PINSEL0, PINSEL1)
  *
  * @{
  */

#define PINSEL_SEL_DEFAULT             (0x00)              /*!< Default Pin Function             */
#define PINSEL_SEL_ALT1                (0x01)              /*!< Alternate Pin Function #1        */
#define PINSEL_SEL_ALT2                (0x02)              /*!< Alternate Pin Function #2        */
#define PINSEL_SEL_ALT3                (0x03)              /*!< Alternate Pin Function #3        */

/**
  * @}
  */

/** @defgroup PINSEL0_Bit_Definitions (PINSEL0) Pin Select Register 0 Bit Definitions (P0.0 - P0.15)
  *
  * @{
  */

#define PINSEL_P0_0_Mask               (0x03UL)            /*!< P0.0 Pin Function                */
#define PINSEL_P0_0_Shift              (0)
#define PINSEL_P0_1_Mask               (0x03UL << 2)       /*!< P0.1 Pin Function                */
#define PINSEL_P0_1_Shift              (2)
#define PINSEL_P0_2_Mask               (0x03UL << 4)       /*!< P0.2 Pin Function                */
#define PINSEL_P0_2_Shift              (4)
#define PINSEL_P0_3_Mask               (0x03UL << 6)       /*!< P0.3 Pin Function                */
#define PINSEL_P0_3_Shift              (6)
#define PINSEL_P0_4_Mask               (0x03UL << 8)       /*!< P0.4 Pin Function                */
#define PINSEL_P0_4_Shift              (8)
#define PINSEL_P0_5_Mask               (0x03UL << 10)      /*!< P0.5 Pin Function                */
#define PINSEL_P0_5_Shift              (10)
#define PINSEL_P0_6_Mask               (0x03UL << 12)      /*!< P0.6 Pin Function                */
#define PINSEL_P0_6_Shift              (12)
#define PINSEL_P0_7_Mask               (0x03UL << 14)      /*!< P0.7 Pin Function                */
#define PINSEL_P0_7_Shift              (14)
#define PINSEL_P0_8_Mask               (0x03UL << 16)      /*!< P0.8 Pin Function                */
#define PINSEL_P0_8_Shift              (16)
#define PINSEL_P0_9_Mask               (0x03UL << 18)      /*!< P0.9 Pin Function                */
#define PINSEL_P0_9_Shift              (18)
#define PINSEL_P0_10_Mask              (0x03UL << 20)      /*!< P0.10 Pin Function               */
#define PINSEL_P0_10_Shift             (20)
#define PINSEL_P0_11_Mask              (0x03UL << 22)      /*!< P0.11 Pin Function               */
#define PINSEL_P0_11_Shift             (22)
#define PINSEL_P0_12_Mask              (0x03UL << 24)      /*!< P0.12 Pin Function               */
#define PINSEL_P0_12_Shift             (24)
#define PINSEL_P0_13_Mask              (0x03UL << 26)      /*!< P0.13 Pin Function               */
#define PINSEL_P0_13_Shift             (26)
#define PINSEL_P0_14_Mask              (0x03UL << 28)      /*!< P0.14 Pin Function               */
#define PINSEL_P0_14_Shift             (28)
#define PINSEL_P0_15_Mask              (0x03UL << 30)      /*!< P0.15 Pin Function               */
#define PINSEL_P0_15_Shift             (30)

/**
  * @}
  */

/** @defgroup PINSEL1_Bit_Definitions (PINSEL1) Pin Select Register 1 Bit Definitions (P0.15 - P0.31)
  *
  * @{
  */

#define PINSEL_P0_16_Mask              (0x03UL)            /*!< P0.16 Pin Function               */
#define PINSEL_P0_16_Shift             (0)
#define PINSEL_P0_17_Mask              (0x03UL << 2)       /*!< P0.17 Pin Function               */
#define PINSEL_P0_17_Shift             (2)
#define PINSEL_P0_18_Mask              (0x03UL << 4)       /*!< P0.18 Pin Function               */
#define PINSEL_P0_18_Shift             (4)
#define PINSEL_P0_19_Mask              (0x03UL << 6)       /*!< P0.19 Pin Function               */
#define PINSEL_P0_19_Shift             (6)
#define PINSEL_P0_20_Mask              (0x03UL << 8)       /*!< P0.20 Pin Function               */
#define PINSEL_P0_20_Shift             (8)
#define PINSEL_P0_21_Mask              (0x03UL << 10)      /*!< P0.21 Pin Function               */
#define PINSEL_P0_21_Shift             (10)
#define PINSEL_P0_22_Mask              (0x03UL << 12)      /*!< P0.22 Pin Function               */
#define PINSEL_P0_22_Shift             (12)
#define PINSEL_P0_23_Mask              (0x03UL << 14)      /*!< P0.23 Pin Function               */
#define PINSEL_P0_23_Shift             (14)
#define PINSEL_P0_24_Mask              (0x03UL << 16)      /*!< P0.24 Pin Function               */
#define PINSEL_P0_24_Shift             (16)
#define PINSEL_P0_25_Mask              (0x03UL << 18)      /*!< P0.25 Pin Function               */
#define PINSEL_P0_25_Shift             (18)
#define PINSEL_P0_26_Mask              (0x03UL << 20)      /*!< P0.26 Pin Function               */
#define PINSEL_P0_26_Shift             (20)
#define PINSEL_P0_27_Mask              (0x03UL << 22)      /*!< P0.27 Pin Function               */
#define PINSEL_P0_27_Shift             (22)
#define PINSEL_P0_28_Mask              (0x03UL << 24)      /*!< P0.28 Pin Function               */
#define PINSEL_P0_28_Shift             (24)
#define PINSEL_P0_29_Mask              (0x03UL << 26)      /*!< P0.29 Pin Function               */
#define PINSEL_P0_29_Shift             (26)
#define PINSEL_P0_30_Mask              (0x03UL << 28)      /*!< P0.30 Pin Function               */
#define PINSEL_P0_30_Shift             (28)
#define PINSEL_P0_31_Mask              (0x03UL << 30)      /*!< P0.31 Pin Function               */
#define PINSEL_P0_31_Shift             (30)

/**
  * @}
  */

/** @defgroup PINSEL2_Bit_Definitions (PINSEL2) Pin Select Register 2 Bit Definitions (Debug / Trace)
  *
  * @{
  */

#define PINSEL_DEBUGEN                 (1 << 2)            /*!< Enable Debug on P1.36-26         */
#define PINSEL_TRACEEN                 (1 << 3)            /*!< Enable Trace on P1.25-16         */

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup SYSCON
  *
  * System Control Block
  *
  * @{
  */

#define LPC2XXX_HAS_SYSCON

/** @defgroup SYSCON_MAM_CR_Bit_Definitions (MAMCR) SYSCON MAM Flash Accelerator Control Register Bit Definitions
  *
  * @{
  */

#define SYSCON_MAMMODE_Mask            (0x03)              /*!< Memory Accelerator Control Mode  */
#define SYSCON_MAMMODE_Shift           (0)

#define SYSCON_MAMMODE_DIS             (0x00)              /*!< MAM Disabled                     */
#define SYSCON_MAMMODE_PART            (0x01)              /*!< MAM Partially Enabled            */
#define SYSCON_MAMMODE_ENA             (0x02)              /*!< MAM Fully Enabled                */

/**
  * @}
  */

/** @defgroup SYSCON_MAM_TIM_Bit_Definitions (MAMTIM) SYSCON MAM Flash Accelerator Timing Control Register Bit Definitions
  *
  * @{
  */

#define SYSCON_MAMTIM_Mask             (0x07)              /*!< Memory Accelerator Flash Timing  */
#define SYSCON_MAMTIM_Shift            (0)

#define SYSCON_MAMTIM_1CLK             (0x01)              /*!< Flash Accesses => 1 CPU Clock    */
#define SYSCON_MAMTIM_2CLK             (0x02)              /*!< Flash Accesses => 2 CPU Clocks   */
#define SYSCON_MAMTIM_3CLK             (0x03)              /*!< Flash Accesses => 3 CPU Clocks   */
#define SYSCON_MAMTIM_4CLK             (0x04)              /*!< Flash Accesses => 4 CPU Clocks   */
#define SYSCON_MAMTIM_5CLK             (0x05)              /*!< Flash Accesses => 5 CPU Clocks   */
#define SYSCON_MAMTIM_6CLK             (0x06)              /*!< Flash Accesses => 6 CPU Clocks   */
#define SYSCON_MAMTIM_7CLK             (0x07)              /*!< Flash Accesses => 7 CPU Clocks   */

/**
  * @}
  */

/** @defgroup SYSCON_EXTINT_Bit_Definitions (EXTINT) SYSCON External Interrupt Flag Register Bit Definitions
  *
  * @{
  */

#define SYSCON_EINT_Mask               (0x0f)              /*!< External Interrupt Bits          */
#define SYSCON_EINT_Shift              (0)

#define SYSCON_EINT0                   (1 << 0)            /*!< External Interrupt 0 Flag        */
#define SYSCON_EINT1                   (1 << 1)            /*!< External Interrupt 1 Flag        */
#define SYSCON_EINT2                   (1 << 2)            /*!< External Interrupt 2 Flag        */

/**
  * @}
  */

/** @defgroup SYSCON_INTWAKE_Bit_Definitions (INTWAKE) SYSCON Interrupt Wakeup Register Bit Definitions
  *
  * @{
  */

#define SYSCON_EXTWAKE_Mask            (0x0c0f)            /*!< External Wake Enable Bits        */
#define SYSCON_EXTWAKE_Shift           (0)

#define SYSCON_EXTWAKE0                (1 << 0)            /*!< External Wake Enable on EINT0    */
#define SYSCON_EXTWAKE1                (1 << 1)            /*!< External Wake Enable on EINT1    */
#define SYSCON_EXTWAKE2                (1 << 2)            /*!< External Wake Enable on EINT2    */
#define SYSCON_BODWAKE                 (1 << 14)           /*!< Enable BOD Interrupt to Wake CPU */
#define SYSCON_RTCWAKE                 (1 << 15)           /*!< Enable RTC Interrupt to Wake CPU */

/**
  * @}
  */

/** @defgroup SYSCON_EXTMODE_Bit_Definitions (EXTMODE) SYSCON External Interrupt Mode Register Bit Definitions
  *
  * @{
  */

#define SYSCON_EXTMODE_Mask            (0x07)              /*!< Useable Bits in EXTMODE          */
#define SYSCON_EXTMODE_Shift           (0)

#define SYSCON_EXTMODE0                (1 << 0)            /*!< 0=>EINT0 level sense, 1=>edge    */
#define SYSCON_EXTMODE1                (1 << 1)            /*!< 0=>EINT1 level sense, 1=>edge    */
#define SYSCON_EXTMODE2                (1 << 2)            /*!< 0=>EINT2 level sense, 1=>edge    */

/**
  * @}
  */

/** @defgroup SYSCON_EXTPOLAR_Bit_Definitions (EXTPOLAR) SYSCON External Interrupt Polarity Control Register Bit Definitions
  *
  * @{
  */

#define SYSCON_EXTPOLAR_Mask           (0x07)              /*!< Useable Bits in EXTPOLAR         */
#define SYSCON_EXTPOLAR_Shift          (0)

#define SYSCON_EXTPOLAR0               (1 << 0)            /*!< 0=>EINT0 Senses LO, 1=>Senses HI */
#define SYSCON_EXTPOLAR1               (1 << 1)            /*!< 0=>EINT1 Senses LO, 1=>Senses HI */
#define SYSCON_EXTPOLAR2               (1 << 2)            /*!< 0=>EINT2 Senses LO, 1=>Senses HI */

/**
  * @}
  */

/** @defgroup SYSCON_MEMMAP_Bit_Definitions (MEMMAP) SYSCON Memory Remap Register Bit Definitions
  *
  * @{
  */

#define SYSCON_MEMMAP_Mask             (0x03)             /*!< Memory Remap Control Bits         */
#define SYSCON_MEMMAP_Shift            (0)

#define SYSCON_MEMMAP_BOOTLOADER       (0x00)             /*!< Interrupt Vectors Mapped Bootload */
#define SYSCON_MEMMAP_FLASH            (0x01)             /*!< Interrupt Vectors Mapped to Flash */
#define SYSCON_MEMMAP_RAM              (0x02)             /*!< Interrupt Vectors Mapped to RAM   */

/**
  * @}
  */

/** @defgroup SYSCON_PLLCON_Bit_Definitions (PLLCON) SYSCON System PLL Control Register Bit Definitions
  *
  * @{
  */

#define SYSCON_PLLCON_Mask             (0x03)              /*!< Useable Bits in PLLCON           */
#define SYSCON_PLLCON_Shift            (0)

#define SYSCON_PLLCON_PLLE             (1 << 0)            /*!< PLL Enable                       */
#define SYSCON_PLLCON_PLLC             (1 << 1)            /*!< PLL Connect                      */

/**
  * @}
  */

/** @defgroup SYSCON_PLLCFG_Bit_Definitions (PLLCFG) SYSCON System PLL Configuration Register Bit Definitions
  *
  * @{
  */

#define SYSCON_PLLCFG_Mask             (0x7f)              /*!< Useable Bits in PLLCFG           */
#define SYSCON_PLLCFG_Shift            (0)

#define SYSCON_PLLCFG_MSEL_Mask        (0x1f)              /*!< PLL Multiplier Value             */
#define SYSCON_PLLCFG_MSEL_Shift       (0)

#define SYSCON_PLLCFG_PSEL_Mask        (0x03 << 5)         /*!< PLL Divider Value                */
#define SYSCON_PLLCFG_PSEL_Shift       (5)

/**
  * @}
  */

/** @defgroup SYSCON_PLLSTAT_Bit_Definitions (PLLSTAT) SYSCON System PLL Status Register Bit Definitions
  *
  * @{
  */
#define SYSCON_PLLSTAT_Mask            (0x077f)
#define SYSCON_PLLSTAT_Shift           (0)

#define SYSCON_PLLSTAT_MSEL_Mask       (0x1f)              /*!< PLL Multiplier Value             */
#define SYSCON_PLLSTAT_MSEL_Shift      (0)

#define SYSCON_PLLSTAT_PSEL_Mask       (0x03 << 5)         /*!< PLL Divider Value                */
#define SYSCON_PLLSTAT_PSEL_Shift      (5)
#define SYSCON_PLLSTAT_PSEL_1          (0x00)
#define SYSCON_PLLSTAT_PSEL_2          (0x01 << 5)
#define SYSCON_PLLSTAT_PSEL_4          (0x02 << 5)
#define SYSCON_PLLSTAT_PSEL_8          (0x03 << 5)

#define SYSCON_PLLSTAT_PLLE            (1 << 8)            /*!< PLL is Enabled Flag              */
#define SYSCON_PLLSTAT_PLLC            (1 << 9)            /*!< PLL is Connected Flag            */
#define SYSCON_PLLSTAT_PLOCK           (1 << 10)           /*!< PLL is Locked Flag               */

/**
  * @}
  */

/** @defgroup SYSCON_PCON_Bit_Definitions (PCON) SYSCON (System) Power Control Register Bit Definitions
  *
  * @{
  */

#define SYSCON_PCON_Mask               (0x03)              /*!< Useable Bits in PCON             */
#define SYSCON_PCON_Shift              (0)

#define SYSCON_PCON_IDL                (1 << 0)            /*!< Enable Power Idle Mode           */
#define SYSCON_PCON_PD                 (1 << 1)            /*!< Enable Power-Down Mode           */
#define SYSCON_PCON_PDBOD              (1 << 2)            /*!< Disable BOD on Power-Down        */

/**
  * @}
  */

/** @defgroup SYSCON_PCONP_Bit_Definitions (PCONP) SYSCON Peripheral Power Control Register Bit Definitions
  *
  * @{
  */

#define SYSCON_PCONP_Mask              (0x0000003beUL)     /*!< Useable Bits in PCONP            */
#define SYSCON_PCONP_Shift             (0)

#define SYSCON_PCONP_PCTIM0            (1UL << 1)          /*!< Enable Timer 0                   */
#define SYSCON_PCONP_PCTIM1            (1UL << 2)          /*!< Enable Timer 1                   */
#define SYSCON_PCONP_PCUART0           (1UL << 3)          /*!< Enable UART 0                    */
#define SYSCON_PCONP_PCUART1           (1UL << 4)          /*!< Enable UART 1                    */
#define SYSCON_PCONP_PCPWM0            (1UL << 5)          /*!< Enable PWM                       */
#define SYSCON_PCONP_PCI2C0            (1UL << 7)          /*!< Enable I2C0                      */
#define SYSCON_PCONP_PCSPI0            (1UL << 8)          /*!< Enable SPI0                      */
#define SYSCON_PCONP_PCRTC             (1UL << 9)          /*!< Enable Real Time Clock           */

#define SYSCON_PCONP_PCSPI1            (SYSCON_PCONP_PCSSP0) /*!< Weird NXP Naming               */

/**
  * @}
  */

/** @defgroup SYSCON_APBDIV_Bit_Definitions (APBDIV) SYSCON APB Divider Register Bit Definitions
  *
  * @{
  */

/*! @brief SYSCON APB Divider Register (APBDIV) Bit Masks & Definitions */
#define SYSCON_APBDIV_Mask   (0x03)    /*!< Peripheral Clock Divider Mask          */
#define SYSCON_APBDIV_Shift  (0)

#define SYSCON_APBDIV_DIV4   (0x00)    /*!< Divide CPU clock by 4 for Periph Clock */
#define SYSCON_APBDIV_DIV1   (0x01)    /*!< CPU Clock used for Periph Clock        */
#define SYSCON_APBDIV_DIV2   (0x02)    /*!< Divide CPU clock by 2 for Periph Clock */

/**
  * @}
  */

/**
  * @}
  */


/** @addtogroup VIC
  *
  * Vectored Interrupt Controller
  *
  * @{
  */

#define LPC2XXX_HAS_VIC

/** @defgroup VICVECTCNTL0-15_Bit_Definitions (VICVECTCNTLn) VIC Vector Control Registers 0-15 Bit Definitions
  *
  * @{
  */

#define VICVECTCNTL_Mask               (0x3f)              /* Useable Bits in VICVECTCNTL Reg's  */
#define VICVECTCNTL_Shift              (0)

#define VIC_IRQNUM_Mask                (0x1f)              /*!< # of Int. Req. or Software Int   */
#define VIC_IRQNUM_Shift               (0)

#define VIC_IRQEN                      (1 << 5)            /*!< Enable this IRQ slot             */

/**
  * @}
  */

/** @defgroup VICPROTECTION_Bit_Definitions (VICPROTECTION) VIC Protection Register Bit Definitions
  *
  * @{
  */

#define VIC_ACCESS                     (1 << 0)            /*!< VIC Accesses Restricted          */

/**
  * @}
  */

/**
  * @}
  */


/* Peripheral Base Memory Addresses -------------------------------------------------------------*/

/** @defgroup LPC2106_PeripheralMemoryLocations LPC2106/5/4 Peripheral Memory Locations
  * @{
  */

#define FIO0_BASE       (0x3fffC000UL)                     /*!< Fast IO Port 0                   */
#define WDT_BASE        (0xe0000000UL)                     /*!< Watchdog Timer                   */
#define TIMER0_BASE     (0xe0004000UL)                     /*!< TIMER 0                   */
#define TIMER1_BASE     (0xe0008000UL)                     /*!< TIMER 1                   */
#define UART0_BASE      (0xe000c000UL)                     /*!< UART 0                           */
#define UART1_BASE      (0xe0010000UL)                     /*!< UART 1                           */
#define PWM0_BASE       (0xe0014000UL)                     /*!< PWM Controller 0                 */
#define I2C0_BASE       (0xe001c000UL)                     /*!< I2C Controller 0                 */
#define SPI0_BASE       (0xe0020000UL)                     /*!< SPI Interface 0                  */
#define RTC_BASE        (0xe0024000UL)                     /*!< Real Time Clock                  */
#define GPIO0_BASE      (0xe0028000UL)                     /*!< GPIO Port 0                      */
#define PINSEL_BASE     (0xe002c000UL)                     /*!< Pin Function Selection Block     */
#define SYSCON_BASE     (0xe01fc000UL)                     /*!< System Control Block             */
#define VIC_BASE        (0xfffff000UL)                     /*!< Interrupt Controller             */

/**
  * @}
  */

/* Peripheral Instances -------------------------------------------------------------------------*/

/** @defgroup LPC2106_PeripheralInstances LPC2106/5/4 Peripheral Instances
  * @{
  */

#define FIO0            ((FIO_Type *)FIO0_BASE)            /*!< Fast IO Port 0                   */
#define WDT             ((WDT_Type *)WDT_BASE)             /*!< Watchdog Timer                   */
#define TIMER0          ((TIMER_Type *)TIMER0_BASE)        /*!< 32-Bit Timer 0                   */
#define TIMER1          ((TIMER_Type *)TIMER1_BASE)        /*!< 32-Bit Timer 1                   */
#define UART0           ((UART_Type *)UART0_BASE)          /*!< UART 0                           */
#define UART1           ((UART_Type *)UART1_BASE)          /*!< UART 1                           */
#define PWM0            ((PWM_Type *)PWM0_BASE)            /*!< PWM Controller 0                 */
#define I2C0            ((I2C_Type *)I2C0_BASE)            /*!< I2C Controller 0                 */
#define SPI0            ((SPI_Type *)SPI0_BASE)            /*!< SPI Interface 0                  */
#define RTC             ((RTC_Type *)RTC_BASE)             /*!< Real Time Clock                  */
#define GPIO0           ((GPIO_Type *)GPIO0_BASE)          /*!< GPIO Port 0                      */
#define PINSEL          ((PINSEL_Type *)PINSEL_BASE)       /*!< Pin Function Selection Block     */
#define SYSCON          ((SYSCON_Type *)SYSCON_BASE)       /*!< System Control Block             */
#define VIC             ((VIC_Type *)VIC_BASE)             /*!< Interrupt Controller             */

/**
  * @}
  */


#ifdef __cplusplus
};
#endif

#endif /* #ifndef LPC2106_H_ */
