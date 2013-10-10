#ifndef KBD_H
#define KBD_H

/* Keypad Connections */
#define KBD_R1    P0_0
#define KBD_R2    P0_1
#define KBD_R3    P0_2
#define KBD_R4    P0_3
#define KBD_C1    P0_4
#define KBD_C2    P0_5
#define KBD_C3    P0_6
#define KBD_C4    P0_7
#define KBD_RC    P0

/* Key definitions */
#define KEY_SC_1         1
#define KEY_SC_2         2
#define KEY_SC_3         3
#define KEY_SC_4         4
#define KEY_SC_5         5
#define KEY_SC_6         6
#define KEY_SC_7         7
#define KEY_SC_8         8
#define KEY_SC_9         9
#define KEY_SC_0         0
#define KEY_SC_PRINT    10
#define KEY_SC_LEFT     11
#define KEY_SC_RIGHT    12
#define KEY_SC_ENTER    13
#define KEY_SC_INVALID  15

extern void    KbdInit(void);
extern uint8_t KbdScan(void);
extern uint8_t KbdHit;

#endif
