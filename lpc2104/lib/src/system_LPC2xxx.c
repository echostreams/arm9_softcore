/******************************************************************************
 * @file:    system_LPC2xxxx.c
 * @purpose: LPC2xxx MCU Initialization Functions 
 * @version: V1.0
 * @author:  Tymm Twillman
 * @date:    31. December 2011
 *
 * This file contains system utility functions for LPC2xxxx MCU's.
 *
 *****************************************************************************/
 
/* Includes -----------------------------------------------------------------*/

#include <stddef.h>
#include "LPC2xxx.h"
#include "system_LPC2xxx.h"
//#include "LPC2xxx_syscon.h"
#include "LPC2xxx_vic.h"


/* Sanity Checks ------------------------------------------------------------*/

/* If no APB clock divisor has been set, default to 4. */
#ifndef APBCLKDIV_Val
# define APBCLKDIV_Val 4
#endif

/* If a High Speed External Oscillator speed was given, it takes precedence
 *  over MCUOSC_Val (MCU Oscillator speed).
 *
 *  (This platform doesn't have an internal oscillator.)
 */
#ifdef HSE_Val
# define MCUOSC_Val HSE_Val
#endif

#ifndef MCUOSC_Val
# error "No MCUOSC_Val (MCU Oscillator Speed) or HSE_Val (High Speed External Oscillator Speed) set!"
#endif


/* File Local Types ---------------------------------------------------------*/

/** @brief Internal type for PLL register accesses.
  *
  * PLL's are part of SYSCON block but have a shared register layout.
  */
typedef struct PLL {
    __IO    uint32_t    PLLCON;      /*!< Offset: 0x080 PLL Control Register */
    __IO    uint32_t    PLLCFG;      /*!< Offset: 0x084 PLL Config Register  */
    __I     uint32_t    PLLSTAT;     /*!< Offset: 0x088 PLL Status Register  */
    __O     uint32_t    PLLFEED;     /*!< Offset: 0x08c PLL Feed Register    */
} PLL_Type;


/* Global Variables ---------------------------------------------------------*/

/* System clocking default values */
uint32_t ClockSource        = MCUOSC_Val;
uint32_t SystemCoreClock    = MCUOSC_Val;

/*! Counter for spurious IRQ's (IRQ's for which there is no handler set up) */
volatile int SpuriousIRQCount = 0;


/* Functions ----------------------------------------------------------------*/

/** @brief  PLL configuration function (used by SysPLL_Config and USBPLL_Config)
  *
  * @param  [in]  pll      Location of the PLL registers
  * @param  [in]  pll_val  The PLL configuration value to write
  *
  * @return None.
  *
  * This function will set the new PLL configuration, enable it, wait for the
  *  PLL to lock and then connect it.
  */
static void PLL_Config(PLL_Type *PLL, int8_t pll_val)
{
    /* Reserved bit (shouldn't be set) -- also
     *  can mean negative value returned from
     *  pll_calc to indicate invalid value, in
     *  which case don't want to set up the PLL
     *  with that value anyhow.
     */
    if (pll_val & 0x80) {
        return;
    }

    __asm__ __volatile__ (
        "    mov     r0, %0        \r\n"
        "    mov     r1, %1        \r\n"
        "    mov     r2, #0xaa     \r\n"
        "    mov     r3, #0x55     \r\n"

        /* Set new PLL config and enable */
        "    str     r1, [r0, %3]  \r\n"
        "    mov     r1, %6        \r\n"
        "    str     r1, [r0, %2]  \r\n"
        "    str     r2, [r0, %5]  \r\n"
        "    str     r3, [r0, %5]  \r\n"

        /* Wait for PLL to lock */
        "1:  ldr     r1, [r0, %4]  \r\n"
        "    ands    r1, r1, %8    \r\n"
        "    beq     1b            \r\n"

        /* Connect the PLL */
        "    mov     r1, %7        \r\n"
        "    str     r1, [r0, %2]  \r\n"
        "    str     r2, [r0, %5]  \r\n"
        "    str     r3, [r0, %5]  \r\n"
    :
    : "r" (PLL),
      "r" (pll_val),
      "i" (0),
      "i" (offsetof(PLL_Type, PLLCFG)),
      "i" (offsetof(PLL_Type, PLLSTAT)),
      "i" (offsetof(PLL_Type, PLLFEED)),
      "i" (SYSCON_PLLCON_PLLE),
      "i" (SYSCON_PLLCON_PLLE | SYSCON_PLLCON_PLLC),
      "i" (SYSCON_PLLSTAT_PLOCK)
    :"r0","r1","r2","r3");
}


/* Keep Doxygen from documenting file static functions
 *  (while still picking up static inlines in headers)
 */
#ifndef DOXYGEN_SHOULD_SKIP_THIS 

#if MCUOSC_Val != F_CPU

/** @brief  Initialize system PLL
  * @param  None.
  * @return None.
  *
  * Sets up main microcontroller PLL for operation at <F_CPU> Hz
  *  from system oscillator running at <MCUOSC_Val> Hz
  */
static inline void SysPLLInit(void)
{
    uint8_t PLLVal;
    
    
    PLLVal = (F_CPU / MCUOSC_Val) - 1;
    
#if   (F_CPU < 19500000)
    PLLVal |= SYSCON_PLLSTAT_PSEL_8;
#elif (F_CPU < 40000000)
    PLLVal |= SYSCON_PLLSTAT_PSEL_4;
#else
    PLLVal |= SYSCON_PLLSTAT_PSEL_2;
#endif
    
    SysPLL_Config(PLLVal);
}
#endif /* #if MCUOSC_Val != F_CPU */

#endif /* #ifndef DOXYGEN_SHOULD_SKIP_THIS */


/** @brief Catches Spurious IRQs
  * @param None.
  * @return None.
  *
  * Just increment the spurious IRQ counter.
  */
void Default_IRQHandler(void) __attribute__ ((interrupt ("IRQ"))) __attribute__ ((weak));
void Default_IRQHandler(void)
{    
    SpuriousIRQCount++;
    printf("IRQ Count: %d\n", SpuriousIRQCount);
    /* Acknowledge the IRQ */
    VIC_IRQDone();
}


/** @brief  Configure the MCU System PLL with the given scaler.
  *
  * @param  [in]  pll_scaler  The PLL scaler value to write
  *
  * @return None.
  *
  * Wrapper for PLL_Config to update the System PLL.
  */
void SysPLL_Config(int8_t pll_scaler)
{
#ifdef LPC2XXX_HAS_USB   /* USB parts have numbered PLL register naming */
    PLL_Config((PLL_Type *)(&(SYSCON->PLL0CON)), pll_scaler);
#else
    PLL_Config((PLL_Type *)(&(SYSCON->PLLCON)), pll_scaler);
#endif
}


#ifdef LPC2XXX_HAS_USB
/** @brief  Configure the USB PLL with the given scaler.
  *
  * @param  [in]  pll_scaler  The PLL scaler value to write
  *
  * @return None.
  *
  * Wrapper for PLL_Config to update the USB PLL.
  */
void USBPLL_Config(int8_t pll_scaler)
{
    PLL_Config((PLL_Type *)(&(SYSCON->PLL1CON)), pll_scaler);
}
#endif


/** @brief Set the SystemCoreClock variable based on current clock config
  * @param None.
  * @return None.
  *
  * Sets SystemCoreClock based on values in clock configuration registers.
  */ 
void SystemCoreClockUpdate(void)
{
/*
    uint32_t clockSpeed;
    

    clockSpeed = ClockSource;
    
    if (SYSCON_SysPLLIsEnabled()) {
        clockSpeed *= (uint32_t)SYSCON_GetSysPLLMVal();
    }
    
    SystemCoreClock = clockSpeed;
*/
}


/** @brief Initialize system PLL & other necessary clocks
  * @param None.
  * @return None.
  * Sets up necessary system clocks and basic hardware.
  */
void SystemInit(void) __attribute__((__weak__));
void SystemInit(void)
{
#ifdef CONFIG_LPC2XXX_BOOT_DELAY
    int i;

    for (i = 0; i < 500000; i++) {
        __asm__("    nop\r\n");
    }
#endif

#if 0

#if defined(__DEBUG_RAM)
    SYSCON_SetMemRemap(SYSCON_MemRemap_RAM);
#elif defined(__DEBUG_FLASH)
    SYSCON_SetMemRemap(SYSCON_MemRemap_FLASH);
#endif

    ClockSource = MCUOSC_Val;

    /* Enable Memory Accelerator
     *     => full speed, 3 cycles per flash fetch
     */
    SYSCON_SetMAMMode(SYSCON_MAMMode_Disabled);
    SYSCON_SetMAMFlashAccessCycles(3);
    SYSCON_SetMAMMode(SYSCON_MAMMode_Full);

#if MCUOSC_Val != F_CPU
    SysPLLInit();
#endif

    SystemCoreClock = F_CPU;

    /* Todo? Reset MAM Settings? (decrease flash access cycles maybe?) */

    SYSCON_SetAPBClockDivider(APBCLKDIV_Val);

    /* Make sure no IRQs are classified as FIQ's yet, just to be sure */
    VIC_ClearFIQs();

    /* Set the Default IRQ Handler to the Spurious IRQ Counter */
    VIC_SetDefaultIRQHandler(Default_IRQHandler);
#endif
}
