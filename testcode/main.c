/*************************************************************************************
 *
 * \Description:
 * Program przykładowy - odpowiednik "Hello World" dla systemów wbudowanych
 * Rekomendujemy wkopiowywanie do niniejszego projektu nowych funkcjonalności
 *
 *
 * UWAGA! Po zmianie rozszerzenia na cpp program automatycznie będzie używał
 * kompilatora g++
 *
 *
 * Program przykładowy wykorzystuje Timer #0 i Timer #1 do "mrugania" diodami
 * Dioda P0.8 jest zapalona i gaszona, a czas pomiędzy tymi zdarzeniami
 * odmierzany jest przez Timer #0.
 * Program aktywnie oczekuje na upłynięcie odmierzanego czasu (1s)
 *
 * Druga z diod P0.9 jest gaszona i zapalana w takt przerwań generowanych
 * przez timer #1, z okresem 500 ms i wypełnieniem 20%.
 * Procedura obsługi przerwań zdefiniowana jest w innym pliku (irq/irq_handler.c)
 * Sama procedura MUSI być oznaczona dla kompilatora jako procedura obsługi 
 * przerwania odpowiedniego typu. W przykładzie jest to przerwanie wektoryzowane.
 * Odpowiednia deklaracja znajduje się w pliku (irq/irq_handler.h)
 * 
 * Prócz "mrugania" diodami program wypisuje na konsoli powitalny tekst.
 * 
 * \Authors: Michał Morawski, 
 *           Daniel Arendt, 
 *           Przemysław Ignaciuk,
 *           Marcin Kwapisz
 *
 * \Change log:
 *           2016.12.01: Wersja oryginalna.
 *
 ******************************************************************************/

#include "general.h"
#include <lpc2xxx.h>

//#include <printf_P.h>
#include <stdio.h>

#include <ea_init.h>
#include <consol.h>
#include <config.h>
#include "irq/irq_handler.h"
#include "lm75/lm75.h"
#include "timer.h"
#include "VIC.h"





/************************************************************************
 * \Description: opóźnienie wyrażone w liczbie sekund
 * \Parameter:
 *    [in] seconds: liczba sekund opóźnienia
 * \Returns: Nothing
 * \Side effects:
 *    przeprogramowany Timer #0
 *************************************************************************/
static void sdelay (tU32 seconds)
{
  T0TCR = TIMER_RESET;                    //Zatrzymaj i zresetuj
  T0PR  = PERIPHERAL_CLOCK-1;             //jednostka w preskalerze
  T0MR0 = seconds;
  T0IR  = TIMER_ALL_INT;                  //Resetowanie flag przerwań
  T0MCR = MR0_S;                          //Licz do wartości w MR0 i zatrzymaj się
  T0TCR = TIMER_RUN;                      //Uruchom timer

  // sprawdź czy timer działa
  // nie ma wpisanego ogranicznika liczby pętli, ze względu na charakter procedury
  while (T0TCR & TIMER_RUN)
  {
  }
}

/************************************************************************
 * \Description: uruchomienie obsługi przerwań 
 * \Parameter:
 *    [in] period    : okres generatora przerwań
 *    [in] duty_cycle: wypełnienie w %
 * \Returns: Nothing
 * \Side effects:
 *    przeprogramowany timer #1
 *************************************************************************/
static void init_irq (tU32 period, tU8 duty_cycle)
{
  //Zainicjuj VIC dla przerwań od Timera #1
  VICIntSelect &= ~TIMER_1_IRQ;           //Przerwanie od Timera #1 przypisane do IRQ (nie do FIQ)
  VICVectAddr5  = (tU32)IRQ_Test;         //adres procedury przerwania
  VICVectCntl5  = VIC_ENABLE_SLOT | TIMER_1_IRQ_NO;            
  VICIntEnable  = TIMER_1_IRQ;            // Przypisanie i odblokowanie slotu w VIC od Timera #1
  
  T1TCR = TIMER_RESET;                    //Zatrzymaj i zresetuj
  T1PR  = 0;                              //Preskaler nieużywany
  T1MR0 = ((tU64)period)*((tU64)PERIPHERAL_CLOCK)/1000;
  T1MR1 = (tU64)T1MR0 * duty_cycle / 100; //Wypełnienie 
  T1IR  = TIMER_ALL_INT;                  //Resetowanie flag przerwań
  T1MCR = MR0_I | MR1_I | MR0_R;          //Generuj okresowe przerwania dla MR0 i dodatkowo dla MR1
  T1TCR = TIMER_RUN;                      //Uruchom timer 
}

int main(void)
{
	printf("Hello World!\n");
	return 0;
}

tS32 __main(void)
{
  tS32 temperature;
  tS32 targetTemperature = 28;
  tBool isTargetReached = TRUE;

  //uruchomienie 'simple printf'
  eaInit();

  //powitanie
  printf("\n\n\n\n");
  printf("\n*********************************************************");
  printf("\n*");
  printf("\n* Systemy Wbudowane");
  printf("\n* Wydzial FTIMS");
  printf("\n* Moj pierwszy program");
  printf("\n*");
  printf("\n*********************************************************");

  // Setup lm75 (not sure if necessary)
  printf("\rSetting up LM75");
  // lm75Init();

  printf("\rConfiguring");
  // Setup buzzer (P.07)
  PINSEL0 &= ~((1 << 14) | (1 << 15));
  IODIR0 |= (1 << 7); // Try 3?

  // uruchomienie GPIO na nodze P.08: out
  PINSEL0 &= ~((1 << 16) | (1 << 17));
  IODIR0 |= (1 << 8);
  // uruchomienie GPIO na nodze P.09: out
  PINSEL0 &= ~((1 << 18) | (1 << 19));
  IODIR0 |= (1 << 9);

  printf("\rInitialising IRQ");
  // Uruchomienie przerwań co 1/2 s.
  init_irq(1, 20);

  // Aktywne "mruganie" diodą
  tU8 i = 0;
  while (1) 
  {
    // Read temperature
    // lm75TemperatureRead(&temperature);
    // if (temperature >= targetTemperature)
    // {
    //   isTargetReached = TRUE;
    //   printf("\rTemperature exceeded %d", targetTemperature);
    // }
    // else
    // {
    //   isTargetReached = FALSE;
    //   printf("\rTemperature below %d", targetTemperature);
    // }
    // printf("\rLM75 temp = %d.%d", temperature / 2, (temperature&1) * 5);

    // Buzzer ON/OFF
    if (i) 
    {
      IODIR0=0xffffffff;
      IOSET0 = 0x00100000;
      printf("Buzzer ON....\n");
    }
    // else 
    // {
    //   IOCLR0 = 0x00100000;
    //   printf("Buzzer OFF\n");
    // }

    // Diode ON/OFF
    // if (i) 
    // {
    //   IOCLR0 = (1 << 7);
    //   printf("Diode ON\n");
    // }
    // else 
    // {
    //   IOSET0 = (1 << 7);
    //   printf("Diode OFF\n");
    // }
    i = (i + 1) % 2;

    // Wait 1s
    sdelay(1);
  };
  return 0;
} 


