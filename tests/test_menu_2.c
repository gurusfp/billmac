#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

#include "kbd.h"

uint8_t test_key_idx = 0;
uint8_t *test_key = NULL;

#define INIT_TEST_KEYS(A) do { test_key = A; test_key_idx = 0; } while (0)
#define KBD_SHIFT                0x80
#define KBD_KEY_TIMES(N)   ((N-1)<<4)
#define KBD_KEY(N)                  N

uint8_t kbd_reverse_map(uint8_t);

void
get_test_key(uint8_t* p_key, uint8_t* p_key_n, uint8_t* p_key_s)
{
  if (0 != KbdData) /* not yet consumed the old data */
    return;
  if (0 == test_key[test_key_idx]) { /* completed */
    if (test_key_idx) {
      KbdData = KEY_SC_ENTER;
      *p_key = KEY_SC_ENTER;
      *p_key_n = 1;
      test_key_idx = 0;
    }
    return;
  }

  KbdData = kbd_reverse_map(test_key[test_key_idx]);
  test_key_idx++;
  if (0 == (rand % 5)) {
    KbdData = KEY_SC_LEFT;
  }
  uint8_t key, key_n, key_s;
  KBD_GET_KEY;
  *p_key = key;
  *p_key_n = key_n;
  *p_key_s = key_s;
}

/* Redefine certain macros for testing purpose */
#undef KBD_NOT_HIT
#define KBD_NOT_HIT (0)
#undef KBD_GET_KEY
#define KBD_GET_KEY get_test_key(&key, &key_n, &key_s)

#include "kbd.c"
#include "menu.c"

uint8_t
kbd_reverse_map(uint8_t in)
{
  for (uint8_t ui1=0; ui1<(KCHAR_ROWS*KCHAR_COLS); ui1++) {
    if (in == keyChars[ui1]) {
      uint8_t ret, ui2;
      ret = ui1/KCHAR_COLS;
      ui2 = ui1%KCHAR_COLS;
      if (ui2>=KCHAR_SHIFT_SZ) {
	ui2 %= KCHAR_SHIFT_SZ;
	ret |= KBD_SHIFT;
      }
      ret |= ui2<<4;
      return ret;
    }
  }
  assert(0);
}

int
main(void)
{
  srand(time());
  /* test string */
  INIT_TEST_KEYS("Hello World");
  menu_main();

  return 0;
}
