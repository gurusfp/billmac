#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define __code
#define __idata
#define __pdata
#define __sbit  uint8_t

#define ERROR(msg) fprintf(stderr, msg)

#include "assert.h"
#include "billing.h"
#include "crc.h"
#include "lcd.h"
#include "kbd.h"
#include "i2c.h"
#include "uart.h"
#include "flash.h"
#include "ep_store.h"
#include "printer.h"
#include "menu.h"

#include "assert.c"
#include "crc.c"
#include "lcd.c"
#include "kbd.c"
#include "i2c.c"
#include "uart.c"
#include "flash.c"
#include "ep_store.c"
#include "printer.c"
#include "menu.c"

uint8_t inp[FLASH_SECTOR_SIZE];

int
main(void)
{
  uint32_t loop;
  uint8_t ui1;

  srand(time(NULL));

  /* test string */
  for (loop=0; loop<1000; loop++) {
    for (ui1=0; ui1<LCD_MAX_COL/2; ui1++) {
      if (0 == (rand() % 3))
	inp[ui1] = 'A' + (rand()%26);
      else
	inp[ui1] = 'a' + (rand()%26);
    }
    INIT_TEST_KEYS(inp);
    KBD_RESET_KEY;
    menu_getopt("Prompt 1", &arg1, MENU_ITEM_STR);
    assert(0 == strncmp("Promp ?         ", lcd_buf, LCD_MAX_COL));
    if (0 != strncmp("Promp ?         ", lcd_buf, LCD_MAX_COL)) {
#ifdef DEBUG
      for (ui1=0; ui1<LCD_MAX_COL; ui1++)
	putchar(lcd_buf[0][ui1]);
      putchar('\n');
      for (ui1=0; ui1<LCD_MAX_COL; ui1++)
	putchar(lcd_buf[1][ui1]);
      putchar('\n');
#endif
    }
    assert(0 == strncmp(inp, lcd_buf[1], LCD_MAX_COL/2));
    if (0 != strncmp(inp, lcd_buf[1], LCD_MAX_COL/2)) {
      printf("lcd_buf:");
      for (ui1=0; ui1<LCD_MAX_COL; ui1++)
	printf("%c ", lcd_buf[1][ui1]);
      printf("\ninp:");
      for (ui1=0; ui1<LCD_MAX_COL; ui1++)
	printf("%c ", inp[ui1]);
      printf("\n");
    }
    assert(0 == strncmp("        ", lcd_buf[0]+LCD_MAX_COL+LCD_MAX_COL/2, LCD_MAX_COL/2));
    if (0 != strncmp("        ", lcd_buf[0]+LCD_MAX_COL+LCD_MAX_COL/2, LCD_MAX_COL/2)) {
      printf("\ninp:");
      for (ui1=0; ui1<LCD_MAX_COL/2; ui1++)
	printf("%c ", (lcd_buf[0]+LCD_MAX_COL+LCD_MAX_COL/2)[ui1]);
      printf("\n");
    }
  }

  /* */
  for (loop=0; loop<1000; loop++) {
    for (ui1=0; ui1<LCD_MAX_COL; ui1++) {
      if (0 == (rand() % 3))
	inp[ui1] = 'A' + (rand()%26);
      else
	inp[ui1] = 'a' + (rand()%26);
    }
    INIT_TEST_KEYS(inp);
    //    printf("After construction:%s\n", inp);
    KBD_RESET_KEY;
    menu_getopt("mlpsdlfjlalkjf", &arg1, MENU_ITEM_STR);
    //    printf("After getopt:%s\n", inp);
    assert(0 == strncmp("mlpsd ?          ", lcd_buf, LCD_MAX_COL));
    if (0 != strncmp("mlpsd ?          ", lcd_buf, LCD_MAX_COL)) {
#ifdef DEBUG
      for (ui1=0; ui1<LCD_MAX_COL; ui1++)
	putchar(lcd_buf[0][ui1]);
      putchar('\n');
      for (ui1=0; ui1<LCD_MAX_COL; ui1++)
	putchar(lcd_buf[1][ui1]);
      putchar('\n');
#endif
    }
    assert(0 == strncmp(inp, lcd_buf[1], LCD_MAX_COL));
    if (0 != strncmp(inp, lcd_buf[1], LCD_MAX_COL)) {
      printf("lcd_buf:");
      for (ui1=0; ui1<LCD_MAX_COL; ui1++)
	printf("%c ", lcd_buf[1][ui1]);
      printf("\ninp:");
      for (ui1=0; ui1<LCD_MAX_COL; ui1++)
	printf("%c ", inp[ui1]);
      printf("\n");
    }
  }

  /* integer */
  for (loop=0; loop<1000; loop++) {
    //    printf("loop:%d\n", loop);
    uint32_t r1 = rand() & 0xFFFFFF, r2;
    uint8_t  s[16], ui2;
    r2 = r1;
    for (ui1=0; r1; ui1++) {
      s[ui1] = '0' + r1%10;
      r1 /= 10;
    }
    inp[ui1] = 0;
    for (ui2=0;ui1;ui1--, ui2++) {
      inp[ui1-1] = s[ui2];
    }
    INIT_TEST_KEYS(inp);
    KBD_RESET_KEY;
    menu_getopt("lsjdflkjf", &arg1, MENU_ITEM_ID);
    r1 = arg1.value.integer.i8;
    r1 <<= 16;
    r1 |= arg1.value.integer.i16;
    assert(r2 == r1);
    if (r2 != r1) {
      printf("r2:0x%x r1:0x%x\n", r2, r1);
    }
  }

  /* date DDMMYYYY */
  for (loop=0; loop<1000; loop++) {
    uint8_t date, month, year;
    date = 1 + (rand() % 28);
    month = 1 + (rand() % 12);
    year = rand() % 100;
    sprintf(inp, "%02d%02d%04d", date, month, 2000+year);
    INIT_TEST_KEYS(inp);
    KBD_RESET_KEY;
    menu_getopt("dflkjf", &arg1, MENU_ITEM_DATE);
    if ( (date != arg1.value.date.date) || (month != arg1.value.date.month) || (year != arg1.value.date.year) ) {
      printf("string:%s\n", inp);
      printf("date:%d org:%d\n", arg1.value.date.date, date);
      printf("month:%d org:%d\n", arg1.value.date.month, month);
      printf("year:%d org:%d\n", arg1.value.date.year, year);
    }
    assert(date == arg1.value.date.date);
    assert(month == arg1.value.date.month);
    assert(year == arg1.value.date.year);
  }

  /* month MMYYYY */
  for (loop=0; loop<1000; loop++) {
    uint8_t month, year;
    month = 1 + (rand() % 12);
    year = rand() % 100;
    sprintf(inp, "%02d%04d", month, 2000+year);
    INIT_TEST_KEYS(inp);
    KBD_RESET_KEY;
    menu_getopt("kjslfjklsdlff", &arg1, MENU_ITEM_MONTH);
    if ( (month != arg1.value.date.month) || (year != arg1.value.date.year) ) {
      printf("string:%s\n", inp);
      printf("month:%d org:%d\n", arg1.value.date.month, month);
      printf("year:%d org:%d\n", arg1.value.date.year, year);
    }
    assert(month == arg1.value.date.month);
    assert(year == arg1.value.date.year);
  }

  /* Time HHMM */
  for (loop=0; loop<1000; loop++) {
    uint8_t hour, min;
    hour = rand() % 24;
    min = rand() % 60;
    sprintf(inp, "%02d%02d", hour, min);
    INIT_TEST_KEYS(inp);
    KBD_RESET_KEY;
    menu_getopt("dflkjf", &arg1, MENU_ITEM_TIME);
    if ( (hour != arg1.value.time.hour) || (min != arg1.value.time.min) ) {
      printf("string:%s\n", inp);
      printf("hour:%d org:%d\n", arg1.value.time.hour, hour);
      printf("min:%d org:%d\n", arg1.value.time.min, min);
      printf("%s\n", lcd_buf[0]);
    }
    assert(0 == strncmp("dflkj ?          ", lcd_buf, LCD_MAX_COL));
    assert(hour == arg1.value.time.hour);
    assert(min == arg1.value.time.min);
  }

  /* Get Choice */
  KBD_RESET_KEY;
  for (loop=0; loop<1000; loop++) {
    //    printf("loop:%d\n", (uint32_t)loop);
    uint8_t ui1, ui2;
    uint16_t ui3;
    ui1 = rand() % MENU_STR1_IDX_NUM_ITEMS; /* choosen one */
    ui2 = 0; ui3 = 0;
    while (ui1 != ui2) {
      if (rand()%2) {
	inp[ui3] = ASCII_LEFT;
	ui3++;
	ui2 = (0==ui2) ? MENU_STR1_IDX_NUM_ITEMS-1 : ui2-1;
      } else {
	inp[ui3] = ASCII_RIGHT;
	ui3++;
	ui2 = (MENU_STR1_IDX_NUM_ITEMS-1==ui2) ? 0 : ui2+1;
      }
    }
    inp[ui3] = 0;
    if (ui3 >= FLASH_SECTOR_SIZE)
      continue;
    INIT_TEST_KEYS(inp);
    ui3 = menu_getchoice("what crap?", menu_str1, MENU_STR1_IDX_NUM_ITEMS);
#if 0
    assert(0 == strncmp("what : Name     ", lcd_buf, LCD_MAX_COL));
    if (0 != strncmp("what : Name     ", lcd_buf, LCD_MAX_COL)) {
#ifdef DEBUG
      for (ui1=0; ui1<LCD_MAX_COL; ui1++)
	putchar(lcd_buf[0][ui1]);
      putchar('\n');
      for (ui1=0; ui1<LCD_MAX_COL; ui1++)
	putchar(lcd_buf[1][ui1]);
      putchar('\n');
#endif
    }
#endif
    if (ui3 != ui1) {
      for (ui2=0; 0 != inp[ui2]; ui2++) {
	printf("0x%x ", inp[ui2]);
      }
      printf("\nExp:0x%x Act:0x%x\n", ui1, ui3);
    }
    assert(ui3 == ui1);
  }

  return 0;
}
