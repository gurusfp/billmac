#include <stdint.h>
#include <p89v51rd2.h>

#include "kbd.h"
#include "main.h"

uint8_t KbdData;
__sbit  KbdDataAvail;

#ifdef UNIT_TEST
#error "This file should not be included in Unit tests"
#endif

#define KBD_RISE_DELAY(N) {			\
  uint8_t ui1;					\
  for(ui1=0; ui1<N; ui1++)			\
      {} \
  }

uint8_t keyChars[] = {
  /* KCHAR_ROWS x KCHAR_COLS */
  '0', ' ', '.', ',', ')', '+', '?', '_', ':',
  '1', 'a', 'b', 'c', '!', 'A', 'B', 'C', '~',
  '2', 'd', 'e', 'f', '@', 'D', 'E', 'F', '{',
  '3', 'g', 'h', 'i', '#', 'G', 'H', 'I', '}',
  '4', 'j', 'k', 'l', '$', 'J', 'K', 'L', '[',
  '5', 'm', 'n', 'o', '%', 'M', 'N', 'O', ']',
  '6', 'p', 'q', 'r', '^', 'P', 'Q', 'R', '|',
  '7', 's', 't', 'u', '&', 'S', 'T', 'U', '/',
  '8', 'v', 'w', 'x', '*', 'V', 'W', 'X', '<',
  '9', 'y', 'z', '(', '-', 'Y', 'Z', '=', '>',
};

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

  if (0 != KbdDataAvail)
    return;

  /* Arrangement of keys
     R1C1 R1C2 R1C3 R1C4
     R2C1 R2C2 R2C3 R2C4
  */
  KBD_RC = 0xFF;

  do {
    KBD_R2=0;
    KBD_RISE_DELAY(0x4);
    if (KBD_C4==0) { shift = 0x80; }
    if (KBD_C1==0) { while(KBD_C1==0) { KBD_RISE_DELAY(0x4); } scan_code = 5; gTimer0 = 0; break; }
    if (KBD_C2==0) { while(KBD_C2==0) { KBD_RISE_DELAY(0x4); } scan_code = 6; gTimer0 = 0; break; }
    if (KBD_C3==0) { while(KBD_C3==0) { KBD_RISE_DELAY(0x4); } scan_code = 7; gTimer0 = 0; break; }
    KBD_R2=1;    KBD_R4=0;
    KBD_RISE_DELAY(0x4);
    if (KBD_C1==0) { while(KBD_C1==0) { KBD_RISE_DELAY(0x4); } scan_code = 12; gTimer0 = 0; break; }
     if (KBD_C2==0) { while(KBD_C2==0) { KBD_RISE_DELAY(0x4); } scan_code = 1;  gTimer0 = 0;  break; }
    if (KBD_C3==0) { while(KBD_C3==0) { KBD_RISE_DELAY(0x4); } scan_code = 13; gTimer0 = 0; break; }
    if (KBD_C4==0) { while(KBD_C4==0) { KBD_RISE_DELAY(0x4); } scan_code = 14; gTimer0 = 0; break; }
    KBD_R4=1;    KBD_R1=0;
    KBD_RISE_DELAY(0x4);
    if (KBD_C1==0) { while(KBD_C1==0) { KBD_RISE_DELAY(0x4); } scan_code = 2;  gTimer0 = 0; break; }
    if (KBD_C2==0) { while(KBD_C2==0) { KBD_RISE_DELAY(0x4); } scan_code = 3;  gTimer0 = 0; break; }
    if (KBD_C3==0) { while(KBD_C3==0) { KBD_RISE_DELAY(0x4); } scan_code = 4;  gTimer0 = 0; break; }
    if (KBD_C4==0) { while(KBD_C4==0) { KBD_RISE_DELAY(0x4); } scan_code = 11; gTimer0 = 0; break; }
    KBD_R1=1;    KBD_R3=0;
    KBD_RISE_DELAY(0x4);
    if (KBD_C1==0) { while(KBD_C1==0) { KBD_RISE_DELAY(0x4); } scan_code = 8;  gTimer0 = 0; break; }
    if (KBD_C2==0) { while(KBD_C2==0) { KBD_RISE_DELAY(0x4); } scan_code = 9;  gTimer0 = 0; break; }
    if (KBD_C3==0) { while(KBD_C3==0) { KBD_RISE_DELAY(0x4); } scan_code = 10; gTimer0 = 0; break; }
    if (KBD_C4==0) { while(KBD_C4==0) { KBD_RISE_DELAY(0x4); } scan_code = 14; gTimer0 = 0; break; }
  } while (0);
  KBD_RC = 0xFF;

  /* enough time elapsed after last key hit */
  if ((gTimer0 > 0x8) && (0xFF != KbdData)) {
    uint8_t key_sc = KbdData & 0xF;
    key_sc *= KCHAR_COLS;
    if (KbdData & 0x80) key_sc += KCHAR_SHIFT_SZ;
    key_sc += (KbdData>>4) & 0x7;
    KbdData = keyChars[key_sc];
    KbdDataAvail = 1;
  } else if (0 != gTimer0) {
    /* No key press */
  } else if ((scan_code-1) == (0xF & (uint8_t)KbdData)) {
    KbdData = (KbdData & 0xF0) + 0x10;
  } else if (0 != scan_code) { /* first time */
    scan_code --;
    KbdData = shift | scan_code;
  }

  /* Don't let timer counter go past a value */
  if (0 ==  (gTimer0 & 0x80))
    gTimer0++;
}

void
KbdGetCh(void)
{
  uint8_t ui2, ui3, ui4;

  while (1) {
    if (KbdDataAvail) {
      return;
    }
    /* enter sleep state */
    PCON = IDL;
    __asm
      NOP
      NOP
      NOP
      NOP
      NOP
      __endasm;
    /* Code will reach here only after wakeup */
  }
}

uint8_t
KbdIsShiftPressed(void)
{
  uint8_t shift = ps2ShiftHit ? 0x80 : 0;

  KBD_RC = 0xFF;
  KBD_R2=0;
  KBD_RISE_DELAY(0x4);
  if (KBD_C4==0) { shift = 0x80; }
  KBD_RC = 0xFF;

  return shift;
}

uint8_t
ps2code2ascii[] = {
  ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, '`', ASCII_UNDEF, /* 0-15 */
  ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, '1', ASCII_UNDEF, 'q', ASCII_UNDEF, 'z', 's', 'a', 'w', '2', ASCII_UNDEF, /* 16-31 */
  ASCII_UNDEF, 'c', 'x', 'd', 'e', '4', '3', ASCII_UNDEF, ' ', ASCII_UNDEF, 'v', 'f', 't', 'r', '5', ASCII_UNDEF, /* 32-47 */
  ASCII_UNDEF, 'n', 'b', 'h', 'g', 'y', '6', ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, 'm', 'j', 'u', '7', '8', ASCII_UNDEF, /* 48-63 */
  ASCII_UNDEF, ',', 'k', 'i', 'o', '0', '9', ASCII_UNDEF, ASCII_UNDEF, '.', '/', 'l', ';', 'p', '-', ASCII_UNDEF, /* 64-79 */
  ASCII_UNDEF, ASCII_UNDEF, '\'', ASCII_UNDEF, '[', '=', ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_ENTER, ']', ASCII_UNDEF, '\\', ASCII_UNDEF, ASCII_UNDEF, /* 80-95 */
  ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_BACKSPACE, ASCII_UNDEF, ASCII_UNDEF, '1', ASCII_UNDEF, '4', '7', ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, /* 96-111 */
  '0', '.', '2', '5', '6', '8', ASCII_UNDEF, ASCII_NUMLK, ASCII_UNDEF, '+', '3', '-', '*', '9', ASCII_PRNSCRN, ASCII_UNDEF, /* 112-127 */
};

uint8_t
ps2code2asciiE0[] = {
  ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, /* 0-15 */
  ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, /* 16-31 */
  ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, /* 32-47 */
  ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, /* 48-63 */
  ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, '/', ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, /* 64-79 */
  ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_ENTER, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, /* 80-95 */
  ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_LEFT, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, /* 96-111 */
  ASCII_UNDEF, ASCII_LEFT, ASCII_DOWN, ASCII_UNDEF, ASCII_RIGHT, ASCII_UP, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, /* 112-127 */
};
