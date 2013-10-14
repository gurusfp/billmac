#include<stdint.h>
#include<p89v51rd2.h>

#include "lcd.h"
#include "kbd.h"
#include "main.h"

/* Clock is run at 11MHz. Counter runs at 1/6th the
freq. Need to provide callback for keyboard scanning. How many
callbacks/ second? To decide this, we need to know how many 
clocks we get in a cycle
11MHz/6 ~= 2MHz i.e. 20,00,000 cycles/second
So, 2 scans/second means that we need interrupt for every
10,00,000 cycles. When clock is at 6MHz, we get 1 scans/second, that
should be OK.
Timer0 in mode-1 has 16-bit auto-reload feature, that is 65K cycles.
So, we should be interrupted every 10L/65K = 15.25 ~ 16
 */

void
Timer_Init(void)
{
  TMOD |= 0x1;  /* T0:Mode_1 */
  TR0 = 1;      /* T0Run, IE */
  EA  = 1;      /* all interrupts enable */
  ET0 = 1;      /* Enable Timer0 intrs */
  IP  = 0;
  LCD_DBG_PRINT("Int Setup");
}

void
Timer0_isr(void) __interrupt(TF0_VECTOR) __using(1)
{
  ET0 = 0;     /* Disable Timer0 intrs */
  /* If Kbd is hit continously */
  if ( (0xFF != KBD_RC) || ((0 == KbdDataAvail) && (0xFF != KbdData)) ) {
    KbdScan();
  }
  TF0 = 0;     /* Clear pending interrupt */
  ET0 = 1;     /* Enable Timer0 intrs */
}

void
main(void)
{
  /* init all */
  KbdInit();
  LCD_init();
  Timer_Init();
  

  /* */

  while(1) {
    DELAY(255);

    LCD_init();
    LCD_cmd(LCD_CMD_CUR_10);
    LCD_sendstring("Hello World 4");

    if (0xFF != KbdData) {
      LCD_cmd(LCD_CMD_CUR_20);
      LCD_sendstring("Got Hit on ");
      LCD_wrchar('a' + KbdData);
      KbdData = 0;
    } else {
      LCD_cmd(LCD_CMD_CUR_20);
      LCD_sendstring("No Kbd Hit");
    }
  }

  /* Incase it comes out of this loop :) */
  main();
}
