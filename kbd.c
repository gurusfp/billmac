#include <stdint.h>
#include <p89v51rd2.h>

#include "kbd.h"
#include "main.h"

uint8_t KbdData;
uint8_t KbdDataAvail;

#ifdef UNIT_TEST
#error "This file should not be included in Unit tests"
#endif

void
KbdInit(void)
{
  KBD_RC = 0xFF;
  KbdData = 0xFF;
  KbdDataAvail = 0;
}

void
KbdScan(void)
{
  static uint8_t gTimer0 = 0;
  uint8_t scan_code = 0, shift = 0;

//  if (0 != KbdDataAvail)
//    return;

  /* Arrangement of keys
     R1C1 R1C2 R1C3 R1C4
     R2C1 R2C2 R2C3 R2C4
  */
  KBD_RC = 0xFF;

  do {
    KBD_R2=0;
    DELAY1(0x4);
    if (KBD_C4==0) { shift = 0x80; }
    if (KBD_C1==0) { while(KBD_C1==0) { DELAY1(0x4); } scan_code = 5; gTimer0 = 0; break; }
    if (KBD_C2==0) { while(KBD_C2==0) { DELAY1(0x4); } scan_code = 6; gTimer0 = 0; break; }
    if (KBD_C3==0) { while(KBD_C3==0) { DELAY1(0x4); } scan_code = 7; gTimer0 = 0; break; }
    KBD_R2=1;    KBD_R4=0;
    DELAY1(0x4);
    if (KBD_C1==0) { while(KBD_C1==0) { DELAY1(0x4); } scan_code = 12; gTimer0 = 0; break; }
    if (KBD_C2==0) { while(KBD_C2==0) { DELAY1(0x4); } scan_code = 1; gTimer0 = 0;  break; }
    if (KBD_C3==0) { while(KBD_C3==0) { DELAY1(0x4); } scan_code = 13; gTimer0 = 0; break; }
    if (KBD_C4==0) { while(KBD_C4==0) { DELAY1(0x4); } scan_code = 14; gTimer0 = 0; break; }
    KBD_R4=1;    KBD_R1=0;
    DELAY1(0x4);
    if (KBD_C1==0) { while(KBD_C1==0) { DELAY1(0x4); } scan_code = 2;  gTimer0 = 0; break; }
    if (KBD_C2==0) { while(KBD_C2==0) { DELAY1(0x4); } scan_code = 3;  gTimer0 = 0; break; }
    if (KBD_C3==0) { while(KBD_C3==0) { DELAY1(0x4); } scan_code = 4;  gTimer0 = 0; break; }
    if (KBD_C4==0) { while(KBD_C4==0) { DELAY1(0x4); } scan_code = 11; gTimer0 = 0; break; }
    KBD_R1=1;    KBD_R3=0;
    DELAY1(0x4);
    if (KBD_C1==0) { while(KBD_C1==0) { DELAY1(0x4); } scan_code = 8;  gTimer0 = 0; break; }
    if (KBD_C2==0) { while(KBD_C2==0) { DELAY1(0x4); } scan_code = 9;  gTimer0 = 0; break; }
    if (KBD_C3==0) { while(KBD_C3==0) { DELAY1(0x4); } scan_code = 10; gTimer0 = 0; break; }
    if (KBD_C4==0) { while(KBD_C4==0) { DELAY1(0x4); } scan_code = 14; gTimer0 = 0; break; }
  } while (0);
  KBD_RC = 0xFF;

  /* enough time elapsed after last key hit */
  if ((gTimer0 > 0xF) && (0xFF != KbdData)) {
    KbdDataAvail = 1;
  } else if (0 != gTimer0) {
    /* No key press */
  } else if ((scan_code-1) == (0xF & (uint8_t)KbdData)) {
    KbdData = (KbdData & 0xF0) + 0x10;
  } else if (0 != scan_code) { /* first time */
    scan_code --;
    KbdData = shift | scan_code;
  }
//  lcd_buf_p = &(lcd_buf[1][0]);
//  LCD_PUTCH('K');
//  LCD_PUTCH('B');
//  LCD_PUTCH('D');
//  LCD_PUTCH(':');
//  LCD_PUTCH('a'+KbdDataAvail);
//  } else if (/* implicit (0xFF != KbdData) && */ 
    /* same key pressed multiple times */
//  }

  /* Don't let timer counter go past a value */
  if (0 ==  (gTimer0 & 0x80))
    gTimer0++;
}

uint8_t
KbdIsShiftPressed(void)
{
  uint8_t shift = 0;

  if (0xFF != KBD_RC) {
    KBD_R2=0;
    DELAY1(0x4);
    if (KBD_C4==0) { shift = 0x80; }
  }

  KBD_RC = 0xFF;
  return shift;
}
