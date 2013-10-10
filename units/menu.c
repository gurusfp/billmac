#include <stdint.h>
#include <p89v51rd2.h>

#include "menu.h"

#define ROW_JOIN ,
#define COL_JOIN
#define MENU_NAME(A) A
#define MENU_FUNC(A)
#define ARG1(A, B)
#define ARG2(A, B)
char *menu_names = {
  MENU_ITEMS
};
#undef  ARG2
#undef  ARG1
#undef  MENU_FUNC
#undef  MENU_NAME
#undef  ROW_JOIN
#undef  COL_JOIN

typedef void menu_func_t(void);

#define ROW_JOIN ,
#define COL_JOIN
#define MENU_NAME(A)
#define MENU_FUNC(A) A
#define ARG1(A, B)
#define ARG2(A, B)
menu_func_t menu_handlers[] = {
  MENU_ITEMS
};
#undef  ARG2
#undef  ARG1
#undef  MENU_FUNC
#undef  MENU_NAME
#undef  ROW_JOIN
#undef  COL_JOIN

#define ROW_JOIN ,
#define COL_JOIN
#define MENU_NAME(A)
#define MENU_FUNC(A)
#define ARG1(A, B) B,
#define ARG2(A, B) B
uint8_t menu_args[] = {
  MENU_ITEMS
};
#undef  ARG2
#undef  ARG1
#undef  MENU_FUNC
#undef  MENU_NAME
#undef  ROW_JOIN
#undef  COL_JOIN

#define ROW_JOIN ,
#define COL_JOIN
#define MENU_NAME(A)
#define MENU_FUNC(A)
#define ARG1(A, B) A,
#define ARG2(A, B) A
uint8_t menu_prompts[] = {
  MENU_ITEMS
};
#undef  ARG2
#undef  ARG1
#undef  MENU_FUNC
#undef  MENU_NAME
#undef  ROW_JOIN
#undef  COL_JOIN

char    menu_prompt_str[] = MENU_PROMPTS;

menu_arg_t arg1, arg2;

void
menu_getopt(uint8_t *prompt, uint8_t *optopt, menu_arg_t *arg)
{

}

void
menu_main(void)
{
  uint8_t menu_selected, key;

  /* initialize */
  menu_selected = 0;

menu_main_start:
  /* Wait until get command from user */
  /* FIXME: Need to shop company name in 0,0 */
  while (0xFF == KbdHit) {
    LCD_WR(1, 0, menu_names[menu_selected], MENU_NAMES_LEN);
    DELAY(0xFF);
  }

  if (KEY_SC_ENTER == KbdHit) {
    menu_getopt(menu_prompt_str+((menu_prompts[menu_selected<<1])<<2), menu_args+(menu_selected<<1), &arg1);
    menu_getopt(menu_prompt_str+((menu_prompts[(menu_selected<<1)+1])<<2), menu_args+((menu_selected<<1)+1), &arg2);
    menu_call();
  } else if (KEY_SC_LEFT == KbdHit) {
    menu_selected--;
    key = MENU_MAX - 1;
  } else if (KEY_SC_RIGHT == KbdHit) {
    menu_selected++;
    key = 0;
  } else {
    /* could be hotkey for menu */
    menu_selected *= 10;

    /* Critical section : disable any keypress now */
    ET0 = 0;
    key = (KbdHit & 0xF0)>>4;
    if (0xF == key) {
      key = KbdHit & 0xF;
      KbdHit = 0xFF;
    } else {
      KbdHit |= 0xF0;
    }
    ET0 = 1;

    menu_selected += key;
  }

  /* handle out of bounds */
  if (menu_selected >= MENU_MAX)
    menu_selected = key;

  /* recursive call, capture the case when you have unexpected
     exit */
  goto menu_main_start;
}
