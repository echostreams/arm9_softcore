/******************************************************************************/
/*    Output Options, Entry Symbol, Libraries to Aggressively Use             */
/******************************************************************************/

OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
STARTUP(LPC2xxx_crt0.o)
ENTRY(_start)
GROUP(-lgcc -lc -lLPC2xxx)

/******************************************************************************/
/*                               Stack Sizes                                  */
/******************************************************************************/

__Minimum_Stack_Size__ = 0x100;
C_STACK_SIZE = 512;
IRQ_STACK_SIZE = 16;
FIQ_STACK_SIZE = 16;
SVC_STACK_SIZE = 16;
ABT_STACK_SIZE = 16;
UND_STAC_SIZE = 0;

/******************************************************************************/
/*                           Exception Handlers                               */
/******************************************************************************/

/* By default all un-set IRQ handlers go to reset */

PROVIDE ( Reset_Handler = _start ) ;

PROVIDE ( UndefinedException_Handler = Reset_Handler ) ;

PROVIDE ( SWI_Handler = UndefinedException_Handler ) ;
PROVIDE ( PrefetchAbort_Handler = UndefinedException_Handler ) ;
PROVIDE ( DataAbort_Handler = UndefinedException_Handler ) ;
PROVIDE ( IRQ_Handler = UndefinedException_Handler ) ;
PROVIDE ( FIQ_Handler = UndefinedException_Handler ) ;

/******************************************************************************/
/*                              Peripheral Map                                */
/******************************************************************************/

/* Peripheral base address */
PERIPH_BASE           = 0xE0000000;

/* Peripheral memory map */
APBPERIPH_BASE       = PERIPH_BASE;

PROVIDE ( WDT        = (APBPERIPH_BASE + 0x000000) ) ;
PROVIDE ( CT32B0     = (APBPERIPH_BASE + 0x004000) ) ;
PROVIDE ( CT32B1     = (APBPERIPH_BASE + 0x008000) ) ;
PROVIDE ( UART0      = (APBPERIPH_BASE + 0x00C000) ) ;
PROVIDE ( UART1      = (APBPERIPH_BASE + 0x010000) ) ;
PROVIDE ( I2C0       = (APBPERIPH_BASE + 0x01C000) ) ;
PROVIDE ( SPI0       = (APBPERIPH_BASE + 0x020000) ) ;
PROVIDE ( RTC        = (APBPERIPH_BASE + 0x024000) ) ;
PROVIDE ( GPIO0      = (APBPERIPH_BASE + 0x028000) ) ;
PROVIDE ( PINSEL     = (APBPERIPH_BASE + 0x02C000) ) ;
PROVIDE ( I2C1       = (APBPERIPH_BASE + 0x05C000) ) ;
PROVIDE ( SSP0       = (APBPERIPH_BASE + 0x068000) ) ;
PROVIDE ( CT16B0     = (APBPERIPH_BASE + 0x070000) ) ;
PROVIDE ( CT16B1     = (APBPERIPH_BASE + 0x074000) ) ;
PROVIDE ( SYSCON     = (APBPERIPH_BASE + 0x1FC000) ) ;

/* System Control Space memory map */
SCS_BASE              = 0xFFFFF000;

PROVIDE ( VIC         = (SCS_BASE) ) ;

/******************************************************************************/
/*                                 Memory Map                                 */
/******************************************************************************/

MEMORY {
   flash : ORIGIN = 0x00000000, LENGTH = 128K
   ram   : ORIGIN = 0x40000000, LENGTH = 16K
}

/******************************************************************************/
/*                              Section Layout                                */
/******************************************************************************/

/* Leave RAM space for IAP Commands */
_stack      = ORIGIN(ram) + LENGTH(ram) - 36;
_stack_size = __Minimum_Stack_Size__;

SECTIONS {
    .reset : {
        . = ALIGN(4);
    } > flash
    
    .text : {
        . = ALIGN(4);
        KEEP(*(.startup .startup.*))

        CREATE_OBJECT_SYMBOLS

        /* Code Read Protection */
        . = 0x1fc;
        KEEP(*(.crp))

        . = 0x200;
        *(.text .text.* .gnu.linkonce.t.*)
        *(.plt)
        *(.gnu_warning)
        *(.glue_7t) *(.glue_7)
        
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*crtend.o(.ctors))
         
        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*crtend.o(.dtors))
         
        . = ALIGN(4);
        *(.rodata .rodata.* .gnu.linkonce.r.*)
         
        . = ALIGN(4);
        KEEP(*(.init0))
        KEEP(*(.init1))
        KEEP(*(.init2))
        KEEP(*(.init3))
        KEEP(*(.init4))
        KEEP(*(.init5))
        KEEP(*(.init6))
        KEEP(*(.init7))
        KEEP(*(.init8))
        KEEP(*(.init9))
        
        . = ALIGN(4);
        _init_array_start = ABSOLUTE(.);
        KEEP(*(.init_array))
        _init_array_end = ABSOLUTE(.);
                 
        . = ALIGN(4);
        _fini_array_start = ABSOLUTE(.);
        KEEP(*(.fini_array))
        _fini_array_end = ABSOLUTE(.);
        
        . = ALIGN(4);
        KEEP(*(.fini9))
        KEEP(*(.fini8))
        KEEP(*(.fini7))
        KEEP(*(.fini6))
        KEEP(*(.fini5))
        KEEP(*(.fini4))
        KEEP(*(.fini3))
        KEEP(*(.fini2))
        KEEP(*(.fini1))
        KEEP(*(.fini0))
        
        . = ALIGN(4);
    } >flash
         
    .ARM.exidx : { 
        __exidx_start = ABSOLUTE(.);
        *(.ARM.exidx* .gnu.linkonce.armexidx.*) 
        __exidx_end = ABSOLUTE(.);
       } >flash

    /* C++ exception unwinding stuff--needed by Code Sourcery toolchain */

    .ARM.extab : { 
        *(.ARM.extab* .gnu.linkonce.armextab.*) 
       } >flash
    
    __end_of_text__ = .;

    .data : {
        . = ALIGN(4);
        _data_src_start = LOADADDR(.data);
        _data_start = ABSOLUTE(.);
        KEEP(*(.jcr))
        *(.got.plt) *(.got)
        *(.fastcode)
        *(.shdata)
        *(.data .data.* .gnu.linkonce.d.*)
        . = ALIGN(4);
        _data_end = ABSOLUTE(.);
    } >ram AT>flash

    _data_end = .;

    .dataflash : {
        . = ALIGN(4096);
        KEEP(*(.dataflash))
        . = ALIGN(4096);
    } > flash

    .bss : {
        . = ALIGN(4);
        _bss_start = ABSOLUTE(.);
        *(.shbss)
        *(.bss .bss.* .gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(4);
        . += 4;
    } > ram

    _bss_end = .;


    /* Align here to ensure that the .bss section occupies space up to
     * _end.  Align after .bss to ensure correct alignment even if the
     * .bss section disappears because there are no input sections.  
     */
    . = ALIGN(4);
    _end = .;
    _bss_end = . ;
    PROVIDE (end = .);
    _heap_start = .;
    _heap_size  = _stack - __Minimum_Stack_Size__ - _heap_start;
        
    ._checkstack : {
        /* User stack section; meant to guarantee enough space left for it */
        . = ALIGN(4);
        __checkstack_start__ = .;
        . = . + __Minimum_Stack_Size__ ;
        . = ALIGN(4);
        __checkstack_end__ = .;
    } > ram

    /* Rest is debugging info */
    
    /* remove the debugging information from the standard libraries * /
    /DISCARD/ :
    {
    
     libc.a ( * )
     libm.a ( * )
     libgcc.a ( * )
     
    }

    /* Stabs debugging sections.  */
    .stab          0 : { *(.stab) }
    .stabstr       0 : { *(.stabstr) }
    .stab.excl     0 : { *(.stab.excl) }
    .stab.exclstr  0 : { *(.stab.exclstr) }
    .stab.index    0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment       0 : { *(.comment) }
    /* DWARF debug sections.
       Symbols in the DWARF debugging sections are relative to the beginning
       of the section so we begin them at 0.  */
    /* DWARF 1 */
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    /* GNU DWARF 1 extensions */
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    /* DWARF 1.1 and DWARF 2 */
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    /* DWARF 2 */
    .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }
    /* SGI/MIPS DWARF 2 extensions */
    .debug_weaknames 0 : { *(.debug_weaknames) }
    .debug_funcnames 0 : { *(.debug_funcnames) }
    .debug_typenames 0 : { *(.debug_typenames) }
    .debug_varnames  0 : { *(.debug_varnames) }
}
