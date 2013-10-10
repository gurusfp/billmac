#include <stdint.h>
#include <p89v51rd2.h>

#include "kbd.h"
#include "main.h"

uint8_t KbdHit;
uint8_t KbdScanCode(void);

void
KbdInit(void)
{
  KBD_RC = 0xFF;
  KbdHit = 0xFF;
}

void
KbdScan(void) __naked
{
  /* scan keyboard & push */
  uint8_t ret = KbdScanCode();
  KBD_RC = 0xFF;

  KbdHit <<= 4;
  KbdHit |= ret;

  __asm
    RET
  __endasm;
}

uint8_t
KbdScanCode(void) __naked
{
    R4=0;
    DELAY1(0x4);
    if (KBD_C1==0) { while(KBD_C1==0) { DELAY1(0x4); } return 11;}
    if (KBD_C2==0) { while(KBD_C2==0) { DELAY1(0x4); } return 0;}
    if (KBD_C3==0) { while(KBD_C3==0) { DELAY1(0x4); } return 12;}
    if (KBD_C4==0) { while(KBD_C4==0) { DELAY1(0x4); } return 13;}
    KBD_R4=1;    KBD_R1=0;
    DELAY1(0x4);
    if (KBD_C1==0) { while(KBD_C1==0) { DELAY1(0x4); } return 1; }
    if (KBD_C2==0) { while(KBD_C2==0) { DELAY1(0x4); } return 2; }
    if (KBD_C3==0) { while(KBD_C3==0) { DELAY1(0x4); } return 3; }
    if (KBD_C4==0) { while(KBD_C4==0) { DELAY1(0x4); } return 10;}
    KBD_R1=1;    KBD_R2=0;
    DELAY1(0x4);
    if (KBD_C1==0) { while(KBD_C1==0) { DELAY1(0x4); } return 4; }
    if (KBD_C2==0) { while(KBD_C2==0) { DELAY1(0x4); } return 5; }
    if (KBD_C3==0) { while(KBD_C3==0) { DELAY1(0x4); } return 6; }
    /* One key removed */
    KBD_R2=1;    KBD_R3=0;
    DELAY1(0x4);
    if (KBD_C1==0) { while(KBD_C1==0) { DELAY1(0x4); } return 7; }
    if (KBD_C2==0) { while(KBD_C2==0) { DELAY1(0x4); } return 8; }
    if (KBD_C3==0) { while(KBD_C3==0) { DELAY1(0x4); } return 9; }
    if (KBD_C4==0) { while(KBD_C4==0) { DELAY1(0x4); } return 13;}

    return 0xFF;
}
