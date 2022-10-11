ENTRY(_startup)
EXTERN(_startup)

/* Memory Definitions */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x00020000
  RAM   (rw) : ORIGIN = 0x40000000, LENGTH = 0x00008000
}

/*
 * Reserve memory for heap and stack. The linker will issue an error if there
 * is not enough memory.
 *
 * NOTE: The reserved heap and stack will be added to the bss column of the
 * binutils size command.
 */
_heap_size = 0x1000;    /* required amount of heap  */
_stack_size = 0x488;    /* required amount of stack */

/*
 * The stack starts at the end of RAM and grows downwards. Full-descending
 * stack; decrement first, then store.
 */
_estack = ORIGIN(RAM) + LENGTH(RAM);


/* Section Definitions */

SECTIONS
{

  /* first section is .text which is used for code */

  .text :
  {
    *startup.o (.text)         /* Startup code */
    *(.text)                   /* remaining code */
    *(.glue_7)
    *(.glue_7t)

  } > FLASH = 0

  . = ALIGN(4);

  /* .rodata section which is used for read-only data (constants) */

  .rodata :
  {
    *(.rodata) 
    *(.rodata*)
  } > FLASH

  . = ALIGN(4);

  _etext = . ;
  PROVIDE (etext = .);

  /* .data section which is used for initialized data */

  .data 0x40000080: AT (_etext)
  {
    _data = . ;
    *(.data)
    SORT(CONSTRUCTORS)
  } > RAM
  . = ALIGN(4);

  _edata = . ;
   PROVIDE (edata = .);

  /* .bss section which is used for uninitialized data */

  .bss (NOLOAD):
  {
    _bss          = . ;
    __bss_start   = . ;
    __bss_start__ = . ;
    *(.bss)
    *(COMMON)
    . = ALIGN(4);
  } > RAM
  . = ALIGN(4);
  __bss_end   = . ;
  __bss_end__ = . ;

  _end = . ;
  PROVIDE (end = .);

  /*
   * Reserve memory for heap and stack. The linker will issue an error if
   * there is not enough memory.
   */
   ._heap :
   {
        . = ALIGN(4);
        _HEAP_START = .;
        . = . + _heap_size;
        . = ALIGN(4);
        _HEAP_END = .;
   } >RAM

   ._stack :
   {
        . = ALIGN(4);
        . = . + _stack_size;
        . = ALIGN(4);
   } >RAM

}
