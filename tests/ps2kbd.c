#include <p89v51rd2.h>  
#include <stdint.h>  
   
void
delayms(uint16_t ms)
{   
  unsigned char i;   
  while(ms--)   
    {   
      for(i = 0; i < 120; i++);   
    }   
}   

/*********************************LCD HEADER FILE START*************************************/
#define First_Line  0x80
#define Second_Line 0xc0
#define Curser_On   0x0f
#define Curser_Off  0x0c
#define Clear_Display 0x01
 
#define LCD_rs P2_0
#define LCD_en P2_1
 
void Lcd8_Init();
void Lcd8_Write(unsigned char,unsigned char);
void Lcd8_Display(unsigned char,const unsigned char*,unsigned int);
void Lcd8_Decimal2(unsigned char,unsigned char);
void Lcd8_Decimal3(unsigned char,unsigned char);
void Lcd8_Decimal4(unsigned char,unsigned int);

# define LCD_PORT(val)       \
  P2_2 = (val & 0x1);	     \
  P2_3 = ((val>>1) & 0x1);   \
  P2_4 = ((val>>2) & 0x1);   \
  P2_5 = ((val>>3) & 0x1)

# define LCD_cmd(var)   \
  LCD_PORT(var>>4);	\
  LCD_rs = 0;		\
  LCD_en = 1;		\
  LCD_en = 0;		\
  LCD_PORT(var);	\
  LCD_rs = 0;		\
  LCD_en = 1;		\
  LCD_en = 0;		\
  delayms(10)

# define LCD_wrnib(var)	\
  LCD_PORT(var);	\
  LCD_rs = 1;		\
  LCD_en = 1;		\
  LCD_en = 0

# define LCD_wrchar(var)\
  LCD_wrnib(var>>4);	\
  LCD_wrnib(var);	\
  delayms(10)

void Lcd8_Init()
{
  delayms(100);
  LCD_wrnib(3);
  delayms(100);
  LCD_wrnib(3);
  delayms(10);
  LCD_wrnib(3);
  delayms(10);
  LCD_wrnib(2);
  delayms(10);

  LCD_cmd(0x38); //to select function set
  LCD_cmd(0x38); //to select function set
  LCD_cmd(0x38); //to select function set
  LCD_cmd(0x38); //to select function set
  delayms(100);
  LCD_cmd(0x06); //entry mode set
  delayms(100);
  LCD_cmd(0x0c); //display on
  delayms(100);
  LCD_cmd(0x01); //clear display
}

void Lcd8_Write(unsigned char com,unsigned char lr)
{
  LCD_cmd(com);
  LCD_wrchar(lr);
}
 
void Lcd8_Display(unsigned char com,const unsigned char *word,unsigned int n)
{
  unsigned char Lcd_i;
 
  for(Lcd_i=0;Lcd_i<n;Lcd_i++)
    { 
      Lcd8_Write(com+Lcd_i,word[Lcd_i]);
    }
}
 
void Lcd8_hex(unsigned char com,unsigned char val)
{
  unsigned int Lcd_t, Lcd_o;
 
  Lcd_t = (val>>4) & 0xF;
  Lcd_o = val & 0xF;
 
  Lcd8_Write(com,((Lcd_t>9)?'a'-10:'0')+Lcd_t);
  Lcd8_Write(com+1,((Lcd_o>9)?'a'-10:'0')+Lcd_o);
}
 
void Lcd8_Decimal2(unsigned char com,unsigned char val)
{
  unsigned int Lcd_hr,Lcd_t,Lcd_o;
 
  Lcd_hr=val%100;
  Lcd_t=Lcd_hr/10;
  Lcd_o=Lcd_hr%10;
 
  Lcd8_Write(com,Lcd_t+0x30);
  Lcd8_Write(com+1,Lcd_o+0x30);
}
 
 
void Lcd8_Decimal3(unsigned char com,unsigned char val)
{
  unsigned int Lcd_h,Lcd_hr,Lcd_t,Lcd_o;
 
  Lcd_h=val/100;
  Lcd_hr=val%100;
  Lcd_t=Lcd_hr/10;
  Lcd_o=Lcd_hr%10;
 
  Lcd8_Write(com,Lcd_h+0x30);
  Lcd8_Write(com+1,Lcd_t+0x30);
  Lcd8_Write(com+2,Lcd_o+0x30);
}
 
void Lcd8_Decimal4(unsigned char com,unsigned int val) 
{
  unsigned int Lcd_th,Lcd_thr,Lcd_h,Lcd_hr,Lcd_t,Lcd_o;
 
  val = val%10000;
  Lcd_th=val/1000;
  Lcd_thr=val%1000;
  Lcd_h=Lcd_thr/100;
  Lcd_hr=Lcd_thr%100;
  Lcd_t=Lcd_hr/10;
  Lcd_o=Lcd_hr%10;
 
  Lcd8_Write(com,Lcd_th+0x30);
  Lcd8_Write(com+1,Lcd_h+0x30);
 
  Lcd8_Write(com+2,Lcd_t+0x30);
  Lcd8_Write(com+3,Lcd_o+0x30);
}
 
/*********************************LCD HEADER FILE END*************************************/

#define DISABLE_ALL_INTERRUPTS        EA = 0
#define ENABLE_ALL_INTERRUPTS         EA = 1

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

/* Key definitions, free to use >127 */
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

#define KBD_RESET_CMD    0xFF
#define KBD_LED_CMD      0xED

__code const uint8_t
ps2code2ascii[] = {
  ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, '`', ASCII_UNDEF, /* 0-15 */
  ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, 'q', '1', ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, 'z', 's', 'a', 'w', '2', ASCII_UNDEF, /* 16-31 */
  ASCII_UNDEF, 'c', 'x', 'd', 'e', '4', '3', ASCII_UNDEF, ASCII_UNDEF, ' ', 'v', 'f', 't', 'r', '5', ASCII_UNDEF, /* 32-47 */
  ASCII_UNDEF, 'n', 'b', 'h', 'g', 'y', '6', ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, 'm', 'j', 'u', '7', '8', ASCII_UNDEF, /* 48-63 */
  ASCII_UNDEF, ',', 'k', 'i', 'o', '0', '9', ASCII_UNDEF, ASCII_UNDEF, '.', '/', 'l', ';', 'p', '-', ASCII_UNDEF, /* 64-79 */
  ASCII_UNDEF, ASCII_UNDEF, '\'', ASCII_UNDEF, '[', '=', ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_ENTER, ']', ASCII_UNDEF, '\\', ASCII_UNDEF, ASCII_UNDEF, /* 80-95 */
  ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, ASCII_BACKSPACE, ASCII_UNDEF, ASCII_UNDEF, '1', ASCII_UNDEF, '4', '7', ASCII_UNDEF, ASCII_UNDEF, ASCII_UNDEF, /* 96-111 */
  '0', '.', '2', '5', '6', '8', ASCII_UNDEF, ASCII_NUMLK, ASCII_UNDEF, '+', '3', '-', '*', '9', ASCII_PRNSCRN, ASCII_UNDEF, /* 112-127 */
};

//PS/2- Keyboard   Pins
#define KBD_PS2_DATA   P3_6
#define KBD_PS2_CLK    P3_2
#define LENOF_DR          3
#define KBD_RESET_KEY				\
  KBD_PS2_CLK = 1; EX0 = 1


uint8_t  KbdData, KbdTransData;
__sbit   KbdDataAvail, ps2ShiftHit, ps2CtrlHit, ps2AltHit, ps2CapsHit;
__sbit   KbdDataTrasmitPend, KbdDataTrasmitInProg, ps2SwitchModified;

uint8_t KeyData, bitC, drC, DR0, DR1, DR2, DR3, transL;

void
Timer0_isr(void)
{
  DISABLE_ALL_INTERRUPTS;

  if ((KbdDataTrasmitPend) && (0 == bitC) && (1 == KBD_PS2_CLK)) {
    KbdDataTrasmitInProg = 1;
    KBD_PS2_CLK = 0;
  }

  ENABLE_ALL_INTERRUPTS;
}

void
ex0_isr(void) __interrupt(IE0_VECTOR) /* INT0 P3_2 (Clock) */
{             /* Data come with Clock from Device to MCU together */
  EX0 = 0; /* FIXME: EX0 need not be altered */

  /* next bit to be processed */
  bitC++;

  /* Transmission */
  if (KbdDataTrasmitInProg) {
    if ((bitC >=2) && (bitC <= 9)) {
      KBD_PS2_DATA = (KbdTransData>>(bitC-2)) & 0x1;
    } else if (10 == bitC) {
      DR3 = 1;
      for (DR2=1; DR2; DR2<<=1) {
	DR3 ^= (KbdTransData & DR2) ? 1 : 0;
      }
      KBD_PS2_DATA = DR3;
    } else if (11 == bitC) {
      KBD_PS2_DATA = 1; /* ack : we should float the bus */
    } else if (12 == bitC) { /* transfer complete */
      KbdDataTrasmitInProg = 0;
      KbdDataTrasmitPend = 0;
      bitC = 0;
    } else if (1 == bitC) {
      DISABLE_ALL_INTERRUPTS;
      /* trigger KBD transmission */
      delayms(10);
      KBD_PS2_DATA = 0;
      delayms(100);
      KBD_PS2_CLK = 1;
      ENABLE_ALL_INTERRUPTS;
    }
    return;
  }

  /* Reception ------------------------------------- */
  if (11 == bitC) {
    bitC = 0;
    /* disable further transmissions */
    while (0 == KBD_PS2_CLK) {};
    KBD_PS2_CLK = 0;
  } else if (1 == bitC) {
    KeyData = 0;
    transL = 2;
  } else if (10 == bitC) {
    /* FIXME: Check parity and blink an LED */
  } else {
    KeyData >>= 1;
    KeyData |= (((uint8_t)KBD_PS2_DATA)<<7);
  }

  if (0 != bitC) {
    EX0 = 1;
    return;
  }

  if (0 == drC) {
    DR0 = KeyData;
  } else if (1 == drC) {
    DR1 = KeyData;
  } else if (2 == drC) {
    DR2 = KeyData;
  } else if (3 == drC) {
    DR3 = KeyData;
  }
  drC++;
 
  /* --------------------------------------- */
  if (DR0 == 0xE0) {
    transL = 2;    /* E0 XX */
    if (DR1 == 0x12) {    /* E0 12 E0 7C */
      transL = 4;
    }
    if (DR1 == 0xF0) {
      transL = 3;    /* E0 F0 XX */
      if (DR2 == 0x7C) {    /* E0 F0 7C E0 F0 12 */
	transL = 6;
      } else { /* E0 F0 XX */
	if ( drC >= transL) {
	  KeyData = (0x4A == KeyData) ? '/' : (0x5A == KeyData) ? ASCII_ENTER :
	    (0x71 == KeyData) ? ASCII_LEFT : (0x6B == KeyData) ? ASCII_LEFT :
	    (0x72 == KeyData) ? ASCII_DOWN : (0x74 == KeyData) ? ASCII_RIGHT :
	    (0x75 == KeyData) ? ASCII_UP : ASCII_UNDEF;
	  if (ASCII_UNDEF != KeyData) {
	    KbdData = KeyData;
	    KbdDataAvail = 1;
	  }
	}
      }
    }
  } else if (DR0 == 0xF0) {
    transL = 2;    /* F0 XX */
    if (transL <= drC) { /* Break of normal keys */
      if ((0x12 == DR1) || (0x59 == DR1))
	ps2ShiftHit = 0, ps2SwitchModified = 0;
      else if (0x14 == DR1)
	ps2CtrlHit = 0, ps2SwitchModified = 0;
      else if (0x11 == DR1)
	ps2AltHit = 0, ps2SwitchModified = 0;
      else if (0x58 == DR1)
	ps2CapsHit = 0, ps2SwitchModified = 1;
      else {
	KeyData = ps2code2ascii[KeyData];
        if (ASCII_UNDEF != KeyData) {
	  KbdData = KeyData;
	  KbdDataAvail = 1;
	}
      }
    }
  } else if (0xE1 == DR0) {
    transL = 8;
  } else {
    /* Make code received, generally no action except for sticky keys */
    transL = 1;
    if (transL <= drC) {
      if ((0x12 == DR0) || (0x59 == DR0))
	ps2ShiftHit = 1, ps2SwitchModified = 0;
      else if (0x14 == DR0)
	ps2CtrlHit = 1, ps2SwitchModified = 0;
      else if (0x11 == DR0)
	ps2AltHit = 1, ps2SwitchModified = 0;
      else if (0x58 == DR0)
	ps2CapsHit = 1, ps2SwitchModified = 1;
      /* else if (0xFA == DR0) ACKNOWLEDGEMENT FROM KBD */
    }
  }

  /* --------------------------------------- */
  if (drC >= transL) {
    drC = 0;
    if (0 == KbdDataAvail) {
      KBD_PS2_CLK = 1;
      EX0 = 1;
    }
  } else {
    KBD_PS2_CLK = 1;
    EX0 = 1;
  }
}

void
KbdTransmit(uint8_t Data)
{
  while (1 == KbdDataTrasmitPend) {
    SLEEP_UNTIL_NEXT_INTR;
  }
  KbdDataTrasmitPend = 1;
  KbdDataTrasmitInProg = 0;
  KbdTransData = Data;
}

void KbdInit()
{
//  uint8_t *ui_p = ((uint8_t *)0x8);

//  *ui_p++ = 0, *ui_p++ = 0, *ui_p++ = 0, *ui_p++ = 0;
//  *ui_p++ = 0, *ui_p++ = 0, *ui_p++ = 0, *ui_p++ = 0;

  bitC = 0;
  drC = 0;
  KbdDataAvail = 0;
  KbdDataTrasmitPend = 0;
  KbdDataTrasmitInProg = 0;
  ps2SwitchModified = 0;

  EX0 = 1;      /* enable INT0 */
  IT0 = 1;      /* edge detect */

  EA  = 1;

  /* Reset keyboard */
  KbdTransmit(KBD_RESET_CMD);
  delayms(100);
  KbdTransmit(KBD_LED_CMD);
  delayms(100);
  KbdTransmit(ps2CapsHit ? 0x04: 0x00);
}

/***************************/
/*Main Program Starts Here */
/***************************/
void main()
{
  uint8_t idx;
  uint8_t ui1, ui2, ui3;

  Lcd8_Init();
  KbdInit();
  Lcd8_Display(0x80, "PS2 TESTING:    ",16);
  delayms(100);delayms(100);
  LCD_cmd(0x01);

  delayms(50);
  KBD_PS2_CLK = 1;
  KBD_PS2_DATA = 1;
  delayms(50);

  idx = 0;
  ui2 = ui1 = 0;
  Lcd8_Display(First_Line, "Scan Code: ", 11);
  while (1) {
    KbdDataAvail = 0;
    while (0 == KbdDataAvail) {
      delayms(1);
      if (ps2SwitchModified) {
	KbdTransmit(KBD_LED_CMD);
	KbdTransmit(ps2CapsHit ? 0x04: 0x00);
	ps2SwitchModified = 0;
      }
      ui1++;
      if (0 == ui1) {
	Lcd8_hex(First_Line, bitC);
	LCD_wrchar('!');
	Lcd8_hex(First_Line+3, drC);
	LCD_wrchar('@');
	Lcd8_hex(First_Line+6, transL);
	LCD_wrchar('#');
	Lcd8_hex(First_Line+9, DR0);
	LCD_wrchar('$');
	Lcd8_hex(First_Line+12, DR1);
//	LCD_wrchar('%');
	ui2 &= 0x80;
	ui2 ^= 0x80;
	ui3 = KbdDataAvail;
	ui2 |= (ui3<<0);
	ui3 = EX0;
	ui2 |= (ui3<<1);
	ui3 = KBD_PS2_CLK;
	ui2 |= (ui3<<2);
	ui3 = ps2ShiftHit;
	ui2 |= (ui3<<3);
	ui3 = ps2CtrlHit;
	ui2 |= (ui3<<4);
	ui3 = ps2AltHit;
	ui2 |= (ui3<<5);
	ui3 = ps2CapsHit;
	ui2 |= (ui3<<6);
	Lcd8_hex(First_Line+14, ui2);
      }
    }
    if (idx > 15) idx = 0;
    Lcd8_hex(Second_Line+idx, KbdData);
    LCD_wrchar(':');
    LCD_wrchar(KbdData);
    idx += 4;
    KBD_RESET_KEY;
  }
}
