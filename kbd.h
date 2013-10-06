#ifndef KBD_H
#define KBD_H

//Keypad Connections
#define R1    P0_0
#define R2    P0_1
#define R3    P0_2
#define R4    P0_3
#define C1    P0_4
#define C2    P0_5
#define C3    P0_6
#define C4    P0_7

extern uint8_t KbdScan(void);
extern uint8_t KbdHit;

#endif
