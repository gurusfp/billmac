#include <stdint.h>
#include <p89v51rd2.h>

#include "kbd.h"
#include "main.h"

uint8_t KbdData;
uint8_t KbdDataAvail;

void
KbdInit(void)
{
  KBD_RC = 0xFF;
  KbdData = 0xFF;
  KbdDataAvail = 0;
}

void
KbdScan(void) __naked
{
  static uint8_t gTimer0 = 0;
  uint8_t scan_code = 0, shift = 0;

  /* scan keyboard & push */
  /* Arrangement of keys
     R1C1 R1C2 R1C3 R1C4
     R2C1 R2C2 R2C3 R2C4
 */
  if (0xFF != KBD_RC) {
    gTimer0 = 0;
    do {
      KBD_R2=0;
      DELAY1(0x4);
      if (KBD_C4==0) { shift = 0x80; }
      if (KBD_C1==0) { while(KBD_C1==0) { DELAY1(0x4); } scan_code = 4;  break; }
      if (KBD_C2==0) { while(KBD_C2==0) { DELAY1(0x4); } scan_code = 5;  break; }
      if (KBD_C3==0) { while(KBD_C3==0) { DELAY1(0x4); } scan_code = 6;  break; }
      KBD_R1=1;    KBD_R4=0;
      DELAY1(0x4);
      if (KBD_C1==0) { while(KBD_C1==0) { DELAY1(0x4); } scan_code = 11; break; }
      if (KBD_C2==0) { while(KBD_C2==0) { DELAY1(0x4); } scan_code = 0;  break; }
      if (KBD_C3==0) { while(KBD_C3==0) { DELAY1(0x4); } scan_code = 12; break; }
      if (KBD_C4==0) { while(KBD_C4==0) { DELAY1(0x4); } scan_code = 13; break; }
      KBD_R4=1;    KBD_R1=0;
      DELAY1(0x4);
      if (KBD_C1==0) { while(KBD_C1==0) { DELAY1(0x4); } scan_code = 1;  break; }
      if (KBD_C2==0) { while(KBD_C2==0) { DELAY1(0x4); } scan_code = 2;  break; }
      if (KBD_C3==0) { while(KBD_C3==0) { DELAY1(0x4); } scan_code = 3;  break; }
      if (KBD_C4==0) { while(KBD_C4==0) { DELAY1(0x4); } scan_code = 10; break; }
      KBD_R2=1;    KBD_R3=0;
      DELAY1(0x4);
      if (KBD_C1==0) { while(KBD_C1==0) { DELAY1(0x4); } scan_code = 7;  break; }
      if (KBD_C2==0) { while(KBD_C2==0) { DELAY1(0x4); } scan_code = 8;  break; }
      if (KBD_C3==0) { while(KBD_C3==0) { DELAY1(0x4); } scan_code = 9;  break; }
      if (KBD_C4==0) { while(KBD_C4==0) { DELAY1(0x4); } scan_code = 13; break; }
    } while (0);

    KBD_RC = 0xFF;
    __asm
      RET
    __endasm;
  }

  /* Don't let timer counter go past a value */
  if (0 ==  (gTimer0 & 0x80))
    gTimer0++;

  /* enough time elapsed after last key hit */
  if ((gTimer0 > 0xF) && (0xFF != KbdData))
    KbdDataAvail = 1;
  else if (0xFF == KbdData) { /* first time */
    KbdData = shift | scan_code;
  } else if (/* implicit (0xFF != KbdData) && */ (scan_code == (0xF & (uint8_t)KbdData) ) {
    /* same key pressed multiple times */
    KbdData = (KbdData & 0xF0) + 0x10;
  }

  __asm
    RET
  __endasm;
}
