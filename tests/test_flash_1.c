#include<stdint.h>
#include<p89v51rd2.h>

#include "assert.h"
#include "crc.h"
#include "lcd.h"
#include "kbd.h"
#include "billing.h"
#include "i2c.h"
#include "flash.h"
#include "ep_store.h"
#include "menu.h"

#include "crc.c"
#include "lcd.c"
#include "kbd.c"
#include "i2c.c"
#include "flash.c"
#include "ep_store.c"
#include "menu.c"

#define MAIN_NOMAIN
#include "main.c"

#define DELAY(N) { 				\
  uint8_t ui1, ui2;				\
  for(ui1=0; ui1<N; ui1++)			\
    for(ui2=0; ui2<0xFF; ui2++)			\
      {} \
  }

#define LCD_Display() { \
  uint8_t ui2;				\
  LCD_cmd(LCD_CMD_CUR_10);  \
    for(ui2=0; ui2<LCD_MAX_COL; ui2++)			\
      LCD_wrchar(lcd_buf[0][ui2]);  \
  LCD_cmd(LCD_CMD_CUR_20);  \
    for(ui2=0; ui2<LCD_MAX_COL; ui2++)			\
      LCD_wrchar(lcd_buf[1][ui2]);  \
}

void
main(void)
{
  uint8_t ui1;

start_main:
  LCD_init();
  DELAY(100);
  LCD_cmd(LCD_CMD_CUR_10);
  DELAY(100);
  LCD_WR_LINE(0, 0, "Hello World 1");
  DELAY(100);

  FlashWriteByte (0xF000, 0x1);
  FlashWriteByte (0xF001, 0x2);
  FlashWriteByte (0xF002, 0x3);
  FlashWriteByte (0xF003, 0x4);

  ui1 = FlashReadByte(0xF000);
  LCD_WR("Hello World ");
  LCD_wrchar('0'+ui1);
  DELAY(10);
  ui1 = FlashReadByte(0xF001);
  LCD_WR("Hello World ");
  LCD_wrchar('0'+ui1);
  DELAY(10);
  ui1 = FlashReadByte(0xF002);
  LCD_WR("Hello World ");
  LCD_wrchar('0'+ui1);
  DELAY(10);
  ui1 = FlashReadByte(0xF003);
  LCD_WR("Hello World ");
  LCD_wrchar('0'+ui1);
  DELAY(10);

  goto start_main;
}
