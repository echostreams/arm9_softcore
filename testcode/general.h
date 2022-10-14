/*************************************************************************************
 *
 * @Description:
 * Przykład definiowania uniwersalnych typów.
 * 
 * @Authors: Michał Morawski, 
 *           Daniel Arendt, 
 *           Przemysław Ignaciuk,
 *           Marcin Kwapisz
 *
 * @Change log:
 *           2016.12.01: Wersja oryginalna.
 *
 ******************************************************************************/
#ifndef __GENERAL_H__
#define __GENERAL_H__

typedef unsigned char             tU8;
typedef unsigned short            tU16;
typedef unsigned int              tU32;
typedef unsigned long long        tU64;
typedef signed char               tS8;
typedef signed short              tS16;
typedef signed int                tS32;
typedef signed long long          tS64;
typedef enum{FALSE = 0, 
             TRUE  = 1}           tBool;
#define _BIT(n)                   (1<<(n))
#define _PIN(port,bit)            ((IOPIN##port & _BIT(bit)) != 0)

#endif  //__GENERAL_H__

