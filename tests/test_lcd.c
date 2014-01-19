#include<stdint.h>
#include<p89v51rd2.h>

#define  assert(X)

#include "lcd.h"
#include "i2c.h"
#include "kbd.h"
#include "main.h"

#include "lcd.c"
#include "i2c.c"
#include "kbd.c"

#define MAIN_NOMAIN
#include "main.c"

int
main(void)
{
  uint16_t ui1;

  LCD_init();
  i2cInit();
  Timer_Init();

  LCD_busy
  LCD_CLRSCR

  LCD_WR_LINE(0, 0, "Hello World 7");
  for (ui1=0; ui1<0x4FF; ui1++)
    LCD_busy;
  LCD_WR_LINE(1, 0, "Hello World 8");
  for (ui1=0; ui1<0x4FF; ui1++)
    LCD_busy;

  return 0;
}
