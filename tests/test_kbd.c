#include<stdint.h>
#include<p89v51rd2.h>

#define  assert(X)

#include "lcd.h"
#include "i2c.h"
#include "kbd.h"

#include "lcd.c"
#include "i2c.c"
#include "kbd.c"

#define MAIN_NOMAIN
#include "main.c"

int
main(void)
{
  uint16_t ui1;
  uint8_t key, key_n, key_s;

  LCD_init();
  i2cInit();
  Timer_Init();
  KbdInit();

  LCD_busy
  LCD_CLRSCR

  LCD_WR_LINE(0, 0, "Starting!!!");
  for (ui1=0; ui1<0x3F; ui1++)
    LCD_busy;

  LCD_WR_LINE(0, 0, "Press any key!!!");
  LCD_WR_LINE(1, 0, "That be displayed");
  KBD_RESET_KEY;
  while (1) {
    KBD_GET_KEY;
    if (KBD_HIT) {
      LCD_WR_LINE(1, 0, "Key Press : ");
      LCD_PUTCH('0'+key);
      KBD_RESET_KEY;
      LCD_busy;
    }
    for (ui1=0; ui1<0xFF; ui1++)
      LCD_busy;
  }

  return 0;
}
