#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>

#include "kbd.c"
#include "lcd.c"
#include "menu.c"

uint8_t inp[LCD_MAX_COL];

int
main(void)
{
  uint32_t loop;
  uint8_t ui1;

  srand(time(NULL));

  /* test string */
/*   for (loop=0; loop<1000; loop++) { */
/*     for (ui1=0; ui1<LCD_MAX_COL/2; ui1++) { */
/*       if (0 == (rand() % 3)) */
/* 	inp[ui1] = 'A' + (rand()%26); */
/*       else */
/* 	inp[ui1] = 'a' + (rand()%26); */
/*     } */
/*     INIT_TEST_KEYS(inp); */
/*     menu_getopt("Prompt 1", &arg1, MENU_ITEM_STR); */
/* #ifdef DEBUG */
/*     for (ui1=0; ui1<LCD_MAX_COL; ui1++) */
/*       putchar(lcd_buf[0][ui1]); */
/*     putchar('\n'); */
/*     for (ui1=0; ui1<LCD_MAX_COL; ui1++) */
/*       putchar(lcd_buf[1][ui1]); */
/*     putchar('\n'); */
/* #endif */
/*     assert(0 == strncmp("Prom ?          ", lcd_buf, LCD_MAX_COL)); */
/*     assert(0 == strncmp(inp, lcd_buf[1], LCD_MAX_COL/2)); */
/*     if (0 != strncmp(inp, lcd_buf[1], LCD_MAX_COL/2)) { */
/*       printf("lcd_buf:"); */
/*       for (ui1=0; ui1<LCD_MAX_COL; ui1++) */
/* 	printf("%c ", lcd_buf[1][ui1]); */
/*       printf("\ninp:"); */
/*       for (ui1=0; ui1<LCD_MAX_COL; ui1++) */
/* 	printf("%c ", inp[ui1]); */
/*       printf("\n"); */
/*     } */
/*     assert(0 == strncmp("        ", lcd_buf[0]+LCD_MAX_COL+LCD_MAX_COL/2, LCD_MAX_COL/2)); */
/*     if (0 != strncmp("        ", lcd_buf[0]+LCD_MAX_COL+LCD_MAX_COL/2, LCD_MAX_COL/2)) { */
/*       printf("\ninp:"); */
/*       for (ui1=0; ui1<LCD_MAX_COL/2; ui1++) */
/* 	printf("%c ", (lcd_buf[0]+LCD_MAX_COL+LCD_MAX_COL/2)[ui1]); */
/*       printf("\n"); */
/*     } */
/*   } */

  /* */
/*   for (loop=0; loop<1000; loop++) { */
/*     for (ui1=0; ui1<LCD_MAX_COL; ui1++) { */
/*       if (0 == (rand() % 3)) */
/* 	inp[ui1] = 'A' + (rand()%26); */
/*       else */
/* 	inp[ui1] = 'a' + (rand()%26); */
/*     } */
/*     INIT_TEST_KEYS(inp); */
/*     printf("After construction:%s\n", inp); */
/*     menu_getopt("mlpsdlfjlalkjf", &arg1, MENU_ITEM_STR); */
/*     printf("After getopt:%s\n", inp); */
/* #ifdef DEBUG */
/*     for (ui1=0; ui1<LCD_MAX_COL; ui1++) */
/*       putchar(lcd_buf[0][ui1]); */
/*     putchar('\n'); */
/*     for (ui1=0; ui1<LCD_MAX_COL; ui1++) */
/*       putchar(lcd_buf[1][ui1]); */
/*     putchar('\n'); */
/* #endif */
/*     assert(0 == strncmp("mlps ?          ", lcd_buf, LCD_MAX_COL)); */
/*     assert(0 == strncmp(inp, lcd_buf[1], LCD_MAX_COL)); */
/*     if (0 != strncmp(inp, lcd_buf[1], LCD_MAX_COL)) { */
/*       printf("lcd_buf:"); */
/*       for (ui1=0; ui1<LCD_MAX_COL; ui1++) */
/* 	printf("%c ", lcd_buf[1][ui1]); */
/*       printf("\ninp:"); */
/*       for (ui1=0; ui1<LCD_MAX_COL; ui1++) */
/* 	printf("%c ", inp[ui1]); */
/*       printf("\n"); */
/*     } */
/*   } */

  /* integer */
  /* for (loop=0; loop<1000; loop++) { */
  /*   printf("loop:%d\n", loop); */
  /*   uint32_t r1 = rand(), r2; */
  /*   uint8_t  s[16], ui2; */
  /*   r2 = r1; */
  /*   for (ui1=0; r1; ui1++) { */
  /*     s[ui1] = '0' + r1%10; */
  /*     r1 /= 10; */
  /*   } */
  /*   inp[ui1] = 0; */
  /*   for (ui2=0;ui1;ui1--, ui2++) { */
  /*     inp[ui1-1] = s[ui2]; */
  /*   } */
  /*   INIT_TEST_KEYS(inp); */
  /*   menu_getopt("lsjdflkjf", &arg1, MENU_ITEM_ID); */
  /*   printf("r2:0x%x arg1.integer:0x%x\n", r2, arg1.integer); */
  /*   assert(r2 == arg1.integer); */
  /* } */

  /* date DDMMYYYY */
  /* for (loop=0; loop<1000; loop++) { */
  /*   uint8_t date, month, year; */
  /*   date = 1 + (rand() % 28); */
  /*   month = 1 + (rand() % 12); */
  /*   year = rand() % 100; */
  /*   sprintf(inp, "%02d%02d%04d", date, month, 2000+year); */
  /*   INIT_TEST_KEYS(inp); */
  /*   menu_getopt("dflkjf", &arg1, MENU_ITEM_DATE); */
  /*   printf("string:%s\n", inp); */
  /*   printf("date:%d org:%d\n", arg1.date.date, date); */
  /*   printf("month:%d org:%d\n", arg1.date.month, month); */
  /*   printf("year:%d org:%d\n", arg1.date.year, year); */
  /*   assert(date == arg1.date.date); */
  /*   assert(month == arg1.date.month); */
  /*   assert(year == arg1.date.year); */
  /* } */

  /* month MMYYYY */
  /* for (loop=0; loop<1000; loop++) { */
  /*   uint8_t month, year; */
  /*   month = 1 + (rand() % 12); */
  /*   year = rand() % 100; */
  /*   sprintf(inp, "%02d%04d", month, 2000+year); */
  /*   INIT_TEST_KEYS(inp); */
  /*   menu_getopt("kjslfjklsdlff", &arg1, MENU_ITEM_MONTH); */
  /*   printf("string:%s\n", inp); */
  /*   printf("month:%d org:%d\n", arg1.date.month, month); */
  /*   printf("year:%d org:%d\n", arg1.date.year, year); */
  /*   assert(month == arg1.date.month); */
  /*   assert(year == arg1.date.year); */
  /* } */

  /* Time HHMM */
  /* for (loop=0; loop<1; loop++) { */
  /*   uint8_t hour, min; */
  /*   hour = rand() % 24; */
  /*   min = rand() % 60; */
  /*   sprintf(inp, "%02d%02d", hour, min); */
  /*   INIT_TEST_KEYS(inp); */
  /*   menu_getopt("dflkjf", &arg1, MENU_ITEM_TIME); */
  /*   printf("string:%s\n", inp); */
  /*   printf("hour:%d org:%d\n", arg1.value.time.hour, hour); */
  /*   printf("min:%d org:%d\n", arg1.value.time.min, min); */
  /*   printf("%s\n", lcd_buf[0]); */
  /*   assert(0 == strncmp("dflk ?          ", lcd_buf, LCD_MAX_COL)); */
  /*   assert(hour == arg1.value.time.hour); */
  /*   assert(min == arg1.value.time.min); */
  /* } */

  /* Get Choice */

  return 0;
}
