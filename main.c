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
  TMOD |= 0x11;  /* T0:Mode_1 */
  TR0 = 1;      /* T0Run, IE */
  TR1 = 1;      /* T1Run, IE */
  EA  = 1;      /* all interrupts enable */
  ET0 = 1;      /* Enable Timer0 intrs */
  ET1 = 1;      /* Enable Timer0 intrs */
  IP  = 0;
}

uint8_t timer0_count = 0;
void
Timer0_isr(void) __interrupt(TF0_VECTOR)
{
  EA  = 0;      /* all interrupts disable */
  ET0 = 0;     /* Disable Timer0 intrs */
  TR0 = 0;      /* T0Run, IE */

  KbdScan();

  TF0 = 0;     /* Clear pending interrupt */
  ET0 = 1;     /* Enable Timer0 intrs */
  TR0 = 1;      /* T0Run, IE */
  EA  = 1;      /* all interrupts enable */
}

void
Timer1_isr(void) __interrupt(TF1_VECTOR)
{
  uint8_t ui2;

  EA  = 0;      /* all interrupts disable */
  ET1 = 0;     /* Disable Timer1 intrs */
  TR1 = 0;      /* T0Run, IE */

  /* Enter service routine only when change in data */
  if (0 == (lcd_buf_prop & LCD_PROP_DIRTY)) {
    TF1 = 0;
    ET1 = 1;
    TR1 = 1;      /* T0Run, IE */
    EA  = 1;      /* all interrupts enable */
    return;
  }
  lcd_buf_prop &= ~LCD_PROP_DIRTY;

  /* Display on LCD */
  LCD_cmd(LCD_CMD_CLRSCR);
  LCD_cmd(LCD_CMD_CUR_10);
  lcd_buf_p = (uint8_t*) lcd_buf;
  for (ui2=0; ui2<LCD_MAX_COL; ui2++) {
    LCD_wrchar(lcd_buf_p[0]);
    lcd_buf_p++;
  }
  LCD_cmd(LCD_CMD_CUR_20);
  for (ui2=0; ui2<LCD_MAX_COL; ui2++) {
    LCD_wrchar(lcd_buf_p[0]);
    lcd_buf_p++;
  }

  /* Enable next entry */
  TF1 = 0;     /* Clear pending interrupt */
  ET1 = 1;     /* Enable Timer1 intrs */
  TR1 = 1;      /* T0Run, IE */
  EA  = 1;      /* all interrupts enable */
}

#ifndef MAIN_NOMAIN

void
main(void)
{
main_start:

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
      LCD_WR_LINE_N(1, 0, "Got Hit on:", 11);
      LCD_wrchar('a' + KbdData);
      KBD_RESET_KEY;
    } else {
      LCD_cmd(LCD_CMD_CUR_20);
      LCD_sendstring("No Kbd Hit");
    }
  }

  /* Incase it comes out of this loop :) */
  goto main_start:
}

#endif
