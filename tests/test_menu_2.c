#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>

#include "kbd.h"

uint8_t test_key_idx = -1;
uint8_t *test_key = NULL;

#define INIT_TEST_KEYS(A) do { test_key = A; test_key_idx = 0; } while (0)
#define KBD_SHIFT                0x80
#define KBD_KEY_TIMES(N)   ((N-1)<<4)
#define KBD_KEY(N)                  N

uint8_t kbd_reverse_map(uint8_t);

void
get_test_key(uint8_t* p_key, uint8_t* p_key_n, uint8_t* p_key_s)
{
  static uint8_t do_correct = 0;

  if (0 != KbdData) /* not yet consumed the old data */
    return;
  if (0xFF == test_key_idx) /* no data yet */
    return;
  if ((0 == test_key[test_key_idx]) && (0 == do_correct)) { /* completed */
    assert (test_key_idx);
    assert (0xFF != test_key_idx);
    KbdData = KEY_SC_ENTER;
    *p_key = KEY_SC_ENTER;
    *p_key_n = 1;
    *p_key_s = 0;
    test_key_idx = -1;
    return;
  }

  KbdData = kbd_reverse_map(test_key[test_key_idx]);
  test_key_idx++;
  if (2 <= do_correct) {
    do_correct = 0;
    //    printf("Added correct key\n");
  } else if (1 == do_correct) {
    KbdData = KEY_SC_LEFT;
    test_key_idx--;
    do_correct++;
    //    printf("Added back key\n");
  } else if ((0 == do_correct) && (0 == (rand() % 2))) {
    KbdData = 'a' + (rand() % 26);
    //    printf("Added random key: %c\n", KbdData);
    do_correct++;
    test_key_idx--;
  }
  uint8_t key, key_n, key_s;
  KBD_GET_KEY;
  *p_key = key;
  *p_key_n = key_n;
  *p_key_s = key_s;
  //  printf("do_correct:%d\n", do_correct);
}

/* Redefine certain macros for testing purpose */
#undef KBD_NOT_HIT
#define KBD_NOT_HIT (0)
#undef KBD_GET_KEY
#define KBD_GET_KEY get_test_key(&key, &key_n, &key_s)

#include "kbd.c"
#include "lcd.c"
#include "menu.c"

uint8_t
kbd_reverse_map(uint8_t in)
{
  uint8_t ui1;
  for (ui1=0; ui1<(KCHAR_ROWS*KCHAR_COLS); ui1++) {
    if (in == keyChars[ui1]) {
      uint8_t ret, ui2;
      ret = ui1/KCHAR_COLS;
      ui2 = ui1%KCHAR_COLS;
      if (ui2>=KCHAR_SHIFT_SZ) {
	ui2 %= KCHAR_SHIFT_SZ;
	ret |= KBD_SHIFT;
      }
      ret |= ui2<<4;
      //      printf("kbd_reverse_map:0x%x\n", ret);
      return ret;
    }
  }
  assert(0);
}

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
  for (loop=0; loop<1000; loop++) {
    uint8_t hour, year;
    hour = 1 + (rand() % 60);
    min = rand() % 100;
    sprintf(inp, "%02d%02d%04d", date, hour, min);
    INIT_TEST_KEYS(inp);
    menu_getopt("dflkjf", &arg1, MENU_ITEM_DATE);
    printf("string:%s\n", inp);
    printf("hour:%d org:%d\n", arg1.date.hour, hour);
    printf("year:%d org:%d\n", arg1.date.year, year);
    assert(hour == arg1.time.hour);
    assert(year == arg1.date.year);
  }

  return 0;
}
