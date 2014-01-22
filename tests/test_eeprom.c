#include<stdint.h>
#include<p89v51rd2.h>

#define  assert(X)

#include "delay.h"
#include "lcd.h"
#include "i2c.h"
#include "kbd.h"
#include "main.h"
#include "uart.h"
#include "crc.h"

#include "lcd.c"
#include "i2c.c"
#include "kbd.c"
#include "uart.c"
#include "crc.c"

#define MAIN_NOMAIN
#include "main.c"

int
main(void)
{
  uint16_t ui1;
  uint8_t ui2, rdat;
  uint8_t errors;

  LCD_init();
  i2cInit();
  Timer_Init();
  KbdInit();

  delayms(100);
  LCD_CLRSCR;

  LCD_WR_LINE(0, 0, "Starting!!!");
  delayms(100);

  LCD_WR_LINE(0, 0, "EEPROM Tests!!!");

//  KBD_RESET_KEY;
  rdat = 0xAB;
  errors = 0;
  for (ui1=0; ui1<EEPROM_SIZE; ui1++) {
    rdat = rand8(rdat);
    eepromWriteBytes(ui1, &rdat, 1);
    eepromReadBytes(ui1, &ui2, 1);
    LCD_POS(1, 0);
    LCD_PUT_UINT8X(rdat);
    LCD_PUTCH(':');
    LCD_PUT_UINT8X(ui2);
    if (ui2 == rdat) {
      LCD_WR_N(" PASS", 5);
    } else {
      LCD_WR_N(" FAIL", 5);
      errors++;
    }
    delayms(50);
  }
  LCD_POS(1, 0);
  if (0 == errors) {
    LCD_WR_N("Tests PASS", 10);
  } else {
    LCD_WR_N("Tests FAIL", 10);
    LCD_PUTCH(':');
    LCD_PUT_UINT8X(errors);
  }

  return 0;
}
