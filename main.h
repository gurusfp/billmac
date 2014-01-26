#ifndef MAIN_H
#define MAIN_H

#define DELAY(N) { 				\
  uint8_t ui1, ui2;				\
  for(ui1=0; ui1<N; ui1++)			\
    for(ui2=0; ui2<0xFF; ui2++)			\
      {} \
  }

#define DELAY1(N) { 				\
  uint8_t ui1;					\
  for(ui1=0; ui1<N; ui1++)			\
      {} \
  }

#define DISABLE_ALL_INTERRUPTS        EA  = 0
#define ENABLE_ALL_INTERRUPTS         EA  = 1

#define SLEEP_UNTIL_NEXT_INTR			\
     /* enter sleep state */			\
    PCON = IDL;					\
    __asm					\
      NOP					\
      NOP					\
      NOP					\
      NOP					\
      NOP					\
      __endasm


extern __sbit ps2ShiftHit, ps2CtrlHit, ps2AltHit;

#endif
