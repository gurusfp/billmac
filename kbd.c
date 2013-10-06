#include <stdint.h>
#include <p89v51rd2.h>

#include "kbd.h"
#include "main.h"

uint8_t KbdHit;
uint8_t KbdScanCode(void);

uint8_t
KbdScan(void)
{
  uint8_t ret = KbdScanCode();
  C1=1;    C2=1;    C3=1;    C4=1;
  R1=1;    R2=1;    R3=1;    R4=1;

  return ret;
}

uint8_t
KbdScanCode(void)
{
    /* scan keyboard & push */
    C1=1;    C2=1;    C3=1;    C4=1;
    R1=0;    R2=1;    R3=1;    R4=1;
    DELAY1(0x4);
    if (C1==0) { while(C1==0) { DELAY1(0x4); } return 1; }
    if (C2==0) { while(C2==0) { DELAY1(0x4); } return 2; }
    if (C3==0) { while(C3==0) { DELAY1(0x4); } return 3; }
    if (C4==0) { while(C4==0) { DELAY1(0x4); } return 10;}
    R1=1;    R2=0;    R3=1;    R4=1;
    DELAY1(0x4);
    if (C1==0) { while(C1==0) { DELAY1(0x4); } return 4; }
    if (C2==0) { while(C2==0) { DELAY1(0x4); } return 5; }
    if (C3==0) { while(C3==0) { DELAY1(0x4); } return 6; }
    if (C4==0) { while(C4==0) { DELAY1(0x4); } return 11;}
    R1=1;    R2=1;    R3=0;    R4=1;
    DELAY1(0x4);
    if (C1==0) { while(C1==0) { DELAY1(0x4); } return 7; }
    if (C2==0) { while(C2==0) { DELAY1(0x4); } return 8; }
    if (C3==0) { while(C3==0) { DELAY1(0x4); } return 9; }
    if (C4==0) { while(C4==0) { DELAY1(0x4); } return 12;}
    R1=1;    R2=1;    R3=1;    R4=0;
    DELAY1(0x4);
    if (C1==0) { while(C1==0) { DELAY1(0x4); } return 13;}
    if (C2==0) { while(C2==0) { DELAY1(0x4); } return 14;}
    if (C3==0) { while(C3==0) { DELAY1(0x4); } return 15;}
    if (C4==0) { while(C4==0) { DELAY1(0x4); } return 16;}

    return 0;
}
