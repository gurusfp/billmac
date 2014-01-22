#include<stdint.h>
#include<p89v51rd2.h>

#define  assert(X)

#include "delay.h"
#include "lcd.h"
#include "kbd.h"
#include "uart.h"
#include "main.h"

#include "lcd.c"
#include "kbd.c"
#include "uart.c"

#define MAIN_NOMAIN
#include "main.c"

int
main(void)
{
  uint16_t ui1;
  uint8_t ui2, ui3;

  LCD_init();
  Timer_Init();
  uart_init();

  delayms(2);
  LCD_CLRSCR;

  LCD_WR_LINE(0, 0, "Testing PS2KBD!!!");
  delayms(100);
  LCD_POS(1, 0);

  while (1) {}

  return 0;
}
