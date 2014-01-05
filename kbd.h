#ifndef KBD_H
#define KBD_H

/* Keypad Connections */
#define KBD_R1           P0_0
#define KBD_R2           P0_1
#define KBD_R3           P0_2
#define KBD_R4           P0_3
#define KBD_C1           P0_4
#define KBD_C2           P0_5
#define KBD_C3           P0_6
#define KBD_C4           P0_7
#define KBD_RC           P0
#define KBD_PS2_CLK      P3_2
#define KBD_PS2_DATA     P3_3

/* Key definitions */
#define ASCII_UNDEF      0
#define ASCII_ENTER      0xA0
#define ASCII_BACKSPACE  0xA1
#define ASCII_NUMLK      0xA2
#define ASCII_PRNSCRN    0xA3
#define ASCII_DEL        ASCII_BACKSPACE
#define ASCII_LEFT       0xA4
#define ASCII_RIGHT      0xA5
#define ASCII_UP         0xA6
#define ASCII_DOWN       0xA7

#define KBD_GET_KEY	  \
  KbdGetCh()

#define KBD_RESET_KEY          \
  KbdDataAvail = 0 ; KbdData = 0xFF

#define KBD_HIT      (0x0 != KbdDataAvail)
#define KBD_NOT_HIT  (0x0 == KbdDataAvail)

void    KbdInit(void);
void    KbdScan(void);
uint8_t KbdIsShiftPressed(void);
void    KbdGetCh(void);
extern uint8_t KbdData;
extern __sbit  KbdDataAvail;
extern uint8_t ps2code2ascii[];
extern uint8_t ps2code2asciiE0[];

#endif
