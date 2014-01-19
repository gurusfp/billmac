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
  uint16_t ui1, ui2;
  uint8_t  data[10] = { 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1 }, rval[10];

  LCD_init();
  i2cInit();
  Timer_Init();
  KbdInit();

  LCD_busy
  LCD_CLRSCR

  LCD_WR_LINE(0, 0, "Starting!!!");
  for (ui1=0; ui1<0x3F; ui1++)
    LCD_busy;

  LCD_WR_LINE(0, 0, "EEPROM Tests!!!");
  LCD_POS(1, 0);

//  KBD_RESET_KEY;
  while (1) {
    data[0]++;
    eepromWriteBytes(0, data, 8);
    delayms(500);
    LCD_POS(1, 0);
    LCD_PUTCH('x');
    eepromReadBytes(0, rval, 8);
    LCD_PUTCH('y');
    ui2 = (rval[0]>>4) & 0xF;
    LCD_PUTCH(((ui2>9)?'a'-10:'0')+ui2);
    ui2 = rval[0] & 0xF;
    LCD_PUTCH(((ui2>9)?'a'-10:'0')+ui2);
    for (ui2=0; ui2<0xFF; ui2++)
      for (ui1=0; ui1<0xFFF; ui1++)
	{}
  }

  return 0;
}
