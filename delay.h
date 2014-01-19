#ifndef DELAY_H
#define DELAY_H

#define delayms(ms) {				\
    uint16_t ui1;				\
    uint8_t  ui2;				\
    for(ui1=0; ui1<ms; ui1++)			\
      for(ui2=0; ui2<0xFF; ui2++) {}		\
}

#endif
