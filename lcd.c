#include<stdint.h>
#include<p89v51rd2.h>

#include "lcd.h"

void
LCD_init(void)
{
  /*  Function set: 2 Line, 8-bit, 5x7 dots */
  LCD_cmd(LCD_CMD_2LINE_5x7);

  /* Display on, Curson blinking command */
  LCD_cmd(LCD_CMD_DISON_CURON_BLINKON);

  /* Clear LCD */
  LCD_cmd(LCD_CMD_CLRSCR);

  /* Entry mode, auto increment with no shift */
  LCD_cmd(LCD_CMD_INC_CUR);

  LCD_busy
}

#define LCD_scroll	{      \
  uint8_t ui1, ch[16];	       \
  LCD_cmd(LCD_CMD_CUR_20);     \
  for (ui1=0; ui1<16; ui1++) { \
    LCD_PORT = 0xFF;		       \
    LCD_rs = 1;                \
    LCD_rw = 1;                \
    LCD_en = 1;                \
    LCD_busy                   \
    ch[ui1] = LCD_PORT;        \
    LCD_en = 0;                \
  } \
  LCD_cmd(LCD_CMD_CUR_10);     \
  for (ui1=0; ui1<16; ui1++) { \
    LCD_wrchar(ch[ui1]);       \
  } \
}

void
LCD_sendstring(uint8_t *var)
{
  /* Disable all interrupts */

  /* Consume the new string */
  while (0 != var[0]) {
    LCD_wrchar(var[0]);
    var ++;
  }
  LCD_idle_drive

  /* enable intr */
}

#ifdef LCD_MAIN

void main(void)
{
  LCD_init();
  LCD_busy
  LCD_cmd(LCD_CMD_CUR_10);
  LCD_sendstring("Hello World 7");
  LCD_cmd(LCD_CMD_CUR_20);
  LCD_sendstring("Hello World 8");
  LCD_busy
  LCD_cmd(LCD_CMD_CUR_10);
  LCD_sendstring("Hello World 8");
  LCD_cmd(LCD_CMD_CUR_20);
  LCD_sendstring("Hello World 9");
  LCD_busy
  LCD_cmd(LCD_CMD_CUR_10);
  LCD_sendstring("Hello World 9");
  LCD_cmd(LCD_CMD_CUR_20);
  LCD_sendstring("Hello World 10");
  LCD_busy
}

#endif
