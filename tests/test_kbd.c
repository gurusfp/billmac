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
  LCD_WR_LINE(1, 0, "Shall displayed!");
  KBD_RESET_KEY;
  while (1) {
    if (KBD_HIT) {
      KBD_GET_KEY;
      LCD_POS(1, 0);
      LCD_WR("Scan Code : ");
      LCD_PUT_UINT8X(KbdData);
      KBD_RESET_KEY;
    }
    for (ui1=0; ui1<0xFF; ui1++)
      {}
  }

  return 0;
}
