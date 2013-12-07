
#include "kbd.h"

uint8_t KbdData;
uint8_t KbdDataAvail;

uint16_t test_key_idx = -1;
uint8_t test_key_arr_idx = 0;
#define NUM_TEST_KEY_ARR 16
uint8_t *test_key[NUM_TEST_KEY_ARR];

#define INIT_TEST_KEYS(A) do { test_key[test_key_arr_idx+1] = A; test_key_arr_idx++; assert((test_key_arr_idx+1) < NUM_TEST_KEY_ARR); } while (0)
#define KBD_SHIFT                0x80
#define KBD_KEY_TIMES(N)   ((N-1)<<4)
#define KBD_KEY(N)                  N

uint8_t kbd_reverse_map(uint8_t);

void
KbdInit(void)
{
  uint8_t ui2;

  for (ui2=0; ui2<NUM_TEST_KEY_ARR; ui2++)
    test_key[ui2] = NULL;
}

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
  printf("unable to map %0d\n", (uint32_t)in);
  assert(0);
}

/* FIXME: We make sure that if random char is introduced, a back
key is pressed and it is earsed. Testing is not being done when
the last key is a 'back key' */
void
get_test_key(uint8_t* p_key, uint8_t* p_key_n, uint8_t* p_key_s)
{
  static uint8_t do_correct = 0;

  if KBD_HIT
    return;
  if (((uint16_t) -1) == test_key_idx) {
    if (test_key_arr_idx) {
      uint8_t ui2;
      for (ui2=0; ui2<(NUM_TEST_KEY_ARR-1); ui2++) {
	test_key[ui2] = test_key[ui2+1];
      }
      test_key[NUM_TEST_KEY_ARR-1] = NULL;
      test_key_arr_idx--;
    } else
      return;
  }
  assert(test_key_idx<=FLASH_SECTOR_SIZE);
  if ((0 == test_key[0][test_key_idx]) && (0 == do_correct)) { /* completed */
    assert (((uint16_t)-1) != test_key_idx);
    KbdData = KEY_SC_ENTER;
    KbdDataAvail = 1;
    *p_key = KEY_SC_ENTER;
    *p_key_n = 1;
    *p_key_s = 0;
    test_key_idx = -1;
    //    printf("hack2 kbd.c sending:0x%x\n", KbdData);
    return;
  }

  if ((KEY_SC_ENTER==test_key[0][test_key_idx]) || (KEY_SC_LEFT==test_key[0][test_key_idx]) || (KEY_SC_RIGHT==test_key[0][test_key_idx])) {
    KbdData = test_key[0][test_key_idx];
    KbdDataAvail = 1;
    *p_key = KbdData;
    *p_key_n = 1;
    *p_key_s = 0;
    test_key_idx++;
    //    printf("hack kbd.c sending:0x%x\n", KbdData);
    return;
  }

  KbdData = kbd_reverse_map(test_key[0][test_key_idx]);
  KbdDataAvail = 1;
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

uint8_t
KbdIsShiftPressed(void)
{
  uint8_t shift = 0;

  return shift;
}

/* Redefine certain macros for testing purpose */
#undef KBD_NOT_HIT
#define KBD_NOT_HIT (0)
#undef KBD_GET_KEY
#define KBD_GET_KEY get_test_key(&key, &key_n, &key_s)
