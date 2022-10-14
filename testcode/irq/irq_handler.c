/*************************************************************************************
 *
 * @Description:
 * Przykładowa procedura obsługi przerwania dostosowania do 
 * obsługi przerwań wektoryzowanych.
 * Procedura ta przełącza stan diody P0.9.
 * 
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

#include "../general.h"
#include <lpc2xxx.h>
#include "irq_handler.h"
#include "../timer.h"

void IRQ_Test(void)
{
  if (TIMER_MR0_INT & T1IR)
  {
      IOCLR0 = (1<<9);
      T1IR   = TIMER_MR0_INT;       // Informacja dla Timera #1, 
	                                // że jego przerwanie zostało obsłużone.
  }
  else if (TIMER_MR1_INT & T1IR)
  {
      IOSET0 = (1<<9);
      T1IR   = TIMER_MR1_INT;       // Informacja dla Timera #1, 
	                                // że jego przerwanie zostało obsłużone.
  }
  VICVectAddr = 0x00;                // End of Interrupt -> informacja dla 
                                     // kontrolera przerwań
}

