#include<stdint.h>
#include<p89v51rd2.h>

#include "lcd.h"

uint8_t lcd_buf[LCD_MAX_ROW][LCD_MAX_COL];
uint8_t lcd_buf_prop;
uint8_t *lcd_buf_p;

void
LCD_init(void)
{
  uint8_t ui2, ui3;

  /* */
  LCD_CLRSCR;

  /*  Function set: 2 Line, 8-bit, 5x7 dots */
  LCD_cmd(LCD_CMD_2LINE_5x7);
  LCD_cmd(LCD_CMD_2LINE_5x7);
  LCD_cmd(LCD_CMD_2LINE_5x7);
  LCD_cmd(LCD_CMD_2LINE_5x7);
  LCD_cmd(LCD_CMD_2LINE_5x7);
  LCD_busy;

  /* Display on, Curson blinking command */
  LCD_cmd(LCD_CMD_DISON_CURON_BLINKON);
  LCD_busy;

  /* Clear LCD */
  LCD_cmd(LCD_CMD_CLRSCR);
  for (ui2=0; ui2<LCD_MAX_ROW; ui2++)
    for (ui3=0; ui3<LCD_MAX_COL; ui3++)
      lcd_buf[ui2][ui3] = ' ';
  LCD_busy;

  /* Entry mode, auto increment with no shift */
  LCD_cmd(LCD_CMD_INC_CUR);

  LCD_busy;
}

#ifdef LCD_MAIN

void main(void)
{
  uint8_t ui1;

  LCD_init();
  LCD_busy
  LCD_CLRSCR

  LCD_WR(0, 0, "Hello World 7", 13);
  LCD_WR(1, 0, "Hello World 8", 13);
  LCD_refresh();
  for (ui1=0; ui1<10; ui1++)
    LCD_busy

  LCD_WR(0, 0, "Hello World 8", 13);
  LCD_WR(1, 0, "Hello World 9", 13);
  LCD_refresh();
  for (ui1=0; ui1<10; ui1++)
    LCD_busy

  LCD_WR(0, 0, "Hello World 9", 13);
  LCD_WR(1, 0, "Hello World 10", 14);
  LCD_refresh();
  for (ui1=0; ui1<10; ui1++)
    LCD_busy
}

#endif
