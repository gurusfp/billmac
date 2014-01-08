#include <stdio.h>

static FILE *op=NULL;

void
uart_init(void)
{
  op = fopen("uart.output", "w");
}

void
uart_putc(uint8_t c)
{
  fputc(c, op);
}

uint8_t
uart_getc(void)
{
  assert(0);
}
