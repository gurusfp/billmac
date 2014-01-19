#include <stdint.h>
#include <p89v51rd2.h>

#include "uart.h"

uint8_t uart_rcvnum, uart_rcvdata[UART_RCV_BUF_SIZE];

void
uart_init(void)
{
  /* Setup Timer 1, to be used for Mode 1 UART */
  PCON &= 0xBF;     // Clear SMOD0 of PCON, No Double Baud Rate.
  TMOD |= 0x20;
  TH1 = 0xFD;       // Set Baud Rate to 9600 bps.
  SM0 = 0; SM1 = 1; // Mode 1 of UART
  TR1  = 1;         // Start timer
  TI = 1; RI = 0;
  REN = 1;          // Receive Enable
  ES = 1;           // UART/SPI Interrupt enable

  /* */
  uart_rcvnum = 0;
}

void
uart_putc(uint8_t c)  // Put Character to UART.
{
  SBUF = c;
  while (TI==0);
  TI=0;
}
