
#include "kbd.h"

uint8_t KbdData;
uint8_t KbdDataAvail;

uint16_t test_key_idx = -1;
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
    assert (0xFF != test_key_idx);
    KbdData = KEY_SC_ENTER;
    *p_key = KEY_SC_ENTER;
    *p_key_n = 1;
    *p_key_s = 0;
    test_key_idx = -1;
    printf("hack2 kbd.c sending:0x%x\n", KbdData);
    return;
  }

  if ((KEY_SC_ENTER==test_key[test_key_idx]) || (KEY_SC_LEFT==test_key[test_key_idx]) || (KEY_SC_RIGHT==test_key[test_key_idx])) {
    KbdData = test_key[test_key_idx];
    *p_key = KbdData;
    *p_key_n = 1;
    *p_key_s = 0;
    test_key_idx++;
    printf("hack kbd.c sending:0x%x\n", KbdData);
    return;
  }

  assert(0);
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
