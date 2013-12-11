#include<stdint.h>
#include<p89v51rd2.h>

#include "lcd.h"

void
LCD_init(void)
{
  /* */
  LCD_CLRSCR;

  /*  Function set: 2 Line, 8-bit, 5x7 dots */
  LCD_cmd(LCD_CMD_2LINE_5x7);
  LCD_cmd(LCD_CMD_2LINE_5x7);
  LCD_cmd(LCD_CMD_2LINE_5x7);
  LCD_cmd(LCD_CMD_2LINE_5x7);
  LCD_cmd(LCD_CMD_2LINE_5x7);

  /* Display on, Curson blinking command */
  LCD_cmd(LCD_CMD_DISON_CURON_BLINKON);

  /* Clear LCD */
  LCD_cmd(LCD_CMD_CLRSCR);

  /* Entry mode, auto increment with no shift */
  LCD_cmd(LCD_CMD_INC_CUR);

  LCD_busy
}

uint8_t lcd_buf[LCD_MAX_ROW][LCD_MAX_COL];
uint8_t lcd_buf_prop;
uint8_t *lcd_buf_p;

void
LCD_refresh(void)
{
  uint8_t ui1;
  uint8_t *buf_p = (uint8_t *)lcd_buf;

#ifdef UNIT_TEST
  printf("----------\n");
#endif
  /* Consume the new string */
  LCD_cmd(LCD_CMD_CUR_10);
  for (ui1=0; ui1<LCD_MAX_COL; ui1++) {
    LCD_wrchar(buf_p[0]);
    buf_p ++;
  }
#ifdef UNIT_TEST
  putchar('\n');
#endif
  LCD_cmd(LCD_CMD_CUR_20);
  for (ui1=0; ui1<LCD_MAX_COL; ui1++) {
    LCD_wrchar((lcd_buf_prop & LCD_PROP_NOECHO_L2) ? '*' : buf_p[0]);
    buf_p ++;
  }
#ifdef UNIT_TEST
  putchar('\n');
#endif
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
