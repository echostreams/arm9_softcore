#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

extern long timeval;

clock_t clock(void) {
  return timeval;
}

void _clock_init(void) {
  timeval = 0;
}

#define CR     0x0D

/* registers for user-defined behavior rule */
#define SERIAL_FLAG *(volatile unsigned char *) 0xe0000000
#define SERIAL_OUT *(volatile unsigned char *) 0xe0000004
#define SERIAL_IN *(volatile unsigned char *) 0xe0000008

/* implementation of putchar (also used by printf function to output data)    */
int sendchar (int ch)  {                 /* Write character to Serial Port    */

  if (ch == '\n')  {
    while (SERIAL_FLAG & 0x01);
    SERIAL_OUT = CR;                          /* output CR */
  }
  while (SERIAL_FLAG & 0x01);
  return (SERIAL_OUT = ch);
}


int getkey (void)  {                     /* Read character from Serial Port   */

  while (!(SERIAL_FLAG & 0x02));

  return (SERIAL_IN);
}

// LIBC SYSCALLS
/////////////////////

extern int _end;

caddr_t _sbrk(int incr) {
  static unsigned char *heap = NULL;
  unsigned char *prev_heap;

  if (heap == NULL) {
    heap = (unsigned char *)&_end;
  }
  prev_heap = heap;

  heap += incr;
  //printf("heap: %x\n", heap);

  return (caddr_t) prev_heap;
}

int _close(int file) {
  return -1;
}

int _fstat(int file, struct stat *st) {
  st->st_mode = S_IFCHR;

  return 0;
}

int _isatty(int file) {
  return 1;
}

int _lseek(int file, int ptr, int dir) {
  return 0;
}

//void _exit(int status) {
//  __asm("BKPT #0");
//}

void _kill(int pid, int sig) {
  return;
}

int _getpid(void) {
  return -1;
}

int _write (int file, char * ptr, int len) {
  int written = 0;

  if ((file != 1) && (file != 2) && (file != 3)) {
    return -1;
  }

  for (; len != 0; --len) {
    //if (sendchar( (uint8_t)*ptr++ )) {
    //  return -1;
    //}
    sendchar( *ptr++ );
    ++written;
  }
  return written;
}

int _read (int file, char * ptr, int len) {
  int read = 0;

  if (file != 0) {
    return -1;
  }

  for (; len > 0; --len) {
    //usart_serial_getchar(&stdio_uart_module, (uint8_t *)ptr++);
    *ptr = getkey();
    ptr++;
    read++;
  }
  return read;
}

