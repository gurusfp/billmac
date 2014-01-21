#ifndef UART_H
#define UART_H

void uart_init(void);           // Set UART Baud Rate to 9600 bps.
void uart_putc(uint8_t c);      // Put Character to UART.

#define UART_RCV_BUF_SIZE 8

extern uint8_t uart_rcvnum, uart_rcvdata[UART_RCV_BUF_SIZE];

#define UART_RCV_DATA_PUSH					\
  uart_rcvdata[uart_rcvnum] = SBUF;				\
  uart_rcvnum++

#define UART_RCV_DATA_POP(val)					\
  if UART_DATA_AVAIL {						\
    uint8_t ui2_t;						\
    DISABLE_ALL_INTERRUPTS;					\
    val = uart_rcvdata[0];					\
    uart_rcvnum--;						\
    for (ui2_t=0; ui2_t<uart_rcvnum; ui2_t++) {			\
      uart_rcvdata[ui2_t] = uart_rcvdata[ui2_t+1];		\
    }								\
    ENABLE_ALL_INTERRUPTS;					\
  }

#define UART_DATA_AVAIL (0 != uart_rcvnum)
#define UART_DATA_BUF_FULL (uart_rcvnum >= UART_RCV_BUF_SIZE)

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
