/******************************************************************************
 * @file:    lpc2xxx_crt0.s
 * @purpose: CPU Initialization Code for LPC2000 Microcontrollers
 * @version: V1.0
 * @author:  Tymm Twillman
 * @date:    31. December 2011
 * @license: Simplified BSD License
 *
 * This file has the initial code to be executed when the microcontroller
 *  is reset / powered on, and the default interrupt / exception vectors.
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

/*
 * Sizes of Stacks for different modes
 *  (change if you need to)
 */
.equ    UND_Stack_Size,    0x0
.equ    ABT_Stack_Size,    0x20
.equ    SVC_Stack_Size,    0x20
.equ    IRQ_Stack_Size,    0x20
.equ    FIQ_Stack_Size,    0x20
.equ    USR_Stack_Size,    0x100


/* Most of the functions in this file can be overridden by linking with
 *  alternate functions.
 */
.weak Reset_Handler
.weak UndefinedException_Handler
.weak SWI_Handler
.weak PrefetchAbort_Handler
.weak DataAbort_Handler
.weak IRQ_Handler
.weak FIQ_Handler
.weak __libc_init_array
.weak __libc_fini_array

.global _start
.global endless_loop

/*
 * External dependencies
 */
.extern main
.extern SystemInit
.extern exit
.extern _bss_start
.extern _bss_end
.extern _stack
.extern _data_start
.extern _data_end
.extern _data_src_start


/*
 * Microcontroller definitions that are required for configuration
 */

/* MEMMAP register (used to choose memory mapped to interrupt vectors) */
.equ    MEMMAP,            0xe01fc040

/* Settings for CPU modes */
.equ    Mode_USR,          0x10
.equ    Mode_FIQ,          0x11
.equ    Mode_IRQ,          0x12
.equ    Mode_SVC,          0x13
.equ    Mode_ABT,          0x17
.equ    Mode_UND,          0x1b
.equ    Mode_SYS,          0x1f

/* CPSR enable flags for IRQs, FIQ */
.equ    CPSR_I,            0x80
.equ    CPSR_F,            0x40

/* .text is used instead of .section .text so it works with arm-aout too. */

.text
.code 32
.align  0


/** @name _start
  * @brief Initial startup function executed when the MCU comes out of reset.
  *
  * Maps interrupt vectors to flash, initializes stack pointers, clears
  *  BSS segment, copies DATA segment to RAM, calls SystemInit() to
  *  set up PLLs etc., calls global constructors, calls main().  If
  *  main() returns, will then call global destructors and then spin
  *  in an infinite loop.
  */
_start:
_mainCRTStartup:

    /* Be sure to change out of boot loader mode */
    //ldr    r0, =MEMMAP
    //mov    r1, #0x01
    //str    r1, [r0]

    /* Get the top of the stack and reserve space for stacks in
     *  all the different modes
     */

    ldr    r3, .stack_end
    mov    sp, r3

    /* Set up Undefined Exception stack */
    msr    cpsr_c, #Mode_UND|CPSR_I|CPSR_F
    mov    sp, r3
    sub    r3, r3, #UND_Stack_Size

    /* ... Abort Exception stack ... */
    msr    cpsr_c, #Mode_ABT|CPSR_I|CPSR_F
    mov    sp, r3
    sub    r3, r3, #ABT_Stack_Size

    /* ... Fast IRQ Stack ... */
    msr    cpsr_c, #Mode_FIQ | CPSR_I | CPSR_F
    mov    sp, r3
    sub    r3, r3, #FIQ_Stack_Size

    /* ... IRQ Stack ... */
    msr    cpsr_c, #Mode_IRQ | CPSR_I | CPSR_F
    mov    sp, r3
    sub    r3, r3, #IRQ_Stack_Size

    /* ... Supervisor Mode Stack ... */
    msr    cpsr_c, #Mode_SVC | CPSR_I | CPSR_F
    mov    sp, r3
    sub    r3, r3, #SVC_Stack_Size

    /* ... User Mode Stack */
    msr    cpsr_c, #Mode_USR
    mov    sp, r3

    sub    r3, r3, #USR_Stack_Size

    /* Set the stack limit register to point to the space under the
     *  User mode stack for detection of stack overflows.
     */
    mov    sl, r3

	MOV     R3, #0xe0000000
        MOV     R1, #0x61
        strb    r1, [r3, #4]

    /* Clear the BSS Segment */

    ldr    r1, .bss_start    /* Get start of memory to clear        */
    ldr    r3, .bss_end      /* And end of memory to clear          */
    subs   r3, r3, r1        /* Calculate the length for looping    */
    beq    .end_bss_clear_loop
    mov    r2, #0
.bss_clear_loop:
    strb    r2, [r1], #1
    subs    r3, r3, #1
    bgt    .bss_clear_loop
.end_bss_clear_loop:


    /* Copy data from storage to DATA segment */

    ldr     r1, .data_start      /* Get start of memory to copy to   */
    ldr     r2, .data_src_start  /* Get start of memory to copy from */
    ldr     r3, .data_end        /* Get end of memory to copy to     */
    subs    r3, r3, r1           /* Calculate length for looping     */
    beq     .end_data_copy_loop
.data_copy_loop:
    ldrb    r4, [r2], #1
    strb    r4, [r1], #1
    subs    r3, r3, #1
    bgt    .data_copy_loop
.end_data_copy_loop:


    /* Call SystemInit() to handle other CPU initialization
     *  (PLL setup, etc.)
     */
    ldr    r12,=SystemInit
    mov    lr, pc
    bx     r12

    /* Do global/static constructors */
    ldr    r12,=__libc_init_array
    mov    lr, pc
    bx     r12

    /* Set up for main() */
    mov    r0, #0        /* argc = 0                     */
    mov    r1, #0        /* argv = NULL                  */
    mov    fp, a2        /* Null frame pointer           */
    mov    r7, a2        /* Null frame pointer for Thumb */

    /* Call main() */
    ldr    r12,=main
    mov    lr, pc
    bx     r12

    /* Do global/static destructors */
    ldr    r12,=__libc_fini_array
    mov    lr, pc
    bx     r12

    /* Infinite Loop if main() returns. */
main_completed:
endless_loop:
    b      main_completed


/** @name  __libc_init_array
  * @brief Call all init_array entries (global / static constructors)
  */
__libc_init_array:
    push   {r4, r5, lr}
    ldr    r4, .init_array_start
    ldr    r5, .init_array_end
    subs   r5, r5, r4
    beq    .end_init_array_loop
.init_array_loop:
    ldr    r12,[r4], #4
    mov    lr, pc
    bx     r12
    subs   r5, r5, #4
    bgt    .init_array_loop
.end_init_array_loop:
    /* Return */
    pop    {r4, r5, pc}


/** @name  __libc_fini_array
  * @brief Call all fini_array entries (global / static destructors)
  */
__libc_fini_array:
    push  {r4, r5, lr}
    ldr    r4, .fini_array_start
    ldr    r5, .fini_array_end
    subs   r5, r5, r4
    beq    .end_fini_array_loop
.fini_array_loop:
    ldr    r12,[r4], #4
    mov    lr, pc
    bx     r12
    subs   r5, r5, #4
    bgt    .fini_array_loop
.end_fini_array_loop:
    /* Return */
    pop    {r4, r5, pc}


/*
 * By default, exceptions all just go to the same infinite loop...
 *  This is separate from endless_loop to make debugging a little
 *  easier.
 */
UndefinedException_Handler:
SWI_Handler:
PrefetchAbort_Handler:
DataAbort_Handler:
IRQ_Handler:
FIQ_Handler:
uncaught_exception_loop:
    b       uncaught_exception_loop

/* For Thumb, constants must be after the code since only
 *  positive offsets are supported for PC relative addresses...
 *  which is why these are all located after _start.
 */
.align 0
.bss_start:        .word   _bss_start
.bss_end:          .word   _bss_end
.data_start:       .word   _data_start
.data_src_start:   .word   _data_src_start
.data_end:         .word   _data_end
.init_array_start: .word   _init_array_start
.init_array_end:   .word   _init_array_end
.fini_array_start: .word   _fini_array_start
.fini_array_end:   .word   _fini_array_end
.stack_end:        .word   _stack


/*
 * System Vectors
 */

.section .startup, "ax"
.code 32
.align 0

    ldr    PC, Reset_Addr               /* Reset Vector              */
    ldr    PC, UndefinedException_Addr  /* Undef Exception Vector    */
    ldr    PC, SWI_Addr                 /* Software Interrupt Vector */
    ldr    PC, PrefetchAbort_Addr       /* Prefetch Abort Vector     */
    ldr    PC, DataAbort_Addr           /* Data Abort Vector         */
    nop                                 /* System Vectors Checksum   */
    ldr    PC, [PC, #-0xff0]            /* IRQ Vector                */
    ldr    PC, FIQ_Addr                 /* FIQ Vector                */

/*
 * Storage for Vector Jump Locations
 */

Reset_Addr:
    .word    Reset_Handler
UndefinedException_Addr:
    .word    UndefinedException_Handler
SWI_Addr:
    .word    SWI_Handler
PrefetchAbort_Addr:
    .word    PrefetchAbort_Handler
DataAbort_Addr:
    .word    DataAbort_Handler
IRQ_Addr:
    .word    IRQ_Handler
FIQ_Addr:
    .word    FIQ_Handler

. = 0x100
