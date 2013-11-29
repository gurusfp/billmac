#include <stdint.h>
#include <p89v51rd2.h>

#include "uart.h"

// UART functions for SDCC / P89V51RD2

/* Original Code by:
 * Author       : Chin-Shiuh Shieh (http://bit.kuas.edu.tw/~csshieh/)
 * Date         : 2001-01-01 (yyyy-mm-dd)
 */

/* Modified for ucmicrosys UNI-51-SDK (www.ucmicrosys.com) by
 * Author(s)    : Anurag Chugh (anurag@ucmicrosys.com) and Ranjit Puri (ranjit.puri@gmail.com)
 * Date         : 2006-12-15 (yyyy-mm-dd)
 */

//NOTES:
//1: This code may be distributed and modified freely.
//2: uart_init() should be called before using UART. It is used to initialise the SFRs
//3: No syntax error handling is performed by the functions.
//4: Comment un-used functions to reduce code size.
//5: External Crystal of 11.0592 MHz is assumed.
//6: UART parameters are pre-set to 9600 bps 8-N-1

void
uart_init(void)
{
_asm
  anl     PCON,#0x7F      // Clear SMOD of PCON, No Double Baud Rate.
  anl     TMOD,#0xAF      // Clear C/T, M0 for Timer1 of TMOD.
  orl     TMOD,#0x20      // Set M1 for Timer1 of TMOD.
                          // Set Timer1 to Mode 2 (8-bit auto reload) for Baud Rate Generation.
  mov     TH1,#0xFD       // Set Baud Rate to 9600 bps.
  clr     SM0             // Clear SM0 of SCON.
  setb    SM1             // Set SM1 of SCON.
                          // Set UART to Mode 1 (8-bit UART).
  setb    REN             // Set REN of SCON to Enable UART Receive.
  setb    TR1             // Set TR1 of TCON to Start Timer1.
  setb    TI              // Set TI of SCON to Get Ready to Send.
  clr     RI              // Clear RI of SCON to Get Ready to Receive.
_endasm;
}

void
uart_putc(uint8_t c)  // Put Character to UART.
{
  while(TI==0);
  TI=0;
  SBUF=c;
}

uint8_t
uart_getc(void)    // Get Character from UART.
{
  while(RI==0);
  RI=0;
  return SBUF;
}

#if 0
uint8_t
uart_getce(void)   // Get Character from UART with Echo, (calls uart_putc()).
{
  while(RI==0);
  RI=0;
  uart_putc(SBUF);
  return SBUF;
}

void
uart_puts(uint8_t *s) // Put String to UART, (calls uart_putc()).
{
  while(*s!=0) {
    uart_putc(*s);
    s++;
  }
}

void
uart_gets(uint8_t *s) // Get String from UART, (calls uart_getce()).
{
  while(((*s)=uart_getce())!=13) {
    s++;
  }
  *s=0;
}

void
i2s(int i, uint8_t *s) // Convert Interger to String.
{
  uint8_t sign;
  short len;
  uint8_t *p;

  sign='+';
  len=0;
  p=s;

  if (i<0) {
    sign='-';
    i=-i;
  }

  do {
    *s=(i%10)+'0';
    s++;
    len++;
    i/=10;
  } while (i!=0);

  if (sign=='-') {
    *s='-';
    s++;
    len++;
  }

  for(i=0;i<len/2;i++) {
    p[len]=p[i];
    p[i]=p[len-1-i];
    p[len-1-i]=p[len];
  }

  p[len]=0;
}

int
s2i(uint8_t *s)                // Convert String to Integer.
{
  int i=0;
  uint8_t sign='+';

  if(*s=='-') {
    sign='-';
    s++;
  }

  while(*s!=0) {
    i=i*10+(*s-'0');
    s++;
  }

  if(sign=='-') {
    i=-i;
  }

  return i;
}

void
uart_puti(int i)           // Put Integer to UART, (calls i2s() & uart_puts()).
{
  uint8_t s[7];
  i2s(i,s);
  uart_puts(s);
}

int
uart_geti()                 // Get Integer from UART, (calls uart_gets() & s2i()).
{
  uint8_t s[7];
  uart_gets(s);
  return s2i(s);
}
#endif
