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
  ET0 = 1;      /* Enable Timer0 intrs */
  ET1 = 1;      /* Enable Timer0 intrs */

  /* interrupt prioiryt : EX0 is first */
  IP0 = 1;

  /* EX0 interrupt for PS2 Keyboard */
  EX0 = 1;      /* enable INT0 */
  IT0 = 1;      /* negative enable */

  /* all interrupts enable */
  EA  = 1;
}

/* Timer0 should be used for normal scheduling applications */
uint8_t timer0_count = 0;
void
Timer0_isr(void) __interrupt(TF0_VECTOR)
{
  uint8_t ui2;

  /* all interrupts disable */
  EA  = 0;  ET0 = 0;  TR0 = 0;

  /**************************************************/
  /*****************   KBD   ************************/
  /**************************************************/
  KbdScan();
  /**************************************************/
  /**************************************************/
  /**************************************************/

  /**************************************************/
  /*****************   LCD   ************************/
  /**************************************************/
  /* Enter service routine only when change in data */
  if (0 == (lcd_buf_prop & LCD_PROP_DIRTY)) {
    TF0 = 0;  ET0 = 1;  TR0 = 1;  EA  = 1;
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
  /**************************************************/
  /**************************************************/
  /**************************************************/

  /* enable all interrupts */
  TF0 = 0;     /* Clear pending interrupt */
  ET0 = 1;  TR0 = 1;  EA  = 1;
}

void
Timer1_isr(void) __interrupt(TF1_VECTOR)
{
  EA  = 0;      /* all interrupts disable */
  ET1 = 0;     /* Disable Timer1 intrs */
  TR1 = 0;      /* T0Run, IE */

  /* Enable next entry */
  TF1 = 0;     /* Clear pending interrupt */
//  ET1 = 1;     /* Enable Timer1 intrs */
//  TR1 = 1;      /* T0Run, IE */
  EA  = 1;      /* all interrupts enable */
}

__sbit ps2ShiftHit = 0, ps2CtrlHit = 0, ps2AltHit = 0;

#define LENOF_DR 3
uint8_t DR[LENOF_DR];
void
ex0_isr(void) __interrupt(IE0_VECTOR)    /* INT0 P3_2 (Clock) */
{             /* Data come with Clock from Device to MCU together */
  static uint8_t KeyData, transL, bitC = 0, drC;

  /* Keep interrupting on -ve, +ve until all transmission */
  if (1 == IT0) {
    IT0 = 0;      /* positive enable */
    return;
  }
  IT0 = 1;

  /* ------------------------------------- */
  bitC++;
  if (1 == bitC) {
    KeyData = 0;
    drC = 0;
    transL = 2;
  } else if (10 == bitC) {
    /* FIXME: Check parity and blink an LED */
  } else if (11 == bitC) {
    bitC = 0;
  } else {
    KeyData >>= 1;
    KeyData |= (((uint8_t)KBD_PS2_DATA)<<7);
  }

  if (0 != bitC)
    return;
  else if (drC < LENOF_DR) {
    DR[drC] = KeyData;
  }
  drC++;
 
  /* --------------------------------------- */
  if (DR[0] == 0xE0) {
    transL = 2;    /* E0 XX */
    if (DR[1] == 0x12) {    /* E0 12 E0 7C */
      transL = 4;
    }
    if (DR[1] == 0xF0) {
      transL = 3;    /* E0 F0 XX */
      if (DR[2] == 0x7C) {    /* E0 F0 7C E0 F0 12 */
	transL = 6;
      }
    }
  } else if (DR[0] == 0xF0) {
    transL = 2;    /* F0 XX */
    if (2 == drC) { /* Break of normal keys */
      if ((0x12 == DR[0]) || (0x59 == DR[0]))
	ps2ShiftHit = 0;
      else if (0x14 == DR[0])
	ps2CtrlHit = 0;
      else if (0x11 == DR[0])
	ps2AltHit = 0;
      else {
	KeyData = ps2code2ascii[DR[0]];
	if (ASCII_NUMLK == KeyData) {
	  /* FIXME: Switch TOGGLE the light */
	} else if (ASCII_UNDEF != KeyData) {
	  KbdData = KeyData;
	  KbdDataAvail = 1;
	}
      }
    }
  } else if (0xE1 == DR[0]) {
    transL = 8;
  } else {
    /* Make code received, generally no action except for sticky keys */
    transL = 1;
    if ((0x12 == DR[0]) || (0x59 == DR[0]))
      ps2ShiftHit = 1;
    else if (0x14 == DR[0])
      ps2CtrlHit = 1;
    else if (0x11 == DR[0])
      ps2AltHit = 1;
    /* else if (0xFA == DR[0]) ACKNOWLEDGEMENT FROM KBD */
  }

  /* --------------------------------------- */
  if (drC == transL) {
    if (3 == drC) {
      KbdData = ps2code2ascii[KeyData];
      KbdDataAvail = 1;
    }
    drC = 0;
  }
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
