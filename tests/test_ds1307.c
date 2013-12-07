
#include "lcd.h"
#include "i2c.h"

#include "lcd.c"
#include "i2c.c"

int
main(void)
{
  lcdInit();
  i2cInit();

  LCD_busy
  LCD_CLRSCR

  LCD_WR_LINE(0, 0, "Hello World 7");
  for (ui1=0; ui1<10; ui1++)
    LCD_busy
  LCD_WR("Hello World 8");
}
