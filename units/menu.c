#include <stdio.h>
#include <stdint.h>

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
menu_func_t menu_names[] = {
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

void
menu_main(void)
{
  uint8_t prev_menu_selected = 0, menu_selected;

menu_main_start:
  /* initialize */
  menu_selected = 0;

  /* Wait until get command from user */
  while (0 == KbHit) {
    LCD_WR(1, 0, menu_names[prev_menu_selected], MENU_NAMES_LEN);
    sleep(1);
  }

  if (KEY_SC_ENTER == KbHit) {
  } else {
    /* could be hotkey for menu */
  }

  /* recursive call, capture the case when you have unexpected
     exit */
  goto menu_main_start;
}
