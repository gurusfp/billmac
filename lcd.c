#include<stdint.h>
#include<p89v51rd2.h>

#include "lcd.h"

uint8_t lcd_buf[LCD_MAX_ROW][LCD_MAX_COL];
uint8_t lcd_buf_prop;
uint8_t *lcd_buf_p;

void
LCD_init(void)
{

#if 4 == LCD_DPORT_SIZE
  delayms(100);
  LCD_wrnib(3);
  delayms(100);
  LCD_wrnib(3);
  delayms(100);
  LCD_wrnib(3);
  delayms(100);
  LCD_wrnib(2);
  delayms(100);
  delayms(100);
#endif

  /*  Function set: 2 Line, 8-bit, 5x7 dots */
  LCD_cmd(LCD_CMD_2LINE_5x7);
  LCD_cmd(LCD_CMD_2LINE_5x7);
  LCD_cmd(LCD_CMD_2LINE_5x7);
  LCD_cmd(LCD_CMD_2LINE_5x7);
  LCD_cmd(LCD_CMD_2LINE_5x7);
  delayms(100);

  /* Display on, Curson blinking command */
  LCD_cmd(LCD_CMD_DISON_CURON_BLINKON);
  delayms(100);

  /* Clear LCD */
  LCD_cmd(LCD_CMD_CLRSCR);
  LCD_CLRSCR;
  delayms(100);

  /* Entry mode, auto increment with no shift */
  LCD_cmd(LCD_CMD_INC_CUR);

  delayms(100);
}

#ifdef LCD_MAIN

void main(void)
{
  uint8_t ui1;

  LCD_init();
  delayms(2);
  LCD_CLRSCR

  LCD_WR(0, 0, "Hello World 7", 13);
  LCD_WR(1, 0, "Hello World 8", 13);
  LCD_refresh();
  for (ui1=0; ui1<10; ui1++)
    delayms(2);

  LCD_WR(0, 0, "Hello World 8", 13);
  LCD_WR(1, 0, "Hello World 9", 13);
  LCD_refresh();
  for (ui1=0; ui1<10; ui1++)
    delayms(2);

  LCD_WR(0, 0, "Hello World 9", 13);
  LCD_WR(1, 0, "Hello World 10", 14);
  LCD_refresh();
  for (ui1=0; ui1<10; ui1++)
    delayms(2);
}

#endif
