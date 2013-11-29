#ifndef UART_H
#define UART_H

// UART functions for SDCC / P89V51RD2

/* Original Code by:
 * Author       : Chin-Shiuh Shieh (http://bit.kuas.edu.tw/~csshieh/)
 * Date         : 2001-01-01 (yyyy-mm-dd)
 */

/* Modified for ucmicrosys UNI-51-SDK (www.ucmicrosys.com) by
 * Author(s)    : Anurag Chugh (anurag@ucmicrosys.com) and Ranjit Puri (ranjit.puri@gmail.com)
 * Date         : 2006-12-15 (yyyy-mm-dd)
 */

// UART function protoypes
void uart_init(void);           // Set UART Baud Rate to 9600 bps.
void uart_putc(uint8_t c);      // Put Character to UART.
uint8_t uart_getc(void);        // Get Character from UART.

#if 0
uint8_t uart_getce(void);       // Get Character from UART with Echo, Requires uart_putc().
void uart_puts(uint8_t *s);     // Put String to UART, Requires uart_putc().
void uart_gets(uint8_t *s);     // Get String from UART, Requires uart_getce().
void i2s(int i,uint8_t *s);     // Convert Interger to String.
int s2i(uint8_t *s);            // Convert String to Integer.
void uart_puti(int i);          // Put Integer to UART, Requires i2s() & uart_puts().
int uart_geti();                // Get Integer from UART, Requires uart_gets() & s2i().
#endif

#endif
