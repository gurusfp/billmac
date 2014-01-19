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

  LCD_WR_LINE(0, 0, "Testing UART!!!");
  delayms(100);
  LCD_POS(1, 0);
#if 1
  for (ui3=0; ui3<8; ui3++) {
    LCD_POS(1, 0);
    ui2 = 0xEE;
    LCD_PUT_UINT8X(ui2);
    UART_DATA_RECIEVE(ui2);
    LCD_PUT_UINT8X(ui2);
  }
#else
  ui2 = 0xAA;
  for (ui3=0; ui3<4; ui3++) {
    LCD_PUT_UINT8X(ui2);
    uart_putc(ui2);
    ui2 ++;
  }
#endif

  for (ui1=0; ui1<0xFF; ui1++)
    delayms(2);

  return 0;
}
