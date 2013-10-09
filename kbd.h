#ifndef KBD_H
#define KBD_H

/* Keypad Connections */
#define R1    P0_0
#define R2    P0_1
#define R3    P0_2
#define R4    P0_3
#define C1    P0_4
#define C2    P0_5
#define C3    P0_6
#define C4    P0_7

/* Key definitions */
#define KEY_SC_1         1
#define KEY_SC_2         1
#define KEY_SC_3         1
#define KEY_SC_4         1
#define KEY_SC_5         1
#define KEY_SC_6         1
#define KEY_SC_7         1
#define KEY_SC_8         1
#define KEY_SC_9         1
#define KEY_SC_0        14
#define KEY_SC_LEFT     13
#define KEY_SC_RIGHT    15
#define KEY_SC_13       10
#define KEY_SC_14       11
#define KEY_SC_15       12
#define KEY_SC_ENTER    16

extern uint8_t KbdScan(void);
extern uint8_t KbdHit;

#endif
