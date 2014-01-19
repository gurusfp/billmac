#ifndef UART_H
#define UART_H

void uart_init(void);           // Set UART Baud Rate to 9600 bps.
void uart_putc(uint8_t c);      // Put Character to UART.

#define UART_RCV_BUF_SIZE 8

extern uint8_t uart_rcvnum, uart_rcvdata[UART_RCV_BUF_SIZE];

#define UART_RCV_DATA_PUSH		\
  uart_rcvdata[uart_rcvnum] = SBUF;	\
  LCD_PUT_UINT8X(uart_rcvdata[uart_rcvnum]); \
  uart_rcvnum++

#define UART_RCV_DATA_POP(val)		\
  if (uart_rcvnum > 0) {		\
    uart_rcvnum--;			\
    val = uart_rcvdata[uart_rcvnum];	\
  }

#define UART_DATA_AVAIL (0 != uart_rcvnum)

/* Block until data is available */
#define UART_DATA_RECIEVE(val)	 \
  while (1) {			 \
    if (UART_DATA_AVAIL)	 \
      break;			 \
    /* enter sleep state */	 \
    PCON = IDL;			 \
    __asm			 \
      NOP			 \
      NOP			 \
      NOP			 \
      NOP			 \
      NOP			 \
      __endasm;			 \
  }				 \
  UART_RCV_DATA_POP(val)

#endif
