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
  uint8_t ymd[3];

  LCD_init();
  i2cInit();
  Timer_Init();
  KbdInit();

  LCD_busy
  LCD_CLRSCR

  LCD_WR_LINE(0, 0, "Starting!!!");
  for (ui1=0; ui1<0x3F; ui1++)
    LCD_busy;

  LCD_WR_LINE(0, 0, "DS1307 Tests!!!");
  KBD_RESET_KEY;
//  while (1) {
    timerDateGet(ymd);
    LCD_POS(1, 0);
    LCD_PUTCH('0'+(ymd[0]>>4));
    LCD_PUTCH('0'+(ymd[0]&0xF));
    LCD_PUTCH('/');
    LCD_PUTCH('0'+(ymd[1]>>4));
    LCD_PUTCH('0'+(ymd[1]&0xF));
    LCD_PUTCH('/');
    LCD_PUTCH('0'+(ymd[2]>>4));
    LCD_PUTCH('0'+(ymd[2]&0xF));
//    for (ui1=0; ui1<0xFF; ui1++)
//      {}
//  }

  return 0;
}
